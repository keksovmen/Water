<?php

class DataEntry
{
	public function parse($str){
		$expTemperature = "/Temperature - C: (-?\d*\.?\d*)/iu";
		$expPressure = "/Pressure - mb: (-?\d*\.?\d*)/iu";
		$expTime= "/Time: ((\d+:\d+:\d+):(\d+:\d+:\d+))/iu";
		
		if(preg_match($expTemperature, $str, $matches)){
			$this->temp = $matches[1];
		}
		
		if(preg_match($expPressure, $str, $matches)){
			$this->press = $matches[1];
		}
		
		if(preg_match($expTime, $str, $matches)){
			$this->dateAndTime = $matches[1];
			$this->date = $matches[2];
			$this->time = $matches[3];
		}
	}
	
	public $temp;
	public $press;
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
