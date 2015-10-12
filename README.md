# ethasm
Tools to convert Ethereum Assembly to EVM Bytecode

Notes:

1. If-then-else branch not yet supported
2. JUMPDEST needs an operand (label)
3. PUSHxx will automatically be automatically chosen to required bytes to represent operand
4. Init not included. I still haven't decided whether to design this as an inline assembly support for Solidity compiler or as a standalone compiler. To get init code just add 61xxxx80600D6000396000f300 to the beginning of .binary file where xxxx is the output (number of bytes written in hex) from ethasm
