#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <optional>

/**
* 2D lava island with mirrors.
 */
using Island = std::vector<std::string>;

/**
* Used to specify mirror orientation.
 */
enum Direction { ROW, COL };


/**
* Loads an lava island from an input file.
 *
 * @param in_file Text file containing the lava island.
 * @return Vector of loaded islands.
 * @throws std::runtime_error Thrown if the file could't be opened.
 */
std::vector<Island> LoadIslands(const std::string &in_file) {
    std::vector<Island> islands;

    std::ifstream file(in_file);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + in_file);
    }

    Island curr_island;

    std::string row;
    while (std::getline(file, row)) {
        if (row.empty()) {
            islands.push_back(curr_island);
            curr_island = Island();
        } else {
            curr_island.push_back(row);
        }
    }

    file.close();

    return islands;
}

/**
 *
 * @param island
 * @param direction Check ROW or COLUMN.
 * @param idx
 * @param mirror
 * @return
 */
bool IsMirrored(const Island &island, const Direction direction, const size_t idx, const int mirror) {
    const int l_start = mirror;
    const int l_end = 0;
    const int r_start = mirror + 1;
    const int r_end = direction == ROW ? island[0].size() - 1 : island.size() - 1;

    // check palindrome
    for (int l = l_start, r = r_start; l >= l_end && r <= r_end; --l, ++r) {
        if (direction == ROW) {
            // row
            if (island[idx][l] != island[idx][r]) { return false; }
        } else {
            // column
            if (island[l][idx] != island[r][idx]) { return false; }
        }
    }

    return true;
}


int main() {
    auto islands = LoadIslands("input/day13.txt");

    for (size_t r = 0; r < islands[0].size(); r++) {
        std::cout << IsMirrored(islands[0], ROW, r, 5) << std::endl;
    }


    return 0;
}
