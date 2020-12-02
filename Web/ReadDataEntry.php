<?php

class DataEntry
{
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
}



$DATA_ENTRIES = array();

$file = fopen("data.txt", "r");
if(!$file){
	echo "ERROR with data file opening<br/>";
	return;
}


while(!feof($file)){
	$line = fgets($file);
	
	//remove left and right spaces
	$line = trim($line);
	
	if(strlen($line) == 0){
		break;
	}
	
	$tmp = new DataEntry();
	$tmp->parse($line);
	
	$DATA_ENTRIES[] = $tmp;
}

fclose($file);
