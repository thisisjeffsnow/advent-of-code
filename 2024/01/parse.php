<?php

// Load input and parse numbers into left and right lists
$input = file_get_contents("input.txt");
preg_match_all("#[0-9]+#", $input, $matches);

$left = [];
$right = [];

for ($i = 0; $i < count($matches[0]); $i += 2) {
    $left[] = (int)$matches[0][$i];
    $right[] = (int)$matches[0][$i + 1];
}

// Part 1: Calculate total distance
sort($left);
sort($right);

$total_distance = array_reduce(
    range(0, count($left) - 1),
    fn($carry, $i) => $carry + abs($left[$i] - $right[$i]),
    0
);

echo "Part 1: $total_distance\n";

// Part 2: Calculate similarity score
$left_counts = array_count_values($left);
$right_counts = array_count_values($right);

$similarity_score = 0;

foreach ($left_counts as $key => $count) {
    $similarity_score += ($right_counts[$key] ?? 0) * $key;
}

echo "Part 2: $similarity_score\n";
