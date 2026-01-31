// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <llvm-c/Target.h>

#include "aerell/llvm/llvm_init.h"

void llvm_init() { LLVMInitializeNativeTarget(); }