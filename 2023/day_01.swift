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


let numbers = [
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
]


func calibration2Value(for line: Substring) -> Int64? {
    let isDigit: (Character) -> Bool = { ("0"..."9").contains($0) }

    var first: Int?
    var last: Int?
    var calibrationValue: Int64?
    
    for index in line.indices {
        let character = line[index]
        if isDigit(character) {
            first = character.wholeNumberValue!
        } else {
            let remaining = line[index...]
            for numIndex in numbers.indices {
                if remaining.hasPrefix(numbers[numIndex]) {
                    first = numIndex
                    break
                }
            }
        }
        if first != nil {
            break
        }
    }

    for index in line.indices.reversed() {
        let character = line[index]
        if isDigit(character) {
            last = character.wholeNumberValue!
        } else {
            let remaining = line[index...]
            for numIndex in numbers.indices {
                if remaining.hasPrefix(numbers[numIndex]) {
                    last = numIndex
                    break
                }
            }
        }
        if last != nil {
            break
        }
    }
    
    if let first, let last {
        calibrationValue = Int64(first * 10 + last)
    }

    return calibrationValue
}


func day01Part2_2023(input: String, useExample: Bool) -> SwiftSolveResult {
    var sum: Int64 = 0
    
    for line in input.split(whereSeparator: \.isNewline) {
        if let value = calibration2Value(for: line) {
            sum += value
        } else {
            return .unfinished
        }
    }
        
    return .answer(sum)
}


