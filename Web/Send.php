<?php

//Params: temperature, pressure, time

//fetch data
$tempUp = $_POST["0"];
$tempDown = $_POST["1"];
$time = $_POST["2"];
$id = $_POST["10"];

if(!isset($tempUp) && !isset($tempDown) && !isset($time)){
	require_once 'PostParser.php';
	$tempUp = $_POST["0"];
	$tempDown = $_POST["1"];
	$time = $_POST["2"];
	$id = $_POST["10"];
}


// $time = str_replace(":", "-", $time);

// "d-m-y-H-i-s"

include_once 'DatabaseHandler.php';

sendParams($id, $time, $tempUp, $tempDown);


// if(!isset($temp) && !isset($pres) && !isset($time)){
// 	die();
// }

//make as string

// $txtData = "Temperature Up - C: " . $tempUp . "\tTemperature Down - C: " . $tempDown . "\tTime: " . $time . "\n";

//open or create data file

// $file = fopen("data.txt", "a");
// fwrite($file, $txtData);
// fclose($file);

