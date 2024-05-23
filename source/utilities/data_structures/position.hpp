#ifndef POSITION_HPP
#define POSITION_HPP

namespace mtv {
    struct Position {
        unsigned long long row{0};
        unsigned long long column{0};

        Position() = default;

        Position(const unsigned long long row,
                 const unsigned long long column)
            : row(row),
              column(column) {}
    };
} // namespace mtv

#endif //POSITION_HPP
