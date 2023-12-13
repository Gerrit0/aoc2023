<?php

$lines = explode("\n", file_get_contents("input.txt"));

function to_nums($str) {
    $no_space = preg_replace("/[ ]{2,}/", " ", $str);
    $nums = trim(preg_replace("/[^0-9 ]/", "", $no_space));
    return array_map(function($x) { return intval($x); }, explode(" ", $nums));
}

$times = to_nums($lines[0]);
$distances = to_nums($lines[1]);

function calc_wins($duration, $record) {
    $wins = 0;
    for ($i = 1; $i < $duration - 1; $i += 1) {
        if ($i * ($duration - $i) > $record) {
            $wins++;
        } else if ($wins > 1) {
            break;
        }
    }

    return $wins;
}

$answer = 1;
for ($i = 0; $i < count($times); $i++) {
    $answer *= calc_wins($times[$i], $distances[$i]);
}

printf("Part 1: %d\n", $answer);

$time = intval(implode("", $times));
$record = intval(implode("", $distances));
$answer2 = calc_wins($time, $record);

printf("Part 2: %d\n", $answer2);

?>
