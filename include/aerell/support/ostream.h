/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_SUPPORT_OSTREAM_H
#define AERELL_SUPPORT_OSTREAM_H

#include <llvm/Support/raw_ostream.h>
#include <string>

#include <llvm/Support/FormatVariadic.h>
#include <llvm/Support/Error.h>

namespace aerell
{

class OStream : public llvm::raw_ostream
{
  public:
    virtual ~OStream() = default;
    virtual void write(std::string_view str) = 0;
};

class OWStream final : public OStream
{
  private:
    OWStream(const OWStream&) = delete;

    using WriteFunc = void (*)(std::string_view str);
    WriteFunc writer;
    uint64_t totalBytes = 0;

    void write_impl(const char* ptr, size_t size) override;
    uint64_t current_pos() const override;

  public:
    OWStream(WriteFunc writer);
    void write(std::string_view str) override;
};

class OSStream final : public OStream
{
  public:
    OSStream(std::string& str);

    void write(std::string_view str) override;

  private:
    OSStream(const OSStream&) = delete;

    std::string& str;
    void write_impl(const char* ptr, size_t size) override;
    uint64_t current_pos() const override;
};

OStream& outs();

OStream& errs();

// Helper numeric data type
template <typename T> inline void printNumeric(OStream& os, const char* format, T val)
{
    char buf[32];
    int len = std::snprintf(buf, sizeof(buf), format, val);
    if(len > 0) os.write(std::string_view(buf, len));
}

// Integer
inline OStream& operator<<(OStream& os, int v)
{
    printNumeric(os, "%d", v);
    return os;
}
inline OStream& operator<<(OStream& os, unsigned int v)
{
    printNumeric(os, "%u", v);
    return os;
}
inline OStream& operator<<(OStream& os, long v)
{
    printNumeric(os, "%ld", v);
    return os;
}
inline OStream& operator<<(OStream& os, unsigned long v)
{
    printNumeric(os, "%lu", v);
    return os;
}
inline OStream& operator<<(OStream& os, long long v)
{
    printNumeric(os, "%lld", v);
    return os;
}
inline OStream& operator<<(OStream& os, unsigned long long v)
{
    printNumeric(os, "%llu", v);
    return os;
}

// Floating point
inline OStream& operator<<(OStream& os, float v)
{
    printNumeric(os, "%g", (double)v);
    return os;
}
inline OStream& operator<<(OStream& os, double v)
{
    printNumeric(os, "%g", v);
    return os;
}

// Boolean
inline OStream& operator<<(OStream& os, bool v)
{
    os.write(v ? "true" : "false");
    return os;
}

// Char
inline OStream& operator<<(OStream& os, char c)
{
    os.write(std::string_view(&c, 1));
    return os;
}

// Char*
inline OStream& operator<<(OStream& os, const char* str)
{
    os.write(str);
    return os;
}

// Pointer
inline OStream& operator<<(OStream& os, const void* ptr)
{
    if(!ptr)
    {
        os.write("null");
        return os;
    }
    char buf[24];
    int len = std::snprintf(buf, sizeof(buf), "0x%llx", (unsigned long long)(uintptr_t)ptr);
    os.write(std::string_view(buf, len));
    return os;
}

template <typename T> inline OStream& operator<<(OStream& os, T* ptr) { return os << static_cast<const void*>(ptr); }

// String
inline OStream& operator<<(OStream& os, const std::string& s)
{
    os.write(s);
    return os;
}

// String view
inline OStream& operator<<(OStream& os, std::string_view sv)
{
    os.write(sv);
    return os;
}

// LLVM
inline OStream& operator<<(OStream& os, const llvm::Error& error)
{
    llvm::SmallString<128> buf;
    llvm::raw_svector_ostream rsos(buf);
    rsos << error;
    os.write(rsos.str());
    return os;
}

} // namespace aerell

#endif