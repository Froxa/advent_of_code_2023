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

    islands.push_back(curr_island); // last island
    file.close();

    return islands;
}

/**
 * Checks if row or column is mirrored.
 *
 * @param island
 * @param direction Check ROW or COLUMN.
 * @param idx
 * @param mirror
 * @return
 */
unsigned IsMirrored(const Island &island, const Direction direction, const size_t idx, const int mirror) {
    const int l_start = mirror;
    const int l_end = 0;
    const int r_start = mirror + 1;
    const int r_end = direction == ROW ? island[0].size() - 1 : island.size() - 1;

    unsigned smudges = 0;

    // check if row/column is mirrored and count smudges
    for (int l = l_start, r = r_start; l >= l_end && r <= r_end; --l, ++r) {
        if (direction == ROW) {
            // row
            if (island[idx][l] != island[idx][r]) { smudges += 1; }
        } else {
            // column
            if (island[l][idx] != island[r][idx]) { smudges += 1; }
        }
    }

    return smudges;
}

unsigned Solve(const std::vector<Island> &islands, const unsigned desired_smudges) {
    unsigned sum = 0;

    // find mirrors for all the islands
    for (const Island &island_i: islands) {
        const size_t rows = island_i.size();
        const size_t cols = island_i[0].size();

        // try both directions
        for (const auto dir: {ROW, COL}) {
            const int mirror_max = dir == ROW ? cols - 2 : rows - 2;
            const size_t i_max = dir == ROW ? rows - 1 : cols - 1;

            // try all mirrors
            unsigned smudges = 0;
            for (int mirror = 0; mirror <= mirror_max; ++mirror) {
                smudges = 0;

                // check all rows/columns
                for (int i = 0; i <= i_max; ++i) {
                    smudges += IsMirrored(island_i, dir, i, mirror);
                    if (smudges > desired_smudges) { break; }
                }
                // all rows/columns checked
                if (smudges == desired_smudges) {
                    // solution (mirror, dir) found
                    sum += dir == ROW ? mirror + 1 : (mirror + 1) * 100;
                    break;
                }
            }
            if (smudges == desired_smudges) { break; }
        }
    }

    return sum;
}

unsigned SolvePart1(const std::vector<Island> &islands) {
    return Solve(islands, 0);
}

unsigned SolvePart2(const std::vector<Island> &islands) {
    return Solve(islands, 1);
}


int main() {
    const std::string input = "input/day13.txt";
    try {
        const auto islands = LoadIslands(input);
        const auto result1 = SolvePart1(islands);
        const auto result2 = SolvePart2(islands);
        std::cout << "'" << input << "' part1=" << result1 << " part2=" << result2 << std::endl;
    } catch (const std::runtime_error &e) {
        std::cerr << "Error running '" << input << "':" << std::endl
                << e.what() << std::endl;
        return 1;
    }
    return 0;
}
