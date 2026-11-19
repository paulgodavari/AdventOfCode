import argparse
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


class CommandLineTests(unittest.TestCase):
    binary: Path

    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        self.data = self.root / "AdventOfCodeData"
        self.day = self.data / "2023" / "01"
        self.day.mkdir(parents=True)
        (self.day / "input.txt").write_text("1abc2\n")
        (self.day / "test_input1.txt").write_text("1abc2\npqr3stu8vwx\na1b2c3d4e5f\ntreb7uchet\n")
        (self.day / "test_input2.txt").write_text("two1nine\neightwothree\nabcone2threexyz\nxtwone3four\n"
                                                "4nineeightseven2\nzoneight234\n7pqrstsixteen\n")
        (self.day / "answers.txt").write_text("142 12\n281 12\n")

    def run_aoc(self, *arguments):
        return subprocess.run([str(self.binary), *arguments], cwd=self.root, capture_output=True, text=True, timeout=10)

    def run_part(self, *arguments):
        return self.run_aoc("--year", "2023", "--day", "1", "--part", "1", *arguments)

    def test_no_language_runs_both_implementations(self):
        result = self.run_part()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stderr, "")
        lines = result.stdout.splitlines()
        self.assertEqual(len(lines), 2)
        self.assertRegex(lines[0], r"^2023 Day 1 part 1 \(C\+\+\): 12 \d+\.\d{3} ms$")
        self.assertRegex(lines[1], r"^2023 Day 1 part 1 \(Swift\): UNFINISHED \d+\.\d{3} ms$")

    def test_language_filters_work_without_date_filters(self):
        result = self.run_aoc("--lang", "swift")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(len(result.stdout.splitlines()), 1)
        self.assertIn("(Swift): UNFINISHED", result.stdout)
        self.assertNotIn("(C++)", result.stdout)

    def test_cpp_language_filter(self):
        result = self.run_part("--lang", "c++")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(len(result.stdout.splitlines()), 1)
        self.assertIn("(C++): 12", result.stdout)

    def test_missing_swift_parts_are_skipped(self):
        result = self.run_aoc("--year", "2023", "--day", "1")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(len(result.stdout.splitlines()), 3)
        self.assertIn("part 2 (C++): 12", result.stdout)
        self.assertNotIn("part 2 (Swift)", result.stdout)

    def test_part_specific_test_inputs(self):
        result = self.run_aoc("--year", "2023", "--day", "1", "--test", "--lang", "c++")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("part 1 (C++): 142", result.stdout)
        self.assertIn("part 2 (C++): 281", result.stdout)
        self.assertNotIn("UNVERIFIED", result.stdout)
        self.assertNotIn("ERROR", result.stdout)

    def test_swift_test_mode_is_unfinished(self):
        result = self.run_part("--lang", "swift", "--test")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("(Swift): UNFINISHED", result.stdout)
        self.assertNotIn("ERROR", result.stdout)
        self.assertNotIn("UNVERIFIED", result.stdout)

    def test_custom_input_is_unverified(self):
        (self.data / "custom.txt").write_text("4abc5\n")
        result = self.run_part("--lang", "c++", "--input", "custom.txt")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertRegex(result.stdout, r"\(C\+\+\): 45 \d+\.\d{3} ms UNVERIFIED\n$")

    def test_missing_expected_answers_are_unverified(self):
        for saved_answers in (None, "", "142\n281\n"):
            with self.subTest(saved_answers=saved_answers):
                path = self.day / "answers.txt"
                if saved_answers is None:
                    path.unlink()
                else:
                    path.write_text(saved_answers)
                result = self.run_part("--lang", "c++")
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertIn("UNVERIFIED", result.stdout)
                self.assertNotIn("ERROR", result.stdout)

    def test_wrong_answer_is_an_error_and_run_continues(self):
        (self.day / "answers.txt").write_text("142 13\n281 12\n")
        result = self.run_part()
        self.assertEqual(result.returncode, 1)
        self.assertEqual(len(result.stdout.splitlines()), 2)
        self.assertIn("ERROR expected=13", result.stdout)
        self.assertIn("(Swift): UNFINISHED", result.stdout)

    def test_invalid_input_is_an_error_and_run_continues(self):
        (self.day / "input.txt").write_text("no digits\n")
        result = self.run_part()
        self.assertEqual(result.returncode, 1)
        self.assertIn("(C++): ERROR", result.stdout)
        self.assertIn("(Swift): UNFINISHED", result.stdout)

    def test_missing_input_is_reported_on_stdout_for_each_language(self):
        (self.day / "input.txt").unlink()
        result = self.run_part()
        self.assertEqual(result.returncode, 1)
        self.assertEqual(len(result.stdout.splitlines()), 2)
        self.assertIn("(C++): ERROR could not read input", result.stdout)
        self.assertIn("(Swift): ERROR could not read input", result.stdout)

    def test_explicit_selection_without_implementation_fails(self):
        result = self.run_aoc("--year", "2023", "--day", "1", "--part", "2", "--lang", "swift")
        self.assertEqual(result.returncode, 1)
        self.assertEqual(result.stdout, "")
        self.assertIn("No registered implementations", result.stderr)

    def test_invalid_language_options_fail(self):
        for arguments in (("--lang", "c"), ("--lang",), ("--lang", "swift", "--lang", "c++")):
            with self.subTest(arguments=arguments):
                result = self.run_aoc(*arguments)
                self.assertEqual(result.returncode, 2)
                self.assertEqual(result.stdout, "")

    def test_help_describes_languages_and_markers(self):
        result = self.run_aoc("--help")
        self.assertEqual(result.returncode, 0, result.stderr)
        for text in ("--lang c++|swift", "ERROR", "UNVERIFIED", "UNFINISHED"):
            self.assertIn(text, result.stdout)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--binary", required=True, type=Path)
    arguments, unittest_arguments = parser.parse_known_args()
    CommandLineTests.binary = arguments.binary.resolve()
    unittest.main(argv=[sys.argv[0], *unittest_arguments])
