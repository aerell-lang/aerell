#include <format>
#include <fstream>

#include "aerell/compiler/source/source.h"

namespace Aerell
{

Source::Source(const char* path) { this->path = std::filesystem::weakly_canonical(path); }

bool Source::exist()
{
    if(!std::filesystem::exists(this->path))
    {
        llvm::errs() << "'" << this->path.c_str() << "' does not exist.\n";
        return false;
    }

    if(std::filesystem::is_directory(this->path))
    {
        llvm::errs() << "'" << this->path.c_str() << "' is not a file.\n";
        return false;
    }

    return true;
}

bool Source::read()
{
    if(!this->content.empty()) this->content.clear();
    if(!this->lineStarts.empty()) this->lineStarts.clear();

    std::ifstream file(this->path, std::ios::binary);
    if(!file.is_open())
    {
        llvm::errs() << "Failed to open '" << this->path.c_str() << "'.\n";
        return false;
    }

    std::vector<unsigned char> fileBytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    this->content = {fileBytes.begin(), fileBytes.end()};

    lineStarts.push_back(0);
    for(size_t i = 0; i < this->content.size(); ++i)
        if(this->content[i] == '\n') lineStarts.push_back(i + 1);

    return true;
}

const std::string& Source::getContent() { return this->content; }

std::string Source::getPath() const { return this->path.generic_string(); }

void Source::printErrorMessage(size_t offset, size_t size, const char* msg)
{
    // Find line and column from offset
    auto it = std::upper_bound(lineStarts.begin(), lineStarts.end(), offset);

    size_t line = (it - lineStarts.begin());
    size_t lineStart = lineStarts[line - 1];
    size_t column = offset - lineStart + 1;

    // Prepare print message
    auto path = std::format("{}:{}:{}", this->getPath(), line, column);
    auto prefix = std::format(" {}| ", line);
    size_t end = (line < lineStarts.size()) ? lineStarts[line] : content.size();
    auto view = std::string_view{content.data() + lineStart, end - lineStart};
    if(!view.empty() && view.back() == '\n') view.remove_suffix(1);

    // Print Message
    llvm::errs() << std::string(path.size(), '-') << '\n';
    llvm::errs() << path << "\n\n";
    llvm::errs() << prefix << view << "\n";
    llvm::errs() << std::string(prefix.size() + (column - 1), ' ') << std::string(size, '^') << '\n';
    llvm::errs() << ' ' << msg << '\n';
}

} // namespace Aerell