// code ref : http://laure.gonnord.org/pro/research/ER03_2015/lab3_intro.pdf
#define DEBUG_TYPE "opCountMapper"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include<string>

using namespace llvm;

int retType(std::string op_code_str)
{
      
      if(op_code_str=="ret")return 0;
      if(op_code_str=="br")return 0;
      if(op_code_str=="switch")return 0;
      if(op_code_str=="indirectbr")return 0;
      if(op_code_str=="invoke")return 0;
      if(op_code_str=="resume")return 0;
      if(op_code_str=="unreachable")return 0;
      if(op_code_str=="cleanupret")return 0;
      if(op_code_str=="catchret")return 0;
      if(op_code_str=="catchpad")return 0;
      if(op_code_str=="catchswitch")return 0;
      if(op_code_str=="add") return 1;
      if(op_code_str=="fadd") return 1;
      if(op_code_str=="sub") return 1;
      if(op_code_str=="fsub") return 1;
      if(op_code_str=="mul") return 1;
      if(op_code_str=="fmul") return 1;
      if(op_code_str=="udiv") return 1;
      if(op_code_str=="sdiv") return 1;
      if(op_code_str=="fdiv") return 1;
      if(op_code_str=="urem") return 1;
      if(op_code_str=="srem") return 1;
      if(op_code_str=="frem") return 2;
      if(op_code_str=="and") return 2;
      if(op_code_str=="xor") return 2;
      if(op_code_str=="alloca")  return 3;
      if(op_code_str=="load")  return 3;
      if(op_code_str=="store")  return 3;
      if(op_code_str=="cmpxchg")  return 3;
      if(op_code_str=="atomicrmw")  return 3;
      if(op_code_str=="fence")  return 3;
      if(op_code_str=="getelementptr") return 3;
      if(op_code_str=="trunc")   return 4;
      if(op_code_str=="zext")   return 4;
      if(op_code_str=="sext")   return 4;
      if(op_code_str=="fptrunc")   return 4;
      if(op_code_str=="fpext")   return 4;
      if(op_code_str=="fptoui")   return 4;
      if(op_code_str=="fptosi")   return 4;
      if(op_code_str=="uitofp")   return 4;
      if(op_code_str=="sitofp")   return 4;
      if(op_code_str=="inttoptr")   return 4;
      if(op_code_str=="ptrtoint")   return 4;
      if(op_code_str=="bitcast")   return 4;
      if(op_code_str=="addrspacecast")   return 4;
      if(op_code_str=="icmp")  return 5;
      if(op_code_str=="fcmp")return 5;
      if(op_code_str=="phi")return 5;
      if(op_code_str=="select")return 5;
      if(op_code_str=="call")return 5;
      if(op_code_str=="shl")return 5;
      if(op_code_str=="lshr")return 5;
      if(op_code_str=="ashr")return 5;
      if(op_code_str=="va_arg")return 5;
      if(op_code_str=="extractelement")return 5;
      if(op_code_str=="insertelement")return 5;
      if(op_code_str=="shufflevector")return 5;
      if(op_code_str=="extractvalue")return 5;
      if(op_code_str=="insertvalue")return 5;
      if(op_code_str=="landingpad")return 5;
      if(op_code_str=="cleanuppad")return 5;
      return 6;
  
}

namespace 
{
	struct CountOpcodes : public FunctionPass 
	{
		std::map<std::string, int> opCountMapper;
		std::map<int, int> opCountMapperCount;

		static char ID;

		CountOpcodes() : FunctionPass(ID) {}
		
		virtual bool runOnFunction(Function &F) {

		opCountMapperCount[0]=opCountMapperCount[1]=opCountMapperCount[2]=opCountMapperCount[3]=opCountMapperCount[4]=opCountMapperCount[5]=opCountMapperCount[6]=0;
		
		errs() << "function_name >> " << F.getName() << '\n';
		
		for (Function::iterator bb = F.begin(); bb != F.end(); ++bb) 
		{
			
			for (BasicBlock::iterator i = bb->begin(); i != bb->end(); ++i) 
			{

				opCountMapperCount[retType(i->getOpcodeName())]+=1;

				if(opCountMapper.find(i->getOpcodeName()) == opCountMapper.end()) 
				{
					opCountMapper[i->getOpcodeName()] = 1;
				} else 
				{
					opCountMapper[i->getOpcodeName()] += 1;
				}
		    }
		}

		std::map <std::string, int>::iterator i = opCountMapper.begin();
		for(;i!=opCountMapper.end();i++)
		{
			errs() << i->first << " : " << i->second << "\n";
		}

		errs()<<"#######################\n\n";
		errs()<<"Branch Ops: "<<opCountMapperCount[0]<<"\n";
		errs()<<"Arithmetic Ops: "<<opCountMapperCount[1]<<"\n";
		errs()<<"Logical Ops : "<<opCountMapperCount[2]<<"\n";
		errs()<<"Memory Ops: "<<opCountMapperCount[3]<<"\n";
		errs()<<"Convert Ops: "<<opCountMapperCount[4]<<"\n";
		errs()<<"Other Ops : "<<opCountMapperCount[5]<<"\n";
		errs()<<"#######################\n\n";

		
		errs() << "--------End of Function Call--------\n";

		opCountMapper.clear();
		opCountMapperCount.clear();
		return false;

		}
	};
}

char CountOpcodes::ID = 0;
static RegisterPass<CountOpcodes> X("opCountMapper", "counts branching memory and arithmetic ins per cpp file ");
