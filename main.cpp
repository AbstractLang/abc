#include <iostream>

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace legacy;

Module* makeLLVMModule() {
	Module* mod = new Module("test", getGlobalContext());
	Contant* c = mod->getOrInsertFunction("mul_add",
			IntegerType::get(getGlobalContext(), 32),
			IntegerType::get(getGlobalContext(), 32),
			IntegerType::get(getGlobalContext(), 32),
			IntegerType::get(getGlobalContext(), 32),
			NULL);
	Function* mul_add = cast<Function>(c);
	mul_add->setCallingConv(CallingConv::C);

	Function::arg_iterator args = mul_add->arg_begin();
	Value* x = args++;
	x->setName("x");
	Value* y = args++;
	y->setName("y");
	Value* z = args++;
	z->setName("z");

	BasicBlock* block = BasicBlock::Create(getGlobalContext(), "entry", mul_add);
	IRBuilder<> builder(block);

	Value* tmp = builder.CreateBinOp(Instruction::Mul,
			x, y, "tmp");
	Value* tmp2 = builder.CreateBinOp(Instruction::Add,
			tmp, z, "tmp2");
	Builder.CreateRet(tmp2);

	return mod;
}

int main(void) {
	Module* mod = makeLLVMModule();
	
	verifyModule(*mod, PrintMessageAction);

	PassManager passManager;
	passManager.add(createPrintModulePass(&outs()));
	passManager.run(*mod);

	delete mod;
	return 0;
}

