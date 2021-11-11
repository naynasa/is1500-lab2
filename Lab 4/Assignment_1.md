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

>What is the purpose of the ALU? Why are several functions grouped together into one 
>component?