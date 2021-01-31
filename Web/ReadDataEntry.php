<?php

class DataEntry
{
	
	public function __construct($tempUp, $tempDown, $dateAndTime, $id){
		$this->tempUp = $tempUp;
		$this->tempDown = $tempDown;
		$this->dateAndTime= $dateAndTime;
		$this->id= $id;
	}
	
	public function parse($str){
		$expTemperatureUp = "/Temperature Up - C: (-?\d*\.?\d*)/iu";
		$expTemperatureDown = "/Temperature Down - C: (-?\d*\.?\d*)/iu";
		$expTime= "/Time: ((\d+:\d+:\d+):(\d+:\d+:\d+))/iu";
		
		if(preg_match($expTemperatureUp, $str, $matches)){
			$this->tempUp = $matches[1];
		}
		
		if(preg_match($expTemperatureDown, $str, $matches)){
			$this->tempDown = $matches[1];
		}
		
		if(preg_match($expTime, $str, $matches)){
			$this->dateAndTime = $matches[1];
			$this->date = $matches[2];
			$this->time = $matches[3];
		}
	}
	
	public $tempUp;
	public $tempDown;
	public $date;
	public $time;
	public $dateAndTime;
	public $id;
}



$DATA_ENTRIES = array();

include_once 'DatabaseHandler.php';

$result = readParams(null);
if($result){
	while ($row = $result->fetch()){
		$tmp = new DataEntry($row["temp_up"], $row["temp_down"], $row["time"], $row["arduino_id"]);
		$DATA_ENTRIES[] = $tmp;
	}
}

// $file = fopen("data.txt", "r");
// if(!$file){
// 	echo "ERROR with data file opening<br/>";
// 	return;
// }


// while(!feof($file)){
// 	$line = fgets($file);
	
// 	//remove left and right spaces
// 	$line = trim($line);
	
// 	if(strlen($line) == 0){
// 		break;
// 	}
	
// 	$tmp = new DataEntry();
// 	$tmp->parse($line);
	
// 	$DATA_ENTRIES[] = $tmp;
// }

// fclose($file);
