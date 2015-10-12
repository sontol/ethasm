#include <iostream>
#include <sstream>
#include <map>
#include <iomanip>
#include <string>
#include <string.h>


using namespace std;
map<string,uint8_t> InstructionMap;
map<string,unsigned int> Jumpdest;

bool IsHex(const string& str);



bool IsHex(const std::string& str) {
  if (str.length() < 3 || str[0] != '0'||str.length() > 66) {
    return false;
  }

  if (str[1] != 'x' && str[1] != 'X') {
    return false;
  }

  for (size_t i = 2; i < str.length(); i++) {
    char current = str[i];
    if (current >= '0' && current <= '9') {
      continue;
    }

    if (current >= 'A' && current <= 'F') {
      continue;
    }

    if (current >= 'a' && current <= 'f') {
      continue;
    }

    return false;
  }

  return true;
}
enum class Instruction: uint8_t
{
	STOP = 0x00,		///< halts execution
	ADD,				///< addition operation
	MUL,				///< mulitplication operation
	SUB,				///< subtraction operation
	DIV,				///< integer division operation
	SDIV,				///< signed integer division operation
	MOD,				///< modulo remainder operation
	SMOD,				///< signed modulo remainder operation
	ADDMOD,				///< unsigned modular addition
	MULMOD,				///< unsigned modular multiplication
	EXP,				///< exponential operation
	SIGNEXTEND,			///< extend length of signed integer

	LT = 0x10,			///< less-than comparision
	GT,					///< greater-than comparision
	SLT,				///< signed less-than comparision
	SGT,				///< signed greater-than comparision
	EQ,					///< equality comparision
	ISZERO,				///< simple not operator
	AND,				///< bitwise AND operation
	OR,					///< bitwise OR operation
	XOR,				///< bitwise XOR operation
	NOT,				///< bitwise NOT opertation
	BYTE,				///< retrieve single byte from word

	SHA3 = 0x20,		///< compute SHA3-256 hash

	ADDRESS = 0x30,		///< get address of currently executing account
	BALANCE,			///< get balance of the given account
	ORIGIN,				///< get execution origination address
	CALLER,				///< get caller address
	CALLVALUE,			///< get deposited value by the instruction/transaction responsible for this execution
	CALLDATALOAD,		///< get input data of current environment
	CALLDATASIZE,		///< get size of input data in current environment
	CALLDATACOPY,		///< copy input data in current environment to memory
	CODESIZE,			///< get size of code running in current environment
	CODECOPY,			///< copy code running in current environment to memory
	GASPRICE,			///< get price of gas in current environment
	EXTCODESIZE,		///< get external code size (from another contract)
	EXTCODECOPY,		///< copy external code (from another contract)

	BLOCKHASH = 0x40,	///< get hash of most recent complete block
	COINBASE,			///< get the block's coinbase address
	TIMESTAMP,			///< get the block's timestamp
	NUMBER,				///< get the block's number
	DIFFICULTY,			///< get the block's difficulty
	GASLIMIT,			///< get the block's gas limit

	POP = 0x50,			///< remove item from stack
	MLOAD,				///< load word from memory
	MSTORE,				///< save word to memory
	MSTORE8,			///< save byte to memory
	SLOAD,				///< load word from storage
	SSTORE,				///< save word to storage
	JUMP,				///< alter the program counter
	JUMPI,				///< conditionally alter the program counter
	PC,					///< get the program counter
	MSIZE,				///< get the size of active memory
	GAS,				///< get the amount of available gas
	JUMPDEST,			///< set a potential jump destination

	PUSH1 = 0x60,		///< place 1 byte item on stack
	PUSH2,				///< place 2 byte item on stack
	PUSH3,				///< place 3 byte item on stack
	PUSH4,				///< place 4 byte item on stack
	PUSH5,				///< place 5 byte item on stack
	PUSH6,				///< place 6 byte item on stack
	PUSH7,				///< place 7 byte item on stack
	PUSH8,				///< place 8 byte item on stack
	PUSH9,				///< place 9 byte item on stack
	PUSH10,				///< place 10 byte item on stack
	PUSH11,				///< place 11 byte item on stack
	PUSH12,				///< place 12 byte item on stack
	PUSH13,				///< place 13 byte item on stack
	PUSH14,				///< place 14 byte item on stack
	PUSH15,				///< place 15 byte item on stack
	PUSH16,				///< place 16 byte item on stack
	PUSH17,				///< place 17 byte item on stack
	PUSH18,				///< place 18 byte item on stack
	PUSH19,				///< place 19 byte item on stack
	PUSH20,				///< place 20 byte item on stack
	PUSH21,				///< place 21 byte item on stack
	PUSH22,				///< place 22 byte item on stack
	PUSH23,				///< place 23 byte item on stack
	PUSH24,				///< place 24 byte item on stack
	PUSH25,				///< place 25 byte item on stack
	PUSH26,				///< place 26 byte item on stack
	PUSH27,				///< place 27 byte item on stack
	PUSH28,				///< place 28 byte item on stack
	PUSH29,				///< place 29 byte item on stack
	PUSH30,				///< place 30 byte item on stack
	PUSH31,				///< place 31 byte item on stack
	PUSH32,				///< place 32 byte item on stack

	DUP1 = 0x80,		///< copies the highest item in the stack to the top of the stack
	DUP2,				///< copies the second highest item in the stack to the top of the stack
	DUP3,				///< copies the third highest item in the stack to the top of the stack
	DUP4,				///< copies the 4th highest item in the stack to the top of the stack
	DUP5,				///< copies the 5th highest item in the stack to the top of the stack
	DUP6,				///< copies the 6th highest item in the stack to the top of the stack
	DUP7,				///< copies the 7th highest item in the stack to the top of the stack
	DUP8,				///< copies the 8th highest item in the stack to the top of the stack
	DUP9,				///< copies the 9th highest item in the stack to the top of the stack
	DUP10,				///< copies the 10th highest item in the stack to the top of the stack
	DUP11,				///< copies the 11th highest item in the stack to the top of the stack
	DUP12,				///< copies the 12th highest item in the stack to the top of the stack
	DUP13,				///< copies the 13th highest item in the stack to the top of the stack
	DUP14,				///< copies the 14th highest item in the stack to the top of the stack
	DUP15,				///< copies the 15th highest item in the stack to the top of the stack
	DUP16,				///< copies the 16th highest item in the stack to the top of the stack

	SWAP1 = 0x90,		///< swaps the highest and second highest value on the stack
	SWAP2,				///< swaps the highest and third highest value on the stack
	SWAP3,				///< swaps the highest and 4th highest value on the stack
	SWAP4,				///< swaps the highest and 5th highest value on the stack
	SWAP5,				///< swaps the highest and 6th highest value on the stack
	SWAP6,				///< swaps the highest and 7th highest value on the stack
	SWAP7,				///< swaps the highest and 8th highest value on the stack
	SWAP8,				///< swaps the highest and 9th highest value on the stack
	SWAP9,				///< swaps the highest and 10th highest value on the stack
	SWAP10,				///< swaps the highest and 11th highest value on the stack
	SWAP11,				///< swaps the highest and 12th highest value on the stack
	SWAP12,				///< swaps the highest and 13th highest value on the stack
	SWAP13,				///< swaps the highest and 14th highest value on the stack
	SWAP14,				///< swaps the highest and 15th highest value on the stack
	SWAP15,				///< swaps the highest and 16th highest value on the stack
	SWAP16,				///< swaps the highest and 17th highest value on the stack

	LOG0 = 0xa0,		///< Makes a log entry; no topics.
	LOG1,				///< Makes a log entry; 1 topic.
	LOG2,				///< Makes a log entry; 2 topics.
	LOG3,				///< Makes a log entry; 3 topics.
	LOG4,				///< Makes a log entry; 4 topics.

	CREATE = 0xf0,		///< create a new account with associated code
	CALL,				///< message-call into an account
	CALLCODE,			///< message-call with another account's code only
	RETURN,				///< halt execution returning output data
	SUICIDE = 0xff		///< halt execution and register account for later deletion
};

int main(int argc, char *argv[])
{

    InstructionMap.insert(pair<string,uint8_t> (("STOP"),uint8_t(Instruction::STOP)));
    InstructionMap.insert(pair<string,uint8_t> (("ADD"),uint8_t(Instruction::ADD)));
    InstructionMap.insert(pair<string,uint8_t> (("MUL"),uint8_t(Instruction::MUL)));
    InstructionMap.insert(pair<string,uint8_t> (("SUB"),uint8_t(Instruction::SUB)));
    InstructionMap.insert(pair<string,uint8_t> (("DIV"),uint8_t(Instruction::DIV)));
    InstructionMap.insert(pair<string,uint8_t> (("SDIV"),uint8_t(Instruction::SDIV)));
    InstructionMap.insert(pair<string,uint8_t> (("MOD"),uint8_t(Instruction::MOD)));
    InstructionMap.insert(pair<string,uint8_t> (("ADDMOD"),uint8_t(Instruction::ADDMOD)));
    InstructionMap.insert(pair<string,uint8_t> (("MULMOD"),uint8_t(Instruction::MULMOD)));
    InstructionMap.insert(pair<string,uint8_t> (("EXP"),uint8_t(Instruction::EXP)));
    InstructionMap.insert(pair<string,uint8_t> (("SIGNEXTEND"),uint8_t(Instruction::SIGNEXTEND)));
    InstructionMap.insert(pair<string,uint8_t> (("LT"),uint8_t(Instruction::LT)));
    InstructionMap.insert(pair<string,uint8_t> (("GT"),uint8_t(Instruction::GT)));
    InstructionMap.insert(pair<string,uint8_t> (("SLT"),uint8_t(Instruction::SLT)));
    InstructionMap.insert(pair<string,uint8_t> (("SGT"),uint8_t(Instruction::SGT)));
    InstructionMap.insert(pair<string,uint8_t> (("EQ"),uint8_t(Instruction::EQ)));
    InstructionMap.insert(pair<string,uint8_t> (("ISZERO"),uint8_t(Instruction::ISZERO)));
    InstructionMap.insert(pair<string,uint8_t> (("AND"),uint8_t(Instruction::AND)));
    InstructionMap.insert(pair<string,uint8_t> (("OR"),uint8_t(Instruction::OR)));
    InstructionMap.insert(pair<string,uint8_t> (("XOR"),uint8_t(Instruction::XOR)));
    InstructionMap.insert(pair<string,uint8_t> (("NOT"),uint8_t(Instruction::NOT)));
    InstructionMap.insert(pair<string,uint8_t> (("BYTE"),uint8_t(Instruction::BYTE)));
    InstructionMap.insert(pair<string,uint8_t> (("SHA3"),uint8_t(Instruction::SHA3)));
    InstructionMap.insert(pair<string,uint8_t> (("ADDRESS"),uint8_t(Instruction::ADDRESS)));
    InstructionMap.insert(pair<string,uint8_t> (("BALANCE"),uint8_t(Instruction::BALANCE)));
    InstructionMap.insert(pair<string,uint8_t> (("ORIGIN"),uint8_t(Instruction::ORIGIN)));
    InstructionMap.insert(pair<string,uint8_t> (("CALLER"),uint8_t(Instruction::CALLER)));
    InstructionMap.insert(pair<string,uint8_t> (("CALLVALUE"),uint8_t(Instruction::CALLVALUE)));
    InstructionMap.insert(pair<string,uint8_t> (("CALLDATALOAD"),uint8_t(Instruction::CALLDATALOAD)));
    InstructionMap.insert(pair<string,uint8_t> (("CALLDATACOPY"),uint8_t(Instruction::CALLDATACOPY)));
    InstructionMap.insert(pair<string,uint8_t> (("CODESIZE"),uint8_t(Instruction::CODESIZE)));
    InstructionMap.insert(pair<string,uint8_t> (("CODECOPY"),uint8_t(Instruction::CODECOPY)));
    InstructionMap.insert(pair<string,uint8_t> (("GASPRICE"),uint8_t(Instruction::GASPRICE)));
    InstructionMap.insert(pair<string,uint8_t> (("EXTCODESIZE"),uint8_t(Instruction::EXTCODESIZE)));
    InstructionMap.insert(pair<string,uint8_t> (("EXTCODECOPY"),uint8_t(Instruction::EXTCODECOPY)));
    InstructionMap.insert(pair<string,uint8_t> (("BLOCKHASH"),uint8_t(Instruction::BLOCKHASH)));
    InstructionMap.insert(pair<string,uint8_t> (("COINBASE"),uint8_t(Instruction::COINBASE)));
    InstructionMap.insert(pair<string,uint8_t> (("TIMESTAMP"),uint8_t(Instruction::TIMESTAMP)));
    InstructionMap.insert(pair<string,uint8_t> (("NUMBER"),uint8_t(Instruction::NUMBER)));
    InstructionMap.insert(pair<string,uint8_t> (("DIFFICULTY"),uint8_t(Instruction::DIFFICULTY)));
    InstructionMap.insert(pair<string,uint8_t> (("GASLIMIT"),uint8_t(Instruction::GASLIMIT)));
    InstructionMap.insert(pair<string,uint8_t> (("POP"),uint8_t(Instruction::POP)));
    InstructionMap.insert(pair<string,uint8_t> (("MLOAD"),uint8_t(Instruction::MLOAD)));
    InstructionMap.insert(pair<string,uint8_t> (("MSTORE"),uint8_t(Instruction::MSTORE)));
    InstructionMap.insert(pair<string,uint8_t> (("MSTORE8"),uint8_t(Instruction::MSTORE8)));
    InstructionMap.insert(pair<string,uint8_t> (("SLOAD"),uint8_t(Instruction::SLOAD)));
    InstructionMap.insert(pair<string,uint8_t> (("SSTORE"),uint8_t(Instruction::SSTORE)));
    InstructionMap.insert(pair<string,uint8_t> (("JUMP"),uint8_t(Instruction::JUMP)));
    InstructionMap.insert(pair<string,uint8_t> (("JUMPI"),uint8_t(Instruction::JUMPI)));
    InstructionMap.insert(pair<string,uint8_t> (("PC"),uint8_t(Instruction::PC)));
    InstructionMap.insert(pair<string,uint8_t> (("MSIZE"),uint8_t(Instruction::MSIZE)));
    InstructionMap.insert(pair<string,uint8_t> (("GAS"),uint8_t(Instruction::GAS)));
    InstructionMap.insert(pair<string,uint8_t> (("JUMPDEST"),uint8_t(Instruction::JUMPDEST)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH1"),uint8_t(Instruction::PUSH1)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH2"),uint8_t(Instruction::PUSH2)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH3"),uint8_t(Instruction::PUSH3)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH4"),uint8_t(Instruction::PUSH4)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH5"),uint8_t(Instruction::PUSH5)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH6"),uint8_t(Instruction::PUSH6)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH7"),uint8_t(Instruction::PUSH7)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH8"),uint8_t(Instruction::PUSH8)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH9"),uint8_t(Instruction::PUSH9)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH10"),uint8_t(Instruction::PUSH10)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH11"),uint8_t(Instruction::PUSH11)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH12"),uint8_t(Instruction::PUSH12)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH13"),uint8_t(Instruction::PUSH13)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH14"),uint8_t(Instruction::PUSH14)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH15"),uint8_t(Instruction::PUSH15)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH16"),uint8_t(Instruction::PUSH16)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH17"),uint8_t(Instruction::PUSH17)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH18"),uint8_t(Instruction::PUSH18)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH19"),uint8_t(Instruction::PUSH19)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH20"),uint8_t(Instruction::PUSH20)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH21"),uint8_t(Instruction::PUSH21)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH22"),uint8_t(Instruction::PUSH22)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH23"),uint8_t(Instruction::PUSH23)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH24"),uint8_t(Instruction::PUSH24)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH25"),uint8_t(Instruction::PUSH25)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH26"),uint8_t(Instruction::PUSH26)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH27"),uint8_t(Instruction::PUSH27)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH28"),uint8_t(Instruction::PUSH28)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH29"),uint8_t(Instruction::PUSH29)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH30"),uint8_t(Instruction::PUSH30)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH31"),uint8_t(Instruction::PUSH31)));
    InstructionMap.insert(pair<string,uint8_t> (("PUSH32"),uint8_t(Instruction::PUSH32)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP1"),uint8_t(Instruction::DUP1)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP2"),uint8_t(Instruction::DUP2)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP3"),uint8_t(Instruction::DUP3)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP4"),uint8_t(Instruction::DUP4)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP5"),uint8_t(Instruction::DUP5)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP6"),uint8_t(Instruction::DUP6)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP7"),uint8_t(Instruction::DUP7)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP8"),uint8_t(Instruction::DUP8)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP9"),uint8_t(Instruction::DUP9)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP10"),uint8_t(Instruction::DUP10)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP11"),uint8_t(Instruction::DUP11)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP12"),uint8_t(Instruction::DUP12)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP13"),uint8_t(Instruction::DUP13)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP14"),uint8_t(Instruction::DUP14)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP15"),uint8_t(Instruction::DUP15)));
    InstructionMap.insert(pair<string,uint8_t> (("DUP16"),uint8_t(Instruction::DUP16)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP1"),uint8_t(Instruction::SWAP1)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP2"),uint8_t(Instruction::SWAP2)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP3"),uint8_t(Instruction::SWAP3)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP4"),uint8_t(Instruction::SWAP4)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP5"),uint8_t(Instruction::SWAP5)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP6"),uint8_t(Instruction::SWAP6)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP7"),uint8_t(Instruction::SWAP7)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP8"),uint8_t(Instruction::SWAP8)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP9"),uint8_t(Instruction::SWAP9)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP10"),uint8_t(Instruction::SWAP10)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP11"),uint8_t(Instruction::SWAP11)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP12"),uint8_t(Instruction::SWAP12)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP13"),uint8_t(Instruction::SWAP13)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP14"),uint8_t(Instruction::SWAP14)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP15"),uint8_t(Instruction::SWAP15)));
    InstructionMap.insert(pair<string,uint8_t> (("SWAP16"),uint8_t(Instruction::SWAP16)));
    InstructionMap.insert(pair<string,uint8_t> (("LOG0"),uint8_t(Instruction::LOG0)));
    InstructionMap.insert(pair<string,uint8_t> (("LOG1"),uint8_t(Instruction::LOG1)));
    InstructionMap.insert(pair<string,uint8_t> (("LOG2"),uint8_t(Instruction::LOG2)));
    InstructionMap.insert(pair<string,uint8_t> (("LOG3"),uint8_t(Instruction::LOG3)));
    InstructionMap.insert(pair<string,uint8_t> (("LOG4"),uint8_t(Instruction::LOG4)));
    InstructionMap.insert(pair<string,uint8_t> (("CREATE"),uint8_t(Instruction::CREATE)));
    InstructionMap.insert(pair<string,uint8_t> (("CALL"),uint8_t(Instruction::CALL)));
    InstructionMap.insert(pair<string,uint8_t> (("CALLCODE"),uint8_t(Instruction::CALLCODE)));
    InstructionMap.insert(pair<string,uint8_t> (("RETURN"),uint8_t(Instruction::RETURN)));
    InstructionMap.insert(pair<string,uint8_t> (("SUICIDE"),uint8_t(Instruction::SUICIDE)));

    if (argv[1]==nullptr)
    {
        printf("please input filename\n");
        exit(-1);
    }
    FILE *f = fopen(argv[1],"r");
    if (f==nullptr)
    {
        printf("unable to open file %s\n",argv[1]);
        exit(-1);
    }
    FILE *fAsm=fopen(strcat(argv[1],".binary"),"w");
    if (f==nullptr)
    {
        printf("unable to open file %s\n",strcat(argv[1],".binary"));
        exit(-1);
    }
    unsigned int j=0;
    char opcode[15];
    char operand[90];
    char comments[4096];
    for (unsigned i=0;fscanf(f,"%s",opcode)!=EOF;i++)
    {
        string opcodestr(opcode);
        if (opcodestr.find("PUSH")!=opcodestr.npos)
        {
                fscanf(f,"%s",operand);
                string operandstr(operand);
                if (IsHex(operandstr))
                {
                    int numDigit=operandstr.length()-2;
                    int numBytes=numDigit%2?(numDigit+1)/2:numDigit/2;

                    fprintf(fAsm,"%.02x",InstructionMap.find("PUSH1")->second+numBytes-1);
                    if (numDigit%2)
                        fprintf(fAsm,"0");
                    for(unsigned int k=2;k<=operandstr.length()-1;k++)
                        fprintf(fAsm,"%c",operandstr[k]);
                    j+=1+numBytes;
                }
                else if (operandstr.find(":")!=opcodestr.npos)
                {
                    std::string hexstr;
                    std::stringstream stream;

                    if (Jumpdest.find(operandstr)==Jumpdest.end())
                    {
                        printf ("can't find jump tag or future tag (not yet supported), line %d", i+1);
                        exit(-1);
                    }
                    stream <<  std::hex << j-Jumpdest.find(operandstr)->second;
                    hexstr = stream.str();
                    stream.str("");
                    stream  << std::setfill ('0') << std::setw(hexstr.length()%2?hexstr.length()+1:hexstr.length()) << hexstr;
                    hexstr = stream.str();
                    fprintf(fAsm,"%.02x%.02x%s%.02x%.02x",InstructionMap.find("PC")->second,InstructionMap.find("PUSH1")->second+hexstr.length()/2-1,hexstr.c_str(),InstructionMap.find("SWAP1")->second,InstructionMap.find("SUB")->second);
                    //printf ("%x\n",Jumpdest.find(operandstr)->second);
                    j+=4+hexstr.length()/2;

                }
                else{

                    printf ("illegal operand, line %d,",i+1);
                    printf("operand = %s",operand);
                    exit(-1);

                }

        }
        else if (opcodestr.find("JUMPDEST")!=opcodestr.npos)
        {
            fscanf (f,"%s",operand);
            string operandstr(operand);
            if (Jumpdest.find(operandstr)!=Jumpdest.end())
            {
                printf("duplicate jump tag, line %d",i+1);
                exit(-1);
            }
            fprintf(fAsm,"%.02x",InstructionMap.find(opcodestr)->second);
            Jumpdest.insert(pair<string,unsigned int>(operandstr,j));
            j+=1;
        }
        else{
            if (InstructionMap.find(opcodestr)==InstructionMap.end())
            {
                printf("invalid opcode, line %d",i+1);
                exit(-1);
            }
            fprintf(fAsm,"%.02x",InstructionMap.find(opcodestr)->second);
            j+=1;
        }
        fscanf(f,"%[^\n]",comments);
        //printf("%s\n",opcode);
    }
    printf("%x bytes written\n",j);

    return 0;
}

