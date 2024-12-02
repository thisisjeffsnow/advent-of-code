const fs = require('fs');
const readline = require('readline');

const filePath = './input.txt';

const rl = readline.createInterface({
  input: fs.createReadStream(filePath),
  output: process.stdout,
  terminal: false,
});

let safeReportsWithoutDampener = 0; // Part 1
let safeReportsWithDampener = 0; // Part 2

const isSafe = (levels) => {

  // |x - b| <= a is equivalent to a - b <= x <= a + b
  // x = |report - levels[index - 1]|
  // a = 2
  // b = 1
  // a + b = 3 (max difference)
  // a - b = 1 (min difference)

  // Check if all increasing
  const isIncreasing = levels.every((report, index) => {
    return index === 0 || (report > levels[index - 1] && Math.abs(Math.abs(report - levels[index - 1]) - 1) <= 2);
  });

  // Check if all decreasing
  const isDecreasing = levels.every((report, index) => {
    return index === 0 || (report < levels[index - 1] && Math.abs(Math.abs(report - levels[index - 1]) - 1) <= 2);
  });

  return isIncreasing || isDecreasing;
};

rl.on('line', (line) => {
  const reports = line.split(' ').map(Number);

  // Part 1: Count safe reports without dampener
  if (isSafe(reports)) {
    safeReportsWithoutDampener++;
    safeReportsWithDampener++; // If it's safe without dampener, it's also safe with it
    return;
  }

  // Part 2: Count safe reports with dampener
  for (let i = 0; i < reports.length; i++) {
    const modifiedReport = reports.slice(0, i).concat(reports.slice(i + 1));
    if (isSafe(modifiedReport)) {
      safeReportsWithDampener++;
      return;
    }
  }
});

rl.on('close', () => {
  console.log('Finished reading input.txt');
  console.log(`Safe Reports (Without Dampener): ${safeReportsWithoutDampener}`);
  console.log(`Safe Reports (With Dampener): ${safeReportsWithDampener}`);
});
