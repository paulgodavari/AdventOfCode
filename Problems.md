# Problems

Keep the original puzzle statement in `AdventOfCodeData/YYYY/DD/problem.md`, beside the input and answers. Before coding, read the story and
work out what the input represents, what result to calculate, and what limits the solution must handle.

Add one row after solving each part. Use standard names in the **Algorithm** column so you can find other parts that use the same approach.
Record the clue that pointed to the algorithm and the technique you want to remember. For Part 2, note what changed and how it affected your
first approach.

| Puzzle | Part | What to notice | Algorithm | Technique | Solution |
| --- | ---: | --- | --- | --- | --- |
| [2023 Day 14] | 1 | Fixed rocks split columns into segments. | Grid scan | Count gaps to move rocks in one pass. | [C++][day14] |
| [2023 Day 14] | 2 | A billion rounds of the same four tilts. | Cycle detection | Save each grid's first cycle. Skip full periods. | [C++][day14] |

[2023 Day 14]: https://adventofcode.com/2023/day/14
[day14]: 2023/day_14.cpp
