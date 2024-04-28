#include "files.hpp"

namespace mtv {
File::File(const std::string &file) : file(file) {}

void File::set_file(const std::string &file) { this->file = file; }

void File::write(const std::string &data) {
    std::ofstream out(file);
    out << data;
    out.close();
}

void File::read() {
    std::wifstream in(file);
    if(!in.is_open()) {
        throw std::runtime_error("Error opening file.");
    }
    tokens = std::vector<wchar_t>(std::istreambuf_iterator<wchar_t>(in),
                                  std::istreambuf_iterator<wchar_t>());
    in.close();
}

std::vector<wchar_t> File::get_tokens() const { return tokens; }

} // namespace mtv