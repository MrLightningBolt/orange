#include <gen/BreakStatement.h>
#include <gen/ForLoop.h>
#include <gen/generator.h>

std::string BreakStatement::string() {
	return "BREAK";
}

void BreakStatement::resolve() {
	if (resolved)
		return;
	resolved = true; 

	// nothing to resolve for a break statement
}

Value* BreakStatement::Codegen() {
	ForLoop *loop = CG::Symtab()->getLoop();
	if (loop == nullptr) {
		std::cerr << "error: cannot call break while not in a loop.\n";
		exit(1);
	}

	return CG::Builder.CreateBr(loop->getContinue());
}


