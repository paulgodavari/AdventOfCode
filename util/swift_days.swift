// swift_days.swift
// AdventOfCode
//
// Copyright (c) 2026 Paul Godavari. All rights reserved.


public enum SwiftSolveResult {
    case answer(Int64)
    case unfinished
    case invalidInput
}


public struct SwiftPart: Sendable {
    public let year: Int32
    public let day: Int32
    public let part: Int32
}


private struct SwiftPartRegistration: Sendable {
    let part: SwiftPart
    let solve: @Sendable (String, Bool) -> SwiftSolveResult
}


private let swiftParts: [SwiftPartRegistration] = [
    SwiftPartRegistration(part: SwiftPart(year: 2023, day: 1, part: 1), solve: day01Part1_2023)
]


public func swiftPartCount() -> Int {
    swiftParts.count
}


public func swiftPart(at index: Int) -> SwiftPart {
    swiftParts[index].part
}


// Reads exactly inputCount UTF-8 bytes and calls the selected Swift solver. The input pointer is
// borrowed for this call and is not retained by Swift.

public func solveSwiftPart(year: Int32, day: Int32, part: Int32, input: UnsafePointer<UInt8>, inputCount: Int, useExample: Bool) -> SwiftSolveResult {
    guard let implementation = swiftParts.first(where: { $0.part.year == year && $0.part.day == day && $0.part.part == part }) else {
        return .invalidInput
    }

    let bytes = UnsafeBufferPointer(start: input, count: inputCount)
    let text = String(decoding: bytes, as: UTF8.self)
    return implementation.solve(text, useExample)
}
