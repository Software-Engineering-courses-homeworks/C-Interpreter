#include <stdarg.h>
#include "common.h"
#include "vm.h"
#include "debug.h"
#include "compiler.h"
#include <string.h>
#include "object.h"
#include "memory.h"

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

///  prints the line where the script had a runtime error
/// @param format
/// @param ...
static void runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);     ///initalize the argument list starting after 'format'.

    //Print the formatted error message to standard error output.
    vfprintf(stderr, format, args);
    va_end(args);///clean up the argument list

    //prints a new line to separate the error message
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code -1;
    int line = vm.chunk->lines[instruction];

    //prints the line number where the error occurred
    fprintf(stderr, "[line %d] in script \n", line);
    resetStack();
}

/// initializes the VM
void initVM()
{
    resetStack();
    vm.objects = NULL;
    initTable(&vm.strings);
    initTable(&vm.globals);
}

/// frees the VM
void freeVM()
{
    freeTable(&vm.strings);
    freeObjects();
}

/// pushes a value onto the VM stack
/// @param val the value that needs to be pushed
void push(Value val) {
    *vm.stackTop = val;
    vm.stackTop++;
}

/// pops the top value from the VM stack
/// @return the top value in the stack
Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

///
/// @param distance - how far down from the top to look
/// @return returns a value from the stack without popping it
static Value peek(int distance) {
    return vm.stackTop[-1-distance];
}

/// the function gets a value and returns whether it is a nil or false value or not
/// @param val the value that needs to be checked
/// @return returns true for either nil or false, false otherwise
static bool isFalsey(Value val) {
    return IS_NIL(val) || (IS_BOOL(val) && !AS_BOOL(val));
}

/// a helper function to concatenate strings
static void concatenate() {
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());

    int length = a->length + b->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString* result = takeString(chars, length);
    push(OBJ_VAL(result));
}

/// a helper function that executes the bytecode by iterating through the chunk one bytecode at a time
/// @return returns an interpreted result
static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG(arr)  (vm.chunk->constants.values[(uint32_t)arr[2] << 16 | (uint16_t)arr[1] << 8 | arr[0]])
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define READ_SHORT() (vm.ip += 2, (uint16_t)((vm.ip[-2] << 8) | (vm.ip[-1])))

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
    for (Value* slot = vm.stack; slot<vm.stackTop;slot++) {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.ip-vm.chunk->code));
#endif

    //runs through all the instructions in the chunk and returns the runtime result from the interpretation
    for(;;)
    {
        uint8_t instruction;

        //the switch is used to dispatch the instructions in the most simple way
        switch(instruction = READ_BYTE())
        {
            //the case negates the value in the top of the stacks and pushes it back in
            case OP_NEGATE:
                //checks if the value on the top of the stack is a number
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            //end of run opcode
            case OP_RETURN: return INTERPRET_OK;
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
            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_FALSE: push(BOOL_VAL(false)); break;
            //case for popping out of the stack
            case OP_POP: pop(); break;
            //case for reading from a global variable
            case OP_GET_GLOBAL:
                ObjString* name = READ_STRING();
                Value value;
                if (!tableGet(&vm.globals, name, &value)) {
                    runtimeError("Undefined variable '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(value);
                break;
            //case for equality
            case OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a,b)));
                break;
            }
            case OP_GREATER: BINARY_OP(BOOL_VAL, >); break;
            case OP_LESS: BINARY_OP(BOOL_VAL, <); break;
            //cases for arithmetic operations
            case OP_ADD: {
                if(IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                    concatenate();
                }
                else if(IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                }
                else {
                    runtimeError("Operands must be 2 numbers or 2 strings.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL,*); break;
            case OP_DIVIDE: BINARY_OP(NUMBER_VAL, /); break;
            case OP_NOT: push(BOOL_VAL(isFalsey(pop()))); break;
            case OP_PRINT:
                printValue(pop());
                printf("\n");
                break;
            case OP_DEFINE_GLOBAL: {
                ObjString* name = READ_STRING();
                tableSet(&vm.globals, name,peek(0));
                pop();
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString* name = READ_STRING();
                if(tableSet(&vm.globals, name, peek(0))) {
                    tableDelete(&vm.globals, name);
                    runtimeError("Undefined variable '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_GET_LOCAL: {
                uint8_t slot = READ_BYTE();
                push(vm.stack[slot]);
                break;
            }
            case OP_SET_LOCAL: {
                uint8_t slot = READ_BYTE();
                vm.stack[slot] = peek(0);
                break;
            }
            case OP_JUMP_IF_FALSE: {
                uint16_t offset = READ_SHORT();
                if(isFalsey(peek(0))) vm.ip += offset;
                break;
            }
            case OP_JUMP: {
                uint16_t offset = READ_SHORT();
                vm.ip += offset;
                break;
            }
            case OP_LOOP: {
                uint16_t offset = READ_SHORT();
                vm.ip -= offset;
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
/// @param source - the given source code
/// @return the interpreted result of the given chunk(Need to change)
InterpretResult interpret(const char* source)
{
    //creates a chunk and initializes it
    Chunk chunk;
    initChunk(&chunk);

    //tries to compile the code and if it fails, return a compilation error
    if(!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    //initializes the VM chunk and IP
    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    //interprets the code
    InterpretResult result = run();

    //frees the allocated memory and returns the interpretation result
    freeChunk(&chunk);
    return result;
}