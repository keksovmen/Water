<?php
$clock = $_POST["2"];
$volume = $_POST["4"];

if (!isset($clock) || !isset($volume)){
	return;
}

$file = fopen("volumes.txt", "a");
if(!$file){
	return;
}

$text = "Time: " . $clock . "\tVolume: " . $volume . "\n";
fwrite($file, $text);

fclose($file);