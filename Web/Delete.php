<?php

if($_GET["file"] == "data"){
    unlink("data.txt");
    
}else if($_GET["file"] == "time"){
    unlink("time.txt");
}

return;