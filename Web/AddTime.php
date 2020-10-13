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


$file = NULL;

if(isset($_POST["tu"])){
    $file = fopen("time.txt", "a");

	if(filesize("time.txt") != 0){
		writeEndTime($file);
	}

	writeEntry($file, $_POST["tu"], $_POST["td"]);

	fclose($file);

}
