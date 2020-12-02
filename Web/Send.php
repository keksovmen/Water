<?php

//Params: temperature, pressure, time

//fetch data
$tempUp = $_POST["0"];
$tempDown = $_POST["1"];
$time = $_POST["2"];

if(!isset($tempUp) && !isset($tempDown) && !isset($time)){
	require_once 'PostParser.php';
	$tempUp = $_POST["0"];
	$tempDown = $_POST["1"];
	$time = $_POST["2"];
}


// if(!isset($temp) && !isset($pres) && !isset($time)){
// 	die();
// }

//make as string

$txtData = "Temperature Up - C: " . $tempUp . "\tTemperature Down - C: " . $tempDown . "\tTime: " . $time . "\n";

//open or create data file

$file = fopen("data.txt", "a");
fwrite($file, $txtData);
fclose($file);