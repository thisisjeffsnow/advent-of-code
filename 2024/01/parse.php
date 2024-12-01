<?php

$left = [];
$right = [];

$numbers = preg_match_all("#[0-9]+#", file_get_contents("input.txt"), $matches);

for ($i = 0; $i < count($matches[0]); $i += 2) {
    $left[] = $matches[0][$i];
    $right[] = $matches[0][$i + 1];
}

$result = 0;

sort($left);
sort($right);

for ($i = 0; $i < count($left); $i++) {
    $result += abs($left[$i] - $right[$i]);
}

echo $result;
