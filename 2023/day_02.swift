// day_02.swift
// AdventOfCode
//
// Solution for: https://adventofcode.com/2023/day/2
//
// Copyright (c) 2026 Paul Godavari. All rights reserved.


struct Draw {
    let red: Int
    let green: Int
    let blue: Int
    
    func isPossibleFor(_ draw: Draw) -> Bool {
        red <= draw.red && green <= draw.green && blue <= draw.blue
    }
    
    static let configPart1 = Draw(red: 12, green: 13, blue: 14)
}


extension Draw {
    init?(_ line: Substring) {
        var red: Int = 0
        var green: Int = 0
        var blue: Int = 0
        
        for component in line.split(separator: ",") {
            let values = component.split(whereSeparator: \.isWhitespace)
            guard values.count == 2, let num = Int(values[0]), num >= 0 else {
                return nil
            }
            
            switch values[1] {
                case "blue": blue = num
                case "red": red = num
                case "green": green = num
                default: return nil
            }
        }
        
        self.init(red: red, green: green, blue: blue)
    }
}


struct Game {
    let id: Int
    let draws: [Draw]
    
    func isPossibleFor(configuration: Draw) -> Bool {
        draws.allSatisfy { $0.isPossibleFor(configuration) }
    }
    
    init?(_ line: Substring) {
        let gameParts = line.split(separator: ":")
        guard let gameLabel = gameParts.first?.split(separator: " "),
              gameLabel.count >= 2,
              let gameID = Int(gameLabel[1]) else {
            return nil
        }
        
        guard let drawParts = gameParts.last?.split(separator: ";") else {
            return nil
        }
        
        var parsedDraws = [Draw]()
        
        for drawPart in drawParts {
            guard let draw = Draw(drawPart) else {
                return nil
            }
            parsedDraws.append(draw)
        }
        
        self.id = gameID
        self.draws = parsedDraws
    }
}


func day02Part1_2023(input: String, useExample: Bool) -> SwiftSolveResult {
    var sum: Int64 = 0
    
    for gameLine in input.split(whereSeparator: \.isNewline) {
        guard let game = Game(gameLine) else {
            return .invalidInput
        }
        
        if game.isPossibleFor(configuration: Draw.configPart1) {
            sum += Int64(game.id)
        }
    }
    
    return .answer(sum)
}


func day02Part2_2023(input: String, useExample: Bool) -> SwiftSolveResult {
    return .unfinished
}
