// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <cassert>
#include <format>

#include "aerell/module/module.hpp"

namespace aerell
{

Module::Module() : name("unknown") {}

void Module::setName(std::string_view name) { this->name = name; }

std::string Module::toStr() const
{
    std::string str = std::format("# name: {}\n\n", this->name);

    for(const auto& [name, label] : this->labels)
    {
        str += std::format("{}:\n", name);
        const auto& inst = label.getInst();
        for(std::uint32_t i = 0; i < label.getInst().size(); i++)
        {
            auto kind = inst.getKind(i);
            switch(kind)
            {
            case InstKind::PRINT: str += std::format(" {} {}\n", aerell::toStr(kind), inst.getOperand(i)); break;
            }
        }
    }

    return str;
}

Label& Module::addLabel(std::string_view name, bool pub)
{
    auto& label = this->labels.try_emplace(std::string{name}).first->second;
    label.setPub(pub);
    return label;
}

const Label& Module::getLabel(std::string_view name)
{
    assert(this->labels.contains(name));
    return this->labels.find(name)->second;
}

} // namespace aerell