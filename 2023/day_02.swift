// day_02.swift
// AdventOfCode
//
// Solution for: https://adventofcode.com/2023/day/2
//
// Copyright (c) 2026 Paul Godavari. All rights reserved.


struct Game {
    struct Draw {
        var red: Int = 0
        var green: Int = 0
        var blue: Int = 0
        
        func isPossibleFor(_ draw: Draw) -> Bool {
            red <= draw.red && green <= draw.green && blue <= draw.blue
        }
    }
    
    let id: Int
    var draws: [Draw] = []
    
    func isPossibleFor(configuration: Draw) -> Bool {
        draws.allSatisfy { $0.isPossibleFor(configuration) }
    }
}


private let config = Game.Draw(red: 12, green: 13, blue: 14)


func day02Part1_2023(input: String, useExample: Bool) -> SwiftSolveResult {
    var sum: Int64 = 0
    
    for gameLine in input.split(whereSeparator: \.isNewline) {
        let gameParts = gameLine.split(separator: ":", maxSplits: 1)
        guard let gameLabel = gameParts.first?.split(separator: " "),
              gameLabel.count >= 2,
              let gameID = Int(gameLabel[1]) else {
            return .invalidInput
        }
        
        guard let drawParts = gameParts.last?.split(separator: ";") else {
            return .invalidInput
        }
        
        var game = Game(id: gameID)
        
        for drawPart in drawParts {
            var draw = Game.Draw()
            
            let components = drawPart.split(separator: ",")
            for component in components {
                let values = component.split(separator: " ")
                guard values.count == 2, let num = Int(values[0]) else {
                    return .invalidInput
                }
                let label = values[1]
                switch label {
                    case "blue": draw.blue = num
                    case "red": draw.red = num
                    case "green": draw.green = num
                    default: return .invalidInput
                }
            }
            game.draws.append(draw)
        }
        
        if game.isPossibleFor(configuration: config) {
            sum += Int64(game.id)
        }
    }
    
    return .answer(sum)
}


func day02Part2_2023(input: String, useExample: Bool) -> SwiftSolveResult {
    return .unfinished
}
