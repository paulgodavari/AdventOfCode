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


private let numberWords = [
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


func digitValue(at index: String.Index, in line: Substring) -> Int? {
    let character = line[index]
    if ("0"..."9").contains(character) {
        return character.wholeNumberValue
    }
    
    let remaining = line[index...]
    return numberWords.firstIndex { remaining.hasPrefix($0) }
}


func calibration2Value(for line: Substring) -> Int64? {
    var first: Int?
    for index in line.indices {
        if let value = digitValue(at: index, in: line) {
            first = value
            break
        }
    }

    var last: Int?
    for index in line.indices.reversed() {
        if let value = digitValue(at: index, in: line) {
            last = value
            break
        }
    }
    
    if let first, let last {
        return Int64(first * 10 + last)
    }

    return nil
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


