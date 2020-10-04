<?php

$file = fopen("data.txt", "r");
if(!$file){
    echo "ERROR with file opening";
    return;
}
$exp = "/Temperature - C: (\d*.?\d*)\sPressure - mb: (\d*.?\d*)/iu";
while(!feof($file)){
    $line = fgets($file);
    echo $line;
}

fclose($file);


