/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <llvm/Support/raw_ostream.h>

#include "aerell/support/ostream.h"

namespace aerell
{

OWStream::OWStream(WriteFunc writer) : writer(writer) {}

void OWStream::write(std::string_view str) { this->write_impl(str.data(), str.size()); }

void OWStream::write_impl(const char* ptr, size_t size)
{
    this->writer({ptr, size});
    this->totalBytes += size;
}

uint64_t OWStream::current_pos() const { return this->totalBytes; }

OSStream::OSStream(std::string& str) : str(str) {}

void OSStream::write(std::string_view str) { this->write_impl(str.data(), str.size()); }

void OSStream::write_impl(const char* ptr, size_t size) { this->str.append(ptr, size); }

uint64_t OSStream::current_pos() const { return static_cast<uint64_t>(this->str.size()); }

OStream& outs()
{
    static OWStream inst([](std::string_view str) { llvm::outs() << str; });
    return inst;
}

OStream& errs()
{
    static OWStream inst([](std::string_view str) { llvm::errs() << str; });
    return inst;
}

} // namespace aerell