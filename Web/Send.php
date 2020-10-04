<?php

//Params: temperature, pressure

//fetch data

$temp = $_POST["temperature"];
$pres = $_POST["pressure"];

//make as string

$txtData = "Temperature - C: " . $temp . "\tPressure - mb: " . $pres . "\n";

//open or create data file

$file = fopen("data.txt", "a");
fwrite($file, $txtData);
fclose($file);
