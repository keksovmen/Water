<?php

$time = $_POST["date"];
if(!isset($time)){
	return;
}

// $file = fopen("Loh.txt", "w");
// if(!$file){
// 	return;
// }

require_once 'TCPHandler.php';

$tcp = new TCPHandler();
if(!$tcp->init() ||!$tcp->connect("127.0.0.1", 21693)){
	echo "Failure";
	return;
}

$txt = "-1\n#2=" . $time . "$\n";
if($tcp->send($txt)){
// 	echo "Failure";
// 	return;
}

$tcp->close();

echo "Success";

// fwrite($file, $txt);

// fclose($file);