<?php

function writeEndTime($file){
    $time = date("d:m:y:H:i:s");
    fwrite($file, "End - $time\n");
}


function writeEntry($file, $tu, $td){
	$time = date("d:m:y:H:i:s");
	$data = "TU - $tu\t" . "TD - $td\t" . "Start - $time\t";
	fwrite($file, $data);
}


function writeToSim($tu, $td) : bool{
	require_once 'TCPHandler.php';
	
	$tcp = new TCPHandler();
	if(!$tcp->init()){
		return false;
	}
	if(!$tcp->connect("127.0.0.1", 21693)){
		return false;
	}
	
	$txt = "-1\n#8=" . $tu . "$#9=" . $td . "$\n";
	if($tcp->send($txt)){
// 		return false;
	}
	
	$tcp->close();
	
	return true;
}


$file = NULL;

if(isset($_POST["tu"])){
    $file = fopen("time.txt", "a");

	if(filesize("time.txt") != 0){
		writeEndTime($file);
	}

	writeEntry($file, $_POST["tu"], $_POST["td"]);
	if(writeToSim($_POST["tu"], $_POST["td"])){
		echo "Success";
	}else{
		echo "Failure";
	}
	
	fclose($file);

}
