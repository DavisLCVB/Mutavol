#include "scanner.hpp"

namespace mtv {
wchar_t Scanner::scan() {
    if (index < size) {
        return buffer[index++];
    }
    return 0;
}
} // namespace mtv