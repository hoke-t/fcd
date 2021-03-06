//
// pass_targetinfo.h
// Copyright (C) 2015 Félix Cloutier.
// All Rights Reserved.
//
// This file is part of fcd.
// 
// fcd is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// fcd is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with fcd.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef pass_targetinfo_cpp
#define pass_targetinfo_cpp

#include "llvm_warnings.h"

SILENCE_LLVM_WARNINGS_BEGIN()
#include <llvm/Pass.h>
#include <llvm/IR/Instructions.h>
SILENCE_LLVM_WARNINGS_END()

#include <algorithm>
#include <string>
#include <vector>

struct TargetRegisterInfo
{
	size_t offset;
	size_t size;
	llvm::SmallVector<unsigned, 4> gepOffsets;
	std::string name;
	
	static inline bool offset_less_size_more(const TargetRegisterInfo& a, const TargetRegisterInfo& b)
	{
		if (a.offset < b.offset)
		{
			return true;
		}
		if (a.offset == b.offset && a.size > b.size)
		{
			return true;
		}
		return false;
	}
};

class TargetInfo : public llvm::ImmutablePass
{
	std::string name;
	size_t spIndex;
	const std::vector<TargetRegisterInfo>* targetRegInfo;
	const llvm::DataLayout* dl;
	
public:
	static char ID;
	
	inline TargetInfo() : ImmutablePass(ID)
	{
		spIndex = ~0;
	}
	
	virtual bool doInitialization(llvm::Module& m) override;
	
	inline const std::vector<TargetRegisterInfo>& targetRegisterInfo() const
	{
		assert(targetRegInfo != nullptr);
		return *targetRegInfo;
	}
	
	inline void setTargetRegisterInfo(const std::vector<TargetRegisterInfo>& targetRegInfo)
	{
		this->targetRegInfo = &targetRegInfo;
	}
	
	inline void setStackPointer(const TargetRegisterInfo& targetReg)
	{
		for (size_t i = 0; i < targetRegisterInfo().size(); i++)
		{
			const auto& thisReg = targetRegisterInfo()[i];
			if (targetReg.offset == thisReg.offset && targetReg.size == thisReg.size)
			{
				spIndex = i;
				break;
			}
		}
	}
	
	inline const TargetRegisterInfo* getStackPointer() const
	{
		if (spIndex < targetRegisterInfo().size())
		{
			return &targetRegisterInfo()[spIndex];
		}
		return nullptr;
	}
	
	inline std::string& targetName()
	{
		return name;
	}
	
	inline const std::string& targetName() const
	{
		return name;
	}
	
	llvm::GetElementPtrInst* getRegister(llvm::Value* registerStruct, const char* name) const;
	
	const char* registerName(const llvm::Value& value) const;
	const char* registerName(const llvm::GetElementPtrInst& gep) const;
	const char* registerName(size_t offset, size_t size) const;
	const char* largestOverlappingRegister(const char* overlapped) const;
	
	const char* keyName(const char* name) const;
};

namespace llvm
{
	void initializeTargetInfoPass(PassRegistry& PM);
}

#endif /* pass_targetinfo_cpp */
