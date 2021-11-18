# Assignment 1
>Explain how each of the ALU functions are defined. In particular, you need to be able to 
>explain how subtraction works, including the use of  two's complement.

AND och OR fungerar genom att vi bara kör värdena genom respektive gates - vi kan dock antingen välja
B eller !B så vå får både A AND B och A AND !B respektive A OR B och A OR !B

Addition sker bara genom att man skickar in talen i addern(carry in är 0)

Subtraktion fungerar genom "2's complement" där:
A + (-A) = 2^N
där N är antalet bitar i talet

>How did you implement the logic for the Zero output port? Did you consider any 
>alternatives? Be prepared to explain your design choices.

We chose to simple use a 32 wide splitter into a 32 wide NOR-gate to get the output bit. This works by checking if any of the inputs are true aka if the value is non zero then negating that i.e. only 0000..0 gives output 1. 

Pros:
- simple components

Cons:
- requires alot of wires and a big splitter

We could also have used a bit extender or bit selector - which would involve less splitting and less wires but we chose the NOR gate for its simplicity. We also made the 32 to 1 NOR gate into a seperate circuit abstracting away the wires.

>What is the purpose of the ALU? Why are several functions grouped together into one 
>component?

The purpose of the ALU is to perform logical and arithmetic operations. The functions grouped together into one component makes it possibly to chose which operations should be used (via the MUX).  



# Assignment 2

>Explain if the read operation or the write operation, or both operations are clocked (updated at the clock edge). Why is it >implement this way?

The value of the register is only updated on rising edge (when clock goes from 0 to 1). This means input is clocked. The register always has an output without any delay of the value in the register, i.e. ouput isn't clocked. 

We use a clock since the registers consist of flip flops. In order to make sure the flips flops get the new configuration before outputting something we use a clock. This makes sure that the value we read isn't the old value in case the flip flops take different time or for some reason don't assume the right value before the read operation.

>Explain the semantics of reading from and writing to $0, and how you implemented this behavior
 
 $0 is implemented by adding a register which is only connected to the clock. This is done so that output values won't be changed. 

>How many bits of data can this register file store? If the address width was the same size as for a complete 32-bits MIPS >processor, how many bits would in such a case such register file store?

1+32*7 = 225 bits or 224 if you don't count the 0 bit. 32 bits for each register with a total of 7 registers. In real MIPS processors the adress width is 5 which gives 32 registers. 32 registers with 32 bits each gives a total of 32 * 32 = 1024 bits.


# Assignment 3

>Explain how you have implemented the control signals for the beq instruction. Why is this a correct solution?

We used a comparator with the corresponding op-code 4 to get the right ALUcontrol 

>Be prepared to explain why the RegDst control signal or the AluSrc signal is hooked up to certain signals. You should be prepared to explain this using the following figure.

RegDst controls which part of the instruction code that should store the return value. For some instructions its rd and for others its rt. We know this since it pipes in to A3 which is the adress where we write the result.

I vår krets ControlUnit sätts ALUSrc till 1 för addi och 0 annars. Tabellen i PPTen säger oss att lw och sw också sätter ALUSrc till 1. Föreläsningen säger vidare att ALUSrc är en flagga för om båda registerna i instruktionen är register. ALUSrc = 1 ger immidiatevärde (sign extendat från instruktionen) och ALUSrc = 0 ger register.

# Assignment 4

>Explain how the bit selection works for the alternatives that are controlled by the RegDst control signal. Which instructions are using what logic and why?

RegDst kontrollerar om vi ska använda register rs eller rt, eftersom olika kommandon använder olika. Vi plockar ut de respektive bitarna mha selectors och shifters - se diagramet.

>Explain how the beq instruction is implement, how the address is calculated, and how the signals are controlled by the control unit.

ETA = PC_next + 4*imm där imm är immidiate värdet till instruktionen

Vi räknar alltid ut ETA men endast om Branch=1 utnyttjar vi den annars gör vi bara som vanligt med PC_next = PC + 4;

# Assignment 5
pseudokod för fakultetsfunktionen
recursive version:
```c
factorial(n){
    if(n <= 0){
        //error
        return -1;
    }
    if(n == 1){
        return 1;
    }else{
        return factorial(n-1)*n;
    }
}
int k = 3; //our number
printf(factorial(k));
```
loop version:
```c
int sum = 1;
int n = 4;
while(n>1){
    sum = sum * n;
    n--;
}
```
loop version without multiplication
```c
int sum = 1;
int n = 3;
while(n>1){
    //sum = sum * n;
    //a*b = a + a + a... (b times);
    
    int b = n;
    int sum_copy = sum;
    while(b > 1){
        sum += sum_copy;
        b--;
    }
    n--;
}
```

>Show and explain how the factorial function works for arbitrary input value n (the teaching assistant will give you the value that you should test). Be prepared so that you know how to change the input value easily.

Visa och förklara factorial.asm koden 

>Explain how you implemented unconditional jumps in your program.
```s
beq $0,$0, target_label
```