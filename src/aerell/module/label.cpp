// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/module/label.hpp"

namespace aerell
{

Inst& Label::getInst() { return this->inst; }

const Inst& Label::getInst() const { return this->inst; }

void Label::setPub(bool val) { this->pub = val; }

} // namespace aerell