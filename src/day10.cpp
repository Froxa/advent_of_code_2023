#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <optional>

/**
 * 2D grid.
 */
using Maze = std::vector<std::string>;

/**
 * 2D coordinate.
 */
class Vec2 {
public:
    int x, y;

    Vec2(const int x, const int y)
        : x(x), y(y) {}

    Vec2() = default;

    bool operator==(const Vec2 &other) const {
        return x == other.x && y == other.y;
    }
};

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

/**
 * Finds the starting tile 'S' in a maze.
 *
 * @param maze The maze.
 * @return Coordinates of the starting tile or (-1,-1) if not found.
 */
Vec2 FindS(const Maze &maze) {
    const size_t height = maze.size();
    const size_t width = maze[0].size();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (maze[y][x] == 'S') {
                return {x, y};
            }
        }
    }

    return {-1, -1}; // unreachable for valid maze
}

/**
 * Calculates the coordinates of two tiles that are connected through a pipe.
 *
 * @param pipe Type of the pipe.
 * @param pos Coordinate of the pipe.
 * @return The two coordinates or nothing if the pipe type is '.' or 'S'.
 */
std::optional<std::pair<Vec2, Vec2> > PipeConnections(const char pipe, const Vec2 &pos) {
    switch (pipe) {
        case '|':
            return {{Vec2(pos.x, pos.y + 1), Vec2(pos.x, pos.y - 1)}};
        case '-':
            return {{Vec2(pos.x - 1, pos.y), Vec2(pos.x + 1, pos.y)}};
        case 'L':
            return {{Vec2(pos.x, pos.y - 1), Vec2(pos.x + 1, pos.y)}};
        case 'J':
            return {{Vec2(pos.x, pos.y - 1), Vec2(pos.x - 1, pos.y)}};
        case '7':
            return {{Vec2(pos.x, pos.y + 1), Vec2(pos.x - 1, pos.y)}};
        case 'F':
            return {{Vec2(pos.x, pos.y + 1), Vec2(pos.x + 1, pos.y)}};
        default:
            return {};
    }
}

/**
 * Maze bounds checking.
 *
 * @param maze The maze.
 * @param pos The coordinate.
 * @return True if the coordinate is within the maze, and false otherwise.
 */
bool IsPositionInMaze(const Maze &maze, const Vec2 &pos) {
    return pos.x >= 0 && pos.x < maze[0].size() && pos.y >= 0 && pos.y < maze.size();
}

/**
 * Checks if a pipe is connected to a coordinate.
 *
 * @param maze The maze.
 * @param pipe_pos Coordinate of the pipe.
 * @param pos The coordinate.
 * @return True if the pipe is connected to the coordinate, and false otherwise.
 */
bool IsConnectedTo(const Maze &maze, const Vec2 &pipe_pos, const Vec2 &pos) {
    const auto conns = PipeConnections(maze[pipe_pos.y][pipe_pos.x], pipe_pos);

    if (!conns.has_value()) {
        return false;
    }

    return conns->first == pos || conns->second == pos;
}

/**
 * Finds the first segment of the main loop. There are exactly 2 pipes connected to the starting symbol
 * so only 2 (out of 4 possible) random adjecent tiles need to be checked.
 *
 * @param maze The maze.
 * @param s_pos Coordinate of the starting symbol.
 * @return The coordinate of the first segment of the main loop.
 */
Vec2 FindLoopStart(const Maze &maze, const Vec2 &s_pos) {
    const Vec2 left{s_pos.x - 1, s_pos.y};
    if (IsPositionInMaze(maze, left) && IsConnectedTo(maze, left, s_pos)) {
        return left;
    }

    const Vec2 right{s_pos.x + 1, s_pos.y};
    if (IsPositionInMaze(maze, right) && IsConnectedTo(maze, right, s_pos)) {
        return right;
    }

    // main loop pipes are above and below S
    return {s_pos.x, s_pos.y - 1}; // above
}

/**
 * Solves the "Advent of Code 2023 day 10 part 1" problem. Calculates how many steps along the main loop
 * it takes to get from the starting position to the point farthest from the starting position.
 *
 * @param maze Instance of the day 10 problem.
 * @return The number of steps from the starting position to the farthest position.
 */
unsigned SolvePart1(const Maze &maze) {
    // find S
    const auto s_pos = FindS(maze);

    unsigned len = 0;

    auto prev = s_pos;
    auto curr = FindLoopStart(maze, s_pos);

    // walk along the main loop and measure its length
    while (true) {
        auto conns = PipeConnections(maze[curr.y][curr.x], curr);

        if (!conns.has_value()) {
            // looped back to 'S' (or reached '.' which shouldn't happen)
            break;
        }

        // which way is forward?
        if (conns->first == prev) {
            prev = curr;
            curr = conns->second;
        } else {
            prev = curr;
            curr = conns->first;
        }

        len += 1;
    }

    // calculate distance of the farthest point
    return len / 2 + 1;
}

/**
 * Calculates the number of tiles that are enclosed by the main loop.
 *
 * @param maze Original maze.
 * @param maze_hl Copy of maze but main loop tiles are replaced with '#'.
 * @param top_left Top left corner of a bounding rectangle containing the main loop.
 * @param bot_right Bottom right corner of a bounding rectangle containing the main loop.
 * @return The number of enclosed tiles.
 */
unsigned CountEnclosed(const Maze &maze, const Maze &maze_hl, const Vec2 &top_left, const Vec2 &bot_right) {
    unsigned enclosed = 0;

    // count enclosed tiles
    for (int y = top_left.y; y <= bot_right.y; ++y) {
        bool inside = false;
        char last_turn = 'X';

        for (int x = top_left.x; x <= bot_right.x; ++x) {
            char tile = maze[y][x];

            // main loop?
            if (maze_hl[y][x] == '#') {
                switch (tile) {
                    case '|':
                        { inside ^= true; }
                        break;
                    case 'L':
                        last_turn = 'L';
                        break;
                    case 'F':
                        last_turn = 'F';
                        break;
                    case 'J':
                        if (last_turn == 'F') { inside ^= true; }
                        break;
                    case '7':
                        if (last_turn == 'L') { inside ^= true; }
                        break;
                    default:
                        break;
                }
            } else if (inside) {
                enclosed += 1;
            }
        }
    }

    return enclosed;
}

/**
 * Solves the "Advent of Code 2023 day 10 part 2" problem. Calculates how many tiles are withing the area
 * that is enclosed by the main loop.
 *
 * @param maze Instance of the day 10 problem.
 * @return The number of steps from the starting position to the farthest position.
 */
unsigned SolvePart2(Maze &maze) {
    // copy of the maze used to highlight the main loop
    Maze maze_hl(maze);

    // find S
    const auto s_pos = FindS(maze_hl);

    // bounding rectange coordinates
    Vec2 top_left = s_pos;
    Vec2 bot_right = s_pos;

    Vec2 prev = s_pos;
    Vec2 curr = FindLoopStart(maze_hl, s_pos);

    Vec2 start = curr;
    Vec2 end;

    // walk along the main loop, highlight it and find the convex cover
    while (true) {
        auto conns = PipeConnections(maze_hl[curr.y][curr.x], curr);

        if (!conns.has_value()) {
            // looped back to 'S' (or reached '.' which shouldn't happen)
            maze_hl[curr.y][curr.x] = '#';
            end = prev;
            break;
        }

        // highlight the main loop
        maze_hl[curr.y][curr.x] = '#';

        // resize bounding rectangle
        top_left.x = std::min(top_left.x, curr.x);
        top_left.y = std::min(top_left.y, curr.y);
        bot_right.x = std::max(bot_right.x, curr.x);
        bot_right.y = std::max(bot_right.y, curr.y);

        // which way is forward?
        if (conns->first == prev) {
            prev = curr;
            curr = conns->second;
        } else {
            prev = curr;
            curr = conns->first;
        }
    }

    // replace 'S' with a pipe
    for (const char pipe_type: {'-', '|', 'F', '7', 'L', 'J'}) {
        const auto conns = PipeConnections(pipe_type, s_pos); // will always have value
        if ((conns->first == start && conns->second == end) || (conns->first == end && conns->second == start)) {
            maze[s_pos.y][s_pos.x] = pipe_type;
            break;
        }
    }

    return CountEnclosed(maze, maze_hl, top_left, bot_right);
}

int main() {
    for (auto input: {"input/day10_test1.txt", "input/day10_test2.txt", "input/day10_test3.txt", "input/day10.txt"}) {
        try {
            auto maze = LoadMaze(input);
            const auto result1 = SolvePart1(maze);
            const auto result2 = SolvePart2(maze);
            std::cout << "'" << input << "' part1=" << result1 << " part2=" << result2 << std::endl;
        } catch (const std::runtime_error &e) {
            std::cerr << "Error running '" << input << "':" << std::endl
                    << e.what() << std::endl;
            return 1;
        }
    }

    return 0;
}
