#include <stdarg.h>
#include "common.h"
#include "vm.h"
#include "debug.h"
#include "compiler.h"

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

    //Print the formatted error message to standartd error output.
    vfprintf(stderr, format, args);
    va_end(args);///clean up the argument list

    //prints a new line to separate the error message
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code -1;
    int line = vm.chunk->lines[instruction];

    //prints the line number where the error occured
    fprintf(stderr, "[line %d] in script \n", line);
    resetStack();
}

/// initializes the VM
void initVM()
{
    resetStack();
}

/// frees the VM
void freeVM()
{

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

/// a helper function that executes the bytecode by iterating through the chunk one bytecode at a time
/// @return returns an interpreted result
static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG(arr)  (vm.chunk->constants.values[(uint32_t)arr[2] << 16 | (uint16_t)arr[1] << 8 | arr[0]])

//a macro to perform binary operations
#define BINARY_OP(valuetype, op) \
    do { \
        if(!IS_NUMBER(peek(0)|| !IS_NUMBER(peek(1)) { \
            runtimeError("Operands must be numbers."); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop()); \
        double a = AS_NUMBER(pop()); \
        push (valuetype(a op b)); \
        }while(false)

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
                //cheks if the value on the top of the stack is a number
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Opernad must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop)()));
                break;
            //case for a runtime result that pops the stacks.
            case OP_RETURN:
            {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
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
            //cases for arithmetic operations
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
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

