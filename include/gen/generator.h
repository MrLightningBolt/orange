#ifndef __GEN_GENERATOR_H__
#define __GEN_GENERATOR_H__

#include "AST.h"
#include "Block.h"
#include "Symtab.h"
#include <llvm/Support/CodeGen.h>
#include <stack>

class CodeGenerator {
private:
	static void GenerateObject();
public:
	static std::string outputFile;
	static std::string fileBase;
	static bool outputAssembly;
	static bool verboseOutput;
	static bool doLink;
	static bool bits32;
	static std::string tripleName;
	static llvm::CodeModel::Model model;

	static Module *TheModule;
	static IRBuilder<> Builder;
	static FunctionPassManager *TheFPM;
	
	static std::stack<SymTable *> Symtabs;

	static SymTable* Symtab() { return Symtabs.top(); }
	
	static void init();
	static void Generate(Block *globalBlock);
};

typedef CodeGenerator CG;

#endif 