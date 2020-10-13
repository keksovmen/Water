<?php


class Entry{
	
	public function parse($str){
		
		$expTU = "/TU - (\d+)\t/iu";
		$expTD = "/TD - (\d+)\t/iu";
		$expStart = "/Start - ((\d+:\d+:\d+):(\d+:\d+:\d+))/iu";
		$expEnd = "/End - ((\d+:\d+:\d+):(\d+:\d+:\d+))/iu";
		
		if(preg_match($expTU, $str, $matches)){
			$this->tu = $matches[1];
		}
			
		if(preg_match($expTD, $str, $matches)){
			$this->td = $matches[1];
		}
			
		if(preg_match($expStart, $str, $matches)){
			$this->start = $matches[1];
		}
			
		if(preg_match($expEnd, $str, $matches)){
			$this->end = $matches[1];
		}
	}
	
	public $tu;
	public $td;
	public $start;
	public $end;
	
}

$file = fopen("time.txt", "r");

if(!$file){
	echo "File is empty<br/>";
	return;
}


$TIME_ENTRIES = array();
// $count = 0;

while(!feof($file)){
	$line = fgets($file);
	//remove left and right spaces
	$line = trim($line);
	
	if(strlen($line) == 0){
		break;
	}
	
	$tmp = new Entry();
	$tmp->parse($line);
	
// 	$ENTRIES[$count++] = $tmp;
	$TIME_ENTRIES[] = $tmp;
}

fclose($file);
