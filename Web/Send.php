<?php

//Params: temperature, pressure, time

//fetch data
$temp = $_POST["0"];
$pres = $_POST["1"];
$time = $_POST["2"];

//make as string

$txtData = "Temperature - C: " . $temp . "\tPressure - mb: " . $pres . "\tTime: " . $time . "\n";

//open or create data file

$file = fopen("data.txt", "a");
fwrite($file, $txtData);
fclose($file);