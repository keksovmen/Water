<?php

class VolumeStamp
{
	
	public function parse($str){
		$expTime= "/Time: ((\d+:\d+:\d+):(\d+:\d+:\d+))/iu";
		$expVolume = "/Volume: (-?\d*\.?\d*)/iu";
		
		if(preg_match($expTime, $str, $matches)){
			$this->dateAndTime = $matches[1];
			$this->date = $matches[2];
			$this->time = $matches[3];
		}
		
		if(preg_match($expVolume, $str, $matches)){
			$this->volume = $matches[1];
		}
		
	}
	
	public $volume;
	public $date;
	public $time;
	public $dateAndTime;
}


$VOLUME_ENTRIES = array();


$file = fopen("volumes.txt", "r");
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
	
	$tmp = new VolumeStamp();
	$tmp->parse($line);
	
	$VOLUME_ENTRIES[] = $tmp;
}

fclose($file);