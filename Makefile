CONFIGURATION ?= Release
DERIVED_DATA := $(CURDIR)/build/xcode
PROGRAM := $(DERIVED_DATA)/Build/Products/$(CONFIGURATION)/aoc

.PHONY: all build test

all: build

build:
	xcodebuild -project AdventOfCode.xcodeproj -scheme AdventOfCode -configuration "$(CONFIGURATION)" -derivedDataPath "$(DERIVED_DATA)" build
	cp "$(PROGRAM)" "$(CURDIR)/aoc"

test: build
	python3 tests/test_cli.py --binary "$(CURDIR)/aoc"
