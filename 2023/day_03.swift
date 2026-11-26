// day_03.swift
// AdventOfCode
//
// Solution for: https://adventofcode.com/2023/day/3
//
// Copyright (c) 2026 Paul Godavari. All rights reserved.


struct Position {
    var row: Int = 0
    var col: Int = 0
}


struct Number {
    var position = Position()
    var length: Int = 0
    
    func toInt64(from grid: Grid) -> Int64 {
        var accumulator: Int64 = 0
        for index in 0..<length {
            let c = grid[position.row][position.col + index]
            accumulator = accumulator * 10 + Int64(c.wholeNumberValue!)
        }
        return accumulator
    }
}


struct Grid {
    var data: [[Character]]

    var maxRow: Int {
        data.count - 1
    }

    var maxCol: Int {
        data[0].count - 1
    }
    
    init(_ input: String) {
        data = input.split(whereSeparator: \.isNewline).map { Array($0) }
    }
    
    subscript(row: Int) -> [Character] {
        get { data[row] }
        set { data[row] = newValue }
    }
    
    // The number value is 0 if a number is not adjacent to a symbol,
    // or the real value of the number if it is adjacent to a symbol.
    func numberValue(for number: Number) -> Int64 {
        let startRow = max(0, number.position.row - 1)
        let startCol = max(0, number.position.col - 1)
        let stopRow = min(maxRow, number.position.row + 1)
        let stopCol = min(maxCol, number.position.col + number.length)
        
        for rowIndex in startRow...stopRow {
            for colIndex in startCol...stopCol {
                let c = self[rowIndex][colIndex]
                if !c.isNumber && c != "." {
                    return number.toInt64(from: self)
                }
            }
        }
        
        return 0
    }
}


func day03Part1_2023(input: String, useExample: Bool) -> SwiftSolveResult {
    let grid = Grid(input)
    
    var numbers = [Number]()
    
    for rowIndex in 0...grid.maxRow {
        var parsingNumber = false
        var number = Number()
        for colIndex in 0...grid.maxCol {
            let c = grid[rowIndex][colIndex]
            if c.isNumber {
                number.length += 1
                if parsingNumber {
                    continue
                } else {
                    parsingNumber = true
                    number.position = Position(row: rowIndex, col: colIndex)
                }
            } else {
                if parsingNumber {
                    parsingNumber = false
                    numbers.append(number)
                    number.length = 0
                }
            }
        }
        if parsingNumber {
            numbers.append(number)
            number.length = 0
        }
    }
    
    var sum: Int64 = 0
    for number in numbers {
        sum += grid.numberValue(for: number)
    }
    
    return .answer(sum)
}


func day03Part2_2023(input: String, useExample: Bool) -> SwiftSolveResult {
    return .unfinished
}
