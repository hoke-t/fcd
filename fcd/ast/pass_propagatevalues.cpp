//
// pass_propagatevalues.cpp
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

#include "pass_propagatevalues.h"

using namespace llvm;
using namespace std;

void AstPropagateValues::attemptToPropagateUses(VariableUses &uses)
{
	llvm_unreachable("implement me");
}

AstPropagateValues::AstPropagateValues(AstVariableUses& uses)
: useAnalysis(uses)
{
}

void AstPropagateValues::doRun(FunctionNode &fn)
{
	auto end = useAnalysis.end();
	for (auto iter = useAnalysis.begin(); iter != end; ++iter)
	{
		auto& use = useAnalysis.getUseInfo(iter);
		attemptToPropagateUses(use);
	}
}

const char* AstPropagateValues::getName() const
{
	return "Propagate values";
}