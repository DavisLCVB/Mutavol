#ifndef POSITION_HPP
#define POSITION_HPP

/**
 * @file position.hpp
 * @author Davis Cartagena
 * @brief Struct to store a position of a character in a file
 */

// Mutavol namespace
namespace mtv {
    /**
     * @struct Position
     * @brief Struct to store a position of a character in a file
     */
    struct Position {
        // row of the character
        unsigned long long row{0};
        // column of the character
        unsigned long long column{0};

        /**
         * @brief Default constructor, initializes the row and column to 0
         */
        Position() = default;

        /**
         * @brief Constructor that initializes the row and column to the given values
         * @param row The row of the character
         * @param column The column of the character
         */
        Position(const unsigned long long row,
                 const unsigned long long column)
            : row(row),
              column(column) {}
    };
} // namespace mtv

#endif //POSITION_HPP
