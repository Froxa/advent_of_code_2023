# Advent of Code 2023

## Day 10: Pipe Maze
Pipe Maze is a 2d grid filled with tiles:
- `|` is a vertical pipe connecting north and south.
- `-` is a horizontal pipe connecting east and west.
- `L` is a 90-degree bend connecting north and east.
- `J` is a 90-degree bend connecting north and west.
- `7` is a 90-degree bend connecting south and west.
- `F` is a 90-degree bend connecting south and east.
- `.` is ground; there is no pipe in this tile.
- `S` is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape the pipe has.

There is one large, continuous pipe loop starting and ending at `S`.

(Full specification is available at https://adventofcode.com/2023/day/10.)

### Part 1
The objective of part 1 is to find the single giant loop starting at S and count how many steps along the loop does it take to get from the starting position to the point farthest from the starting position.

The main idea of the proposed solution is to find the first pipe that is connected to `S` and start walking along the loop. For each pipe segment its connections are calculated and compared against the position of the previous pipe segment. Position of the next pipe segment is equal to the position that is different from the previous segment. This process is repeated until `S` is reached. The number of steps it takes to get from `S` to the farthest point is equal to the length of the loop divided by 2.

### Part 2
The objective of part 2 is to find the number of tiles in the area enclosed by the loop.

The proposed solution is inspired by the scanline fill algorithm used to fill polygons and works as follows. First, the minimum bounding box for the main loop is calculated. Next, for each row find all intersections with the main loop and determine which intersections are entry or exit points. Variable `inside` is used to track whether the current tile is inside or outside. There are 5 possible patterns that can occur and are solved by following these rules:
- `|` the value of `inside` is flipped.
- `F7` the value of `inside` doesn't change.
- `LJ` the value of `inside` doesn't change.
- `FJ` the value of `inside` is flipped.
- `L7` the value of `inside` is flipped.

`-` tiles have no impact on the `inside` variable. Finally, count every tile which is visited while `inside == true` and which is not part of the main loop. 

## Day 13: Point of Incidence
Point of Incidence deals with lava islands with mirrors. Lava island is a 2d grid filled with ash (`.`) and rocks (`#`).

(Full specification is available at https://adventofcode.com/2023/day/13.)

### Part 1
The objective of part 1 is to find the position of a line of mirrors that are placed between two rows or columns.

The proposed solution iterates over all possible placements of the mirrors and checks whether they reflect correctly or not. There are (width-1) * (height-1) possible placements. For each placement at most (width * height) tiles need to be checked to determine if the placement is valid or not. The time complexity of the proposed algorithm is O(N^2) where N is the total number of tiles. However, in practice many of the possible placements can be disregarded after just a few comparisons.   

### Part 2
Part 2 is the same as part 1 except now we assume that exactly one `.` or `#` should be the opposite type.

The proposed algorithm is the same except this time the mirror placement is considered valid if there is exactly 1 mismatch between a tile and its reflection.

