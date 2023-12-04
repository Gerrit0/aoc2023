// @ts-check

const { readFileSync } = require("fs");

const input = readFileSync("input.txt", "utf-8");

function part1() {
  let answer = 0;
  for (const line of input.split("\n")) {
    if (!line) continue;

    const twoDigit = line.match(/^[^0-9]*([0-9]).*([0-9])[^0-9]*$/);
    if (twoDigit) {
      answer += +`${twoDigit[1]}${twoDigit[2]}`;
      continue;
    }

    const oneDigit = line.match(/[0-9]/);
    if (!oneDigit) throw "Not valid";
    answer += +`${oneDigit[0]}${oneDigit[0]}`;
  }

  console.log(answer); // 55172
}

function part2() {
  const numbers = [
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
  ];

  /** @type {(line: string, index: number) => boolean} */
  function isStartOfNumber(line, index) {
    if (/[0-9]/.test(line[index])) return true;

    return numbers.some((num) => line.startsWith(num, index));
  }

  /** @type {(line: string, index: number) => number} */
  function parseNumber(line, index) {
    const answer = parseInt(line.substring(index, index + 1));
    if (!Number.isNaN(answer)) return answer;
    return numbers.findIndex((num) => line.startsWith(num, index));
  }

  let answer = 0;
  for (const line of input.split("\n")) {
    if (!line) continue;

    let first = -1;
    let last = -1;

    for (let i = 0; i < line.length; i++) {
      if (isStartOfNumber(line, i)) {
        first = parseNumber(line, i);
        last = first;
        break;
      }
    }

    for (let i = line.length - 1; i >= 0; i--) {
      if (isStartOfNumber(line, i)) {
        last = parseNumber(line, i);
        break;
      }
    }

    answer += +`${first}${last}`;
  }

  console.log(answer); // 55172
}

// part1();
part2();
