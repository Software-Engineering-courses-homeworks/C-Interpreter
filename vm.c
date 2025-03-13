#include <stdarg.h>
#include "common.h"
#include "vm.h"
#include "debug.h"
#include "compiler.h"
#include <string.h>
#include <time.h>
#include "object.h"
#include "memory.h"

VM vm;

/// converts the native C clock function into a Lox function
/// @param argCount the number of arguments the C clock function takes
/// @param args     the argument array
/// @return         the return value of C's clock function divided by the clocks per second constant
static Value clockNative(int argCount, Value* args)
{
    return NUMBER_VAL((double) clock() / CLOCKS_PER_SEC);
}

/// resets the VM's stack
static void resetStack()
{
    vm.stackTop = vm.stack;
    vm.frameCount = 0;
    vm.openUpvalues = NULL;
}

///  prints the line where the script had a runtime error
/// @param format
/// @param ... a va_list of parameters
static void runtimeError(const char* format, ...)
{
    va_list args;
    va_start(args, format); ///initialize the argument list starting after 'format'.

    //Print the formatted error message to standard error output.
    vfprintf(stderr, format, args);
    va_end(args); ///clean up the argument list

    //prints a new line to separate the error message
    fputs("\n", stderr);


    // Iterate through the active call frames in the VM to print the call stack.
    for (int i = vm.frameCount - 1; i >= 0; i--)
    {
        // Get the current call frame.
        CallFrame* frame = &vm.frames[i];

        // Retrieve the function associated with the current call frame.
        ObjFunction* function = frame->closure->function;

        // Calculate the current instruction index in the function's bytecode.
        size_t instruction = frame->ip - function->chunk.code - 1;

        fprintf(stderr, "[line %d] in ", function->chunk.lines[instruction]);

        // Print the function name if available, or "script" for the top-level code.
        if (function->name == NULL)
        {
            fprintf(stderr, "script\n");
        }
        else
        {
            fprintf(stderr, "%s()\n", function->name->chars);
        }
    }
    // Reset the VM stack, as the error has been handled.
    resetStack();
}

/// Defines a native function and registers it in the global symbol table.
/// @param name The name of the native function as a string.
/// @param function The native function to be registered.
static void defineNative(const char* name, NativeFn function)
{
    // Push the function name  and the function objects to the stack
    push(OBJ_VAL(copyString(name, (int) strlen(name))));
    push(OBJ_VAL(newNative(function)));

    // Store the function in the global table, associating it with the name.
    tableSet(&vm.globals, AS_STRING(vm.stack[0]), vm.stack[1]);

    //pop the function and his name out of the stack, as they have been stored in the globals table
    pop();
    pop();
}

/// initializes the VM
void initVM()
{
    resetStack();
    vm.objects = NULL;

    //initializes the number,capacity and stack pointer of gray objects
    vm.grayCount = 0;
    vm.grayCapacity = 0;
    vm.grayStack = NULL;
    vm.bytesAllocated = 0;
    vm.nextGC = 1024 * 1024;

    initTable(&vm.strings);
    vm.initString = NULL;
    vm.initString = copyString("init", 4);

    initTable(&vm.globals);

    defineNative("clock", clockNative);
}

/// frees the VM
void freeVM()
{
    freeTable(&vm.strings);
    vm.initString = NULL;
    freeObjects();
}

/// pushes a value onto the VM stack
/// @param val the value that needs to be pushed
void push(Value val)
{
    *vm.stackTop = val;
    vm.stackTop++;
}

/// pops the top value from the VM stack
/// @return the top value in the stack
Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}

/// Peeks a value from the stack without popping it.
/// @param distance how far down from the top to look
/// @return         returns a value from the stack without popping it
static Value peek(int distance)
{
    return vm.stackTop[-1 - distance];
}

/// Calls a function closure with the specified argument count.
/// @param closure The function closure to be called
/// @param argCount The number of arguments passed to the closure.
/// @return true if the function call was successfull, false igf not.
static bool call(ObjClosure* closure, int argCount)
{
    // checks if the number of arguments passed to the closure is correct
    if (argCount != closure->function->arity)
    {
        runtimeError("Expected %d arguments but got %d.", closure->function->arity, argCount);
        return false;
    }

    // checks if the stack has enough space for the new frame
    if (vm.frameCount == FRAMES_MAX)
    {
        runtimeError("Stack overflow.");
        return false;
    }

    // if the number of arguments was correct, create a new frame and push it onto the stack
    CallFrame* frame = &vm.frames[vm.frameCount++];
    frame->closure = closure;
    frame->ip = closure->function->chunk.code;
    frame->slots = vm.stackTop - argCount - 1;
    return true;
}

/// Calls a function or method represented by a `Value` object
/// @param callee the object to be called.
/// @param argCount The number of arguments passed to the callee.
/// @return true if the call is successful, false otherwise.
static bool callValue(Value callee, int argCount)
{
    // Check if the callee is an object
    if (IS_OBJ(callee))
    {
        switch (OBJ_TYPE(callee))
        {
        case OBJ_BOUND_METHOD:
            {
                ObjBoundMethod* bound = AS_BOUND_METHOD(callee);
                vm.stackTop[-argCount - 1] = bound->reciever;
                return call(bound->method, argCount);
            }
        case OBJ_CLASS:
            {
                ObjClass* klass = AS_CLASS(callee);
                vm.stackTop[-argCount - 1] = OBJ_VAL(newInstance(klass));
                Value initializer;
                if (tableGet(&klass->methods, vm.initString, &initializer))
                {
                    return call(AS_CLOSURE(initializer), argCount);
                }
                else if (argCount != 0)
                {
                    runtimeError("Expected 0 arguments but got %d.", argCount);
                    return false;
                }
                return true;
            }
        case OBJ_NATIVE:
            NativeFn native = AS_NATIVE(callee);
            Value result = native(argCount, vm.stackTop - argCount);
            vm.stackTop -= argCount + 1;
            push(result);
            return true;
        case OBJ_CLOSURE:
            return call(AS_CLOSURE(callee), argCount);
        default:
            break;
        }
    }
    runtimeError("Can only call functions and classes.");
    return false;
}

/// Invokes a method from a class based on the method name.
/// @param klass The class object in which the method is defined.
/// @param name The name of the method to invoke.
/// @param argCount The number of arguments passed to the method.
/// @return `true` if the method was successfully called, `false` otherwise.
static bool invokeFromClass(ObjClass* klass, ObjString* name, int argCount)
{
    Value method;
    // Attempt to retrieve the method from the class's method table.
    if (!tableGet(&klass->methods, name, &method))
    {
        runtimeError("Undefined property '%s'.", name->chars);
        return false;
    }
    // If the method is found, call it as a closure with the provided arguments.
    return call(AS_CLOSURE(method), argCount);
}

///Invoke a method on an instance by looking it up in the class and executing it
/// @param name The name of the method to invoke
/// @param argCount The number of arguments passed to the method
/// @return True if the method was successfully invoked, false otherwise.
static bool invoke(ObjString* name, int argCount)
{
    Value receiver = peek(argCount);

    if (!IS_INSTANCE(receiver))
    {
        runtimeError("Only instances have methods.");
        return false;
    }

    ObjInstance* instance = AS_INSTANCE(receiver);

    Value value;

    if (tableGet(&instance->fields, name, &value))
    {
        vm.stackTop[-argCount - 1] = value;
        return callValue(value, argCount);
    }

    return invokeFromClass(instance->klass, name, argCount);
}

/// the function gets a class and a method name and binds it
/// @param klass the class's name identifier
/// @param name  the name of the method identifier
/// @return      true - if the method was bound successfully. false - otherwise
static bool bindMethod(ObjClass* klass, ObjString* name)
{
    Value method;
    if (!tableGet(&klass->methods, name, &method))
    {
        runtimeError("Undefined property", name->chars);
        return false;
    }

    ObjBoundMethod* bound = newBoundMethod(peek(0), AS_CLOSURE(method));

    pop();
    push(OBJ_VAL(bound));
    return true;
}

/// Captures a local variable as an upvalue, allowing it to be closed over by a closure.
/// @param local The local variable (in the stack) to be captured as an upvalue.
/// @return      an upvalue representing the captured local variable
static ObjUpvalue* captureUpvalue(Value* local)
{
    //search for an existing upvalue and reuse it
    ObjUpvalue* prevUpvalue = NULL;
    ObjUpvalue* upvalue = vm.openUpvalues;

    //iterate over the upvalue linked list in the VM
    while (upvalue != NULL && upvalue->location > local)
    {
        prevUpvalue = upvalue;
        upvalue = upvalue->next;
    }

    //if an upvalue that matches the local variable we want to close has been found, return that as the upvalue
    if (upvalue != NULL && upvalue->location == local)
    {
        return upvalue;
    }
    ObjUpvalue* createdUpvalue = newUpvalue(local);

    //inserts the new upvalue into the upvalue list
    createdUpvalue->next = upvalue;
    if (prevUpvalue == NULL)
    {
        vm.openUpvalues = createdUpvalue;
    }
    else
    {
        prevUpvalue->next = createdUpvalue;
    }

    return createdUpvalue;
}

/// Closes all upvalues that are currently open and have a location before the specified last pointer.
/// @param last The stack position (pointer) up to which the upvalues should be closed.
static void closeUpvalues(Value* last)
{
    while (vm.openUpvalues != NULL && vm.openUpvalues->location >= last)
    {
        ObjUpvalue* upvalue = vm.openUpvalues;
        upvalue->closed = *upvalue->location;
        upvalue->location = &upvalue->closed;
        vm.openUpvalues = upvalue->next;
    }
}

/// Defines a method for a given class
/// @param name The name of the method to define
static void defineMethod(ObjString* name)
{
    //Retrieves the method value from the top of the stack
    Value method = peek(0);
    ObjClass* klass = AS_CLASS(peek(1));

    //stores the method in the class's method table using the provided name.
    tableSet(&klass->methods, name, method);
    pop();
}

/// the function gets a value and returns whether it is a nil or false value or not
/// @param val the value that needs to be checked
/// @return    returns true for either nil or false, false otherwise
static bool isFalsey(Value val)
{
    return IS_NIL(val) || (IS_BOOL(val) && !AS_BOOL(val));
}

/// a helper function to concatenate strings
static void concatenate()
{
    ObjString* b = AS_STRING(peek(0));
    ObjString* a = AS_STRING(peek(1));

    int length = a->length + b->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString* result = takeString(chars, length);
    pop();
    pop();
    push(OBJ_VAL(result));
}

/// a helper function that executes the bytecode by iterating through the chunk one bytecode at a time
/// @return returns an interpreted result
static InterpretResult run()
{
    CallFrame* frame = &vm.frames[vm.frameCount - 1];
#define READ_BYTE() (*frame->ip++)
#define READ_CONSTANT() (frame->closure->function->chunk.constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG(arr)  (frame->closure->function->chunk.constants.values[(uint32_t)arr[2] << 16 | (uint16_t)arr[1] << 8 | arr[0]])
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define READ_SHORT() ((frame->ip += 2, (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1])))

    //a macro to perform binary operations
#define BINARY_OP(valueType, op) \
do { \
if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
runtimeError("Operands must be numbers."); \
return INTERPRET_RUNTIME_ERROR; \
} \
double b = AS_NUMBER(pop()); \
double a = AS_NUMBER(pop()); \
push(valueType(a op b)); \
} while (false)

    //a check for a debug flag that if present prints the trace
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++)
    {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
    }
    printf("\n");
    disassembleInstruction(&frame->closure->function->chunk, (int)(frame->ip - frame->closure->function->chunk.code));
#endif

    //runs through all the instructions in the chunk and returns the runtime result from the interpretation
    for (;;)
    {
        uint8_t instruction;

        //the switch is used to dispatch the instructions in the most simple way
        switch (instruction = READ_BYTE())
        {
        //the case negates the value in the top of the stacks and pushes it back in
        case OP_NEGATE:
            //checks if the value on the top of the stack is a number
            if (!IS_NUMBER(peek(0)))
            {
                runtimeError("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_VAL(-AS_NUMBER(pop())));
            break;
        case OP_CALL:
            {
                int argCount = READ_BYTE();
                if (!callValue(peek(argCount), argCount))
                {
                    return INTERPRET_RUNTIME_ERROR;
                }
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
        //end of run opcode
        case OP_RETURN:
            {
                // pops the return value and remove the frame from the stack
                Value result = pop();
                closeUpvalues(frame->slots);
                vm.frameCount--;
                // if the stack is empty, the program has finished
                if (vm.frameCount == 0)
                {
                    pop();
                    return INTERPRET_OK;
                }

                vm.stackTop = frame->slots;
                push(result);
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
        //case for a constant value. pushes the constants into the stack
        case OP_CONSTANT:
            {
                Value constant = READ_CONSTANT();
                push(constant);
                //printValue(constant);
                //printf("\n");
                break;
            }
        //the case handles the long constants, creates an array of the bytes and then build it as a value
        case OP_CONSTANT_LONG:
            {
                //uint32_t constant = READ_CONSTANT_LONG();
                uint8_t arr[] = {READ_BYTE(), READ_BYTE(), READ_BYTE()};
                Value constant = READ_CONSTANT_LONG(arr);
                push(constant);
                //printValue(constant);
                //printf("'\n");
                break;
            }
        //cases for nil,false and true
        case OP_NIL:
            push(NIL_VAL);
            break;
        case OP_TRUE:
            push(BOOL_VAL(true));
            break;
        case OP_FALSE:
            push(BOOL_VAL(false));
            break;
        //case for popping out of the stack
        case OP_POP:
            pop();
            break;
        //case for reading from a global variable
        case OP_GET_GLOBAL:
            ObjString* name = READ_STRING();
            Value value;
            if (!tableGet(&vm.globals, name, &value))
            {
                runtimeError("Undefined variable '%s'.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            push(value);
            break;
        case OP_SET_PROPERTY:
            {
                if (!IS_INSTANCE(peek(1)))
                {
                    runtimeError("Only instances have fields.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                ObjInstance* instance = AS_INSTANCE(peek(1));
                tableSet(&instance->fields, READ_STRING(), peek(0));
                Value value = pop();
                push(value);
                break;
            }
        //case for equality
        case OP_EQUAL:
            {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a, b)));
                break;
            }
        case OP_GREATER:
            BINARY_OP(BOOL_VAL, >);
            break;
        case OP_LESS:
            BINARY_OP(BOOL_VAL, <);
            break;
        //cases for arithmetic operations
        case OP_ADD:
            {
                if (IS_STRING(peek(0)) && IS_STRING(peek(1)))
                {
                    concatenate();
                }
                else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1)))
                {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                }
                else
                {
                    runtimeError("Operands must be 2 numbers or 2 strings.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
        case OP_SUBTRACT:
            BINARY_OP(NUMBER_VAL, -);
            break;
        case OP_MULTIPLY:
            BINARY_OP(NUMBER_VAL, *);
            break;
        case OP_DIVIDE:
            BINARY_OP(NUMBER_VAL, /);
            break;
        case OP_NOT:
            push(BOOL_VAL(isFalsey(pop())));
            break;
        case OP_PRINT:
            printValue(pop());
            printf("\n");
            break;
        case OP_DEFINE_GLOBAL:
            {
                ObjString* name = READ_STRING();
                tableSet(&vm.globals, name, peek(0));
                pop();
                break;
            }
        case OP_SET_GLOBAL:
            {
                ObjString* name = READ_STRING();
                if (tableSet(&vm.globals, name, peek(0)))
                {
                    tableDelete(&vm.globals, name);
                    runtimeError("Undefined variable '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
        case OP_GET_LOCAL:
            {
                uint8_t slot = READ_BYTE();
                push(frame->slots[slot]);
                break;
            }
        case OP_SET_LOCAL:
            {
                uint8_t slot = READ_BYTE();
                frame->slots[slot] = peek(0);
                break;
            }
        case OP_JUMP_IF_FALSE:
            {
                uint16_t offset = READ_SHORT();
                if (isFalsey(peek(0))) frame->ip += offset;
                break;
            }
        case OP_JUMP:
            {
                uint16_t offset = READ_SHORT();
                frame->ip += offset;
                break;
            }
        case OP_LOOP:
            {
                uint16_t offset = READ_SHORT();
                frame->ip -= offset;
                break;
            }
        case OP_INVOKE:
            {
                ObjString* method = READ_STRING();
                int argCount = READ_BYTE();

                if (!invoke(method, argCount))
                {
                    return INTERPRET_RUNTIME_ERROR;
                }
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
        case OP_CLOSURE:
            {
                // Fetch the constant (function) to create a closure from the current chunk of bytecode.
                ObjFunction* function = AS_FUNCTION(READ_CONSTANT());

                // Create a new closure for the function
                ObjClosure* closure = newClosure(function);
                push(OBJ_VAL(closure));

                // For each upvalue handle its closure.
                for (int i = 0; i < closure->upvalueCount; i++)
                {
                    uint8_t isLocal = READ_BYTE();
                    uint8_t index = READ_BYTE();
                    // If the upvalue is local, capture it from the current frame’s slots.
                    if (isLocal)
                    {
                        closure->upvalues[i] = captureUpvalue(frame->slots + index);
                    }
                    else // Otherwise, it's an upvalue from the enclosing closure. Copy it.
                    {
                        closure->upvalues[i] = frame->closure->upvalues[index];
                    }
                }
                break;
            }
        case OP_GET_UPVALUE:
            {
                uint8_t slot = READ_BYTE();
                push(*frame->closure->upvalues[slot]->location);
                break;
            }
        case OP_SET_UPVALUE:
            {
                uint8_t slot = READ_BYTE();
                *frame->closure->upvalues[slot]->location = peek(0);
                break;
            }
        case OP_CLOSE_UPVALUE:
            {
                closeUpvalues(vm.stackTop - 1);
                pop();
                break;
            }
        case OP_CLASS:
            push(OBJ_VAL(newClass(READ_STRING())));
            break;
        case OP_GET_PROPERTY:
            {
                // Check if the value at the top of the stack is an instance. Properties are only available on instances.
                if (!IS_INSTANCE(peek(0)))
                {
                    runtimeError("Only instances have properties.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                ObjInstance* instance = AS_INSTANCE(peek(0));
                ObjString* name = READ_STRING();

                Value value;
                // Try to get the property value from the instance's fields.
                if (tableGet(&instance->fields, name, &value))
                {
                    pop();
                    push(value);
                    break;
                }
                // If the property doesn't exist in the instance's fields, try binding a method from the class.
                if (!bindMethod(instance->klass, name))
                {
                    return INTERPRET_RUNTIME_ERROR;
                }

                break;
            }
        case OP_METHOD:
            {
                defineMethod(READ_STRING());
                break;
            }
        case OP_INHERIT:
            {
                // Get the value of the superclass, which is located at the second-to-top position on the stack.
                Value superclass = peek(1);

                // Check if the value of the superclass is actually a class. If not, show a runtime error.
                if (!IS_CLASS(superclass))
                {
                    runtimeError("Superclass must be a class.");
                    return INTERPRET_RUNTIME_ERROR;
                }

                ObjClass* subclass = AS_CLASS(peek(0));
                tableAddAll(&AS_CLASS(superclass)->methods, &subclass->methods);
                pop();
                break;
            }
        case OP_GET_SUPER:
            {
                ObjString* name = READ_STRING();
                ObjClass* superclass = AS_CLASS(pop());

                //checks if the method is actually bound
                if (!bindMethod(superclass, name))
                {
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
        case OP_SUPER_INVOKE:
            {
                ObjString* method = READ_STRING();
                int argCount = READ_BYTE();
                ObjClass* superclass = AS_CLASS(pop());

                if (!invokeFromClass(superclass, method, argCount))
                {
                    return INTERPRET_RUNTIME_ERROR;
                }

                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
#undef READ_STRING
#undef READ_SHORT
#undef BINARY_OP
}

/// interprets a given chunk to the VM and returns the interpreted result
/// @param source the given source code
/// @return       the interpreted result of the given chunk
InterpretResult interpret(const char* source)
{
    //compiles the source code and save the main function to interpret
    ObjFunction* function = compile(source);
    if (function == NULL) return INTERPRET_COMPILE_ERROR;

    //pushes the main function onto the stack
    push(OBJ_VAL(function));
    // calls the top level function to initialize the VM
    ObjClosure* closure = newClosure(function);
    pop();
    push(OBJ_VAL(closure));
    call(closure, 0);

    //interprets the code and returns the run result
    return run();
}
