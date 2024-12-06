const fs = require('fs');
const readline = require('readline');

const filePath = './input.txt';

const rl = readline.createInterface({
    input: fs.createReadStream(filePath),
    output: process.stdout,
    terminal: false,
});

let safeReports = 0;
let safeReportsWithDampener = 0;

/* ||x - y| - 1| <= 2 is equivalent to 1 <= |x - y| <= 3 */
const isSafeDiff = (x, y) => Math.abs(Math.abs(x - y) - 1) <= 2;

const isSafeReport = (posDiffs, negDiffs, unsafeDiffs) => {
    return unsafeDiffs + posDiffs * negDiffs === 0;
};

const diffParse = (left, right, counts, mod) => {
    if (right - left > 0) {
        counts.pos += mod;
    } else if (right - left < 0) {
        counts.neg += mod;
    } else {
        counts.unsafe += mod;
    }
    counts.unsafe += isSafeDiff(left, right) ? 0 : mod;
};

rl.on('line', (line) => {
    const reports = line.split(' ').map(Number);
    const N = reports.length;

    if (N < 2) {
        safeReports++;
        safeReportsWithDampener++;
        return;
    }

    // Initialize counts object for part 1
    let counts = { pos: 0, neg: 0, unsafe: 0 };

    // Part 1: Count differences
    for (let i = 0; i + 1 < N; ++i) {
        diffParse(reports[i], reports[i + 1], counts, 1);
    }

    if (isSafeReport(counts.pos, counts.neg, counts.unsafe)) {
        safeReports++;
        safeReportsWithDampener++; // safe reports are safe with dampening
        return;
    }

    // Part 2: Does removing a report make that level safe?
    for (let i = 0; i < N; ++i) {
        let tempCounts = { ...counts };

        let left = i > 0 ? reports[i - 1] : undefined;
        let middle = reports[i];
        let right = i + 1 < N ? reports[i + 1] : undefined;

        if (i > 0) {
            diffParse(left, middle, tempCounts, -1);
        }
        if (i + 1 < N) {
            diffParse(middle, right, tempCounts, -1);
        }
        if (i > 0 && i + 1 < N) {
            diffParse(left, right, tempCounts, 1);
        }

        if (isSafeReport(tempCounts.pos, tempCounts.neg, tempCounts.unsafe)) {
            safeReportsWithDampener++;
            break;
        }
    }
});

rl.on('close', () => {
    console.log('Finished reading input.txt');
    console.log(`Safe Reports (Without Dampener): ${safeReports}`);
    console.log(`Safe Reports (With Dampener): ${safeReportsWithDampener}`);
});
