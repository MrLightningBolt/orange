/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level 
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file 
** may not be copied, modified, or distributed except according to those terms.
*/ 

#include <orange/CommaStmt.h>
#include <orange/generator.h>

Value* CommaStmt::Codegen() {
	for (auto expr : m_exprs) expr->Codegen();
	return nullptr;
}

ASTNode* CommaStmt::clone() {
	std::vector<Expression*> clonedExprs;
	for (auto expr : m_exprs) clonedExprs.push_back((Expression*)expr->clone());
	return new CommaStmt(clonedExprs);
}

std::string CommaStmt::string() {
	std::stringstream ss; 

	for (unsigned int i = 0; i < m_exprs.size(); i++) {
		ss << m_exprs[i]->string(); 
		if (i + 1 < m_exprs.size()) ss << " , ";
	}

	return ss.str();
}

CommaStmt::CommaStmt(std::vector<Expression*> expressions) {
	m_exprs = expressions;

	for (unsigned int i = 0; i < m_exprs.size(); i++) {
		auto e = m_exprs[i];

		std::stringstream ss;
		ss << "expr" << i;

		addChild(ss.str(), e);
	}
}
