/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level 
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file 
** may not be copied, modified, or distributed except according to those terms.
*/ 

#include <orange/CastingEngine.h>
#include <orange/generator.h>

bool CastingEngine::AreTypesCompatible(OrangeTy* a, OrangeTy* b) {
	if (a == nullptr || b == nullptr) return false;

	if (a->string() == b->string()) return true; 
	if (a->isIntegerTy() && b->isIntegerTy()) return true;
	if (a->isIntegerTy() && b->isFloatingPointTy()) return true;  
	if (b->isIntegerTy() && a->isFloatingPointTy()) return true;  
	if (a->isFloatingPointTy() && b->isFloatingPointTy()) return true;
	if (a->isIntegerTy() && b->isPointerTy()) return true; 
	if (b->isIntegerTy() && a->isPointerTy()) return true;
	return false;
}

bool CastingEngine::CanTypeBeCasted(OrangeTy* src, OrangeTy* dest) {
	// For now, return the order-independent AreTypesCompatible
	return AreTypesCompatible(src, dest);
}

bool CastingEngine::CastValueToType(Value** v, OrangeTy* t, bool isSigned, bool force) {
	if (v == nullptr || t == nullptr) return false; 

	OrangeTy* srcType = OrangeTy::getFromLLVM((*v)->getType(), false);
	Type* llvmT = t->getLLVMType();

	// If we're not forcing, and we can't cast v to t, just return.
	if (force == false && CanTypeBeCasted(srcType, t) == false) return false;

	// If we're not forcing, and v has a higher precedence than t, return.
	if (force == false && ShouldTypeMorph(srcType, t) == false) return false; 

	// Here we're weeded out all unnecessary or impossible morphing; you don't have 
	// to check for force == true here anymore, so just morph anything that's possible.
	
	// Cast v to the integer type of t; this will account for bitwidths. 
	if (t->isIntegerTy() && srcType->isIntegerTy()) {
		*v = GE::builder()->CreateIntCast(*v, llvmT, isSigned);
		return true;
	}

	if (t->isFloatingPointTy() && srcType->isFloatingPointTy()) {
		*v = GE::builder()->CreateFPCast(*v, llvmT);
		return true;
	}

	if (t->isFloatingPointTy() && srcType->isIntegerTy()) {
		*v = isSigned ? GE::builder()->CreateSIToFP(*v, llvmT) : GE::builder()->CreateUIToFP(*v, llvmT);
		return true; 
	}

	if (t->isIntegerTy() && srcType->isFloatingPointTy()) {
		*v = isSigned ? GE::builder()->CreateFPToSI(*v, llvmT) : GE::builder()->CreateFPToUI(*v, llvmT);
		return true; 
	}

	if (t->isPointerTy() && srcType->isPointerTy()) {
		*v = GE::builder()->CreateBitCast(*v, llvmT);
		return true; 
	}
	
	if (srcType->isPointerTy() && t->isIntegerTy()) {
		*v = GE::builder()->CreatePtrToInt(*v, llvmT);
		return true; 
	}

	if (srcType->isIntegerTy() && t->isPointerTy()) {
		*v = GE::builder()->CreateIntToPtr(*v, llvmT); 
		return true;
	}

	if (t->isPointerTy() && srcType->isArrayTy()) {
		*v = GE::builder()->CreateBitCast(*v, llvmT);
		return true;
	}

	if (t->isArrayTy() && srcType->isArrayTy()) {
		// Do no casting.
		return false; 
	}

	throw std::runtime_error("could not determine type to cast.");
	return false;
}

bool CastingEngine::CastValuesToFit(Value** v1, Value** v2, bool isV1Signed, bool isV2Signed) {
	if (v1 == nullptr || v2 == nullptr) return false; 

	OrangeTy* type1 = OrangeTy::getFromLLVM((*v1)->getType(), isV1Signed);
	OrangeTy* type2 = OrangeTy::getFromLLVM((*v2)->getType(), isV2Signed);

	if (AreTypesCompatible(type1, type2) == false) return false;

	bool retVal = false;

	if (ShouldTypeMorph(type1, type2)) {
		// type2 > type1, so cast type1. 
		retVal = CastValueToType(v1, type2, isV1Signed, false);
	} else {
		// type2 < type1, so cast type2.
		retVal = CastValueToType(v2, type1, isV2Signed, false);
	}

	return retVal;
}

bool CastingEngine::ShouldTypeMorph(OrangeTy* src, OrangeTy* dest) {
	if (dest->isFloatingPointTy() && src->isIntegerTy()) return true; 
	if (dest->isDoubleTy() && src->isFloatTy()) return true;

	if (src->isIntegerTy() && dest->isIntegerTy()) {
		return dest->getIntegerBitWidth() > src->getIntegerBitWidth();
	}

	return false;
}

OrangeTy* CastingEngine::GetFittingType(OrangeTy* v1, OrangeTy* v2) {
	// If v1 and v2 are the same, always prefer the LHS.
	if (v1 == v2) return v1;
	if (AreTypesCompatible(v1, v2) == false) return nullptr;
	return ShouldTypeMorph(v1, v2) ? v2 : v1;
}