<?php


function parseUID($str) : string {
	$result = "";
	$t = strtok($str, ",");
	// 			$this->uidNumber = bcadd($this->uidNumber, $t);
	while($t !== false){
		$result = bcadd($result, $t);
		$t = strtok(",");
		if($t !== false){
			$result = bcmul($result, 256);
		}
	}
	
	return $result;
}


$clock = $_POST["2"];
$volume = $_POST["5"];
$uid = $_POST["6"];


if (!isset($clock) || !isset($volume) || !isset($uid)){
	return;
}

$file = fopen("volumes.txt", "a");
if(!$file){
	return;
}

$text = "Time: " . $clock . "\tVolume: " . $volume . "\tUID: " . parseUID($uid) . "\n";
fwrite($file, $text);

fclose($file);