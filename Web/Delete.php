<?php

include_once 'DatabaseHandler.php';

if($_GET["file"] == "data"){
//     unlink("data.txt");
	deleteEverethingFrom("sensor_entries");
}else if($_GET["file"] == "time"){
    unlink("time.txt");
}else if($_GET["file"] == "volume"){
	unlink("volumes.txt");
}else if($_GET["file"] == "arduino"){
	deleteEverethingFrom("arduino");
}

return;