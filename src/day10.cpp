#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

using Maze = std::vector<std::string>;

/**
 * Checks if a character is a valid maze tile.
 *
 * @param c The character to be checked.
 * @return True if the character is a valid maze tile, false otherwise.
 */
bool IsValidTile(const char c) {
    switch (c) {
        case '|':
        case '-':
        case 'L':
        case 'J':
        case '7':
        case 'F':
        case '.':
        case 'S':
            return true;
        default:
            return false;
    }
}

/**
 * Loads a maze from an input file.
 *
 * @param in_file Text file containing the maze.
 * @return The maze if it is valid.
 * @throws std::runtime_error Thrown if the file could't be opened or if the maze is not valid.
 */
Maze LoadMaze(const std::string &in_file) {
    Maze maze;

    std::ifstream file(in_file);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + in_file);
    }

    size_t width = 0;

    std::string row;
    while (std::getline(file, row)) {
        // every tile must be valid
        if (!std::all_of(row.begin(), row.end(), IsValidTile)) {
            throw std::runtime_error("Invalid tile found");
        }

        // rows must have equal length
        if (maze.empty()) {
            width = row.size();
        } else if (row.size() != width) {
            throw std::runtime_error("Row length mismatch");
        }

        // row is valid -> add to maze
        maze.push_back(row);
    }

    file.close();

    return maze;
}

/** TODO description
 *
 */
unsigned Solve(const Maze &maze) {
    return 0;
}

int main() {
    try {
        const auto maze = LoadMaze("input/day10.txt");
        const auto result = Solve(maze);
        std::cout << result << std::endl;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
