// day_01.swift
// AdventOfCode
//
// Solution for: https://adventofcode.com/2023/day/1
//
// Copyright (c) 2026 Paul Godavari. All rights reserved.


func calibrationValue(for line: Substring) -> Int64? {
    let isDigit: (Character) -> Bool = { ("0"..."9").contains($0) }

    guard let first = line.first(where: isDigit)?.wholeNumberValue,
          let last = line.last(where: isDigit)?.wholeNumberValue else {
        return nil
    }
    
    return Int64(first * 10 + last)
}


func day01Part1_2023(input: String, useExample: Bool) -> SwiftSolveResult {
    var sum: Int64 = 0
    
    for line in input.split(whereSeparator: \.isNewline) {
        if let value = calibrationValue(for: line) {
            sum += value
        } else {
            return .unfinished
        }
    }
        
    return .answer(sum)
}


func day01Part2_2023(input: String, useExample: Bool) -> SwiftSolveResult {
    // Replace this with .answer(total), or return .invalidInput when the input cannot be solved.
    .unfinished
}


