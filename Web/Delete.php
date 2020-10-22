<?php

if($_GET["file"] == "data"){
    unlink("data.txt");
}else if($_GET["file"] == "time"){
    unlink("time.txt");
}else if($_GET["file"] == "volume"){
	unlink("volumes.txt");
}

return;