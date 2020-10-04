<?php
echo "<script>
function del(){
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function(){
		if(this.readyState == 4 && this.status == 200){
			window.location.reload(false);
		}
	}
	
	
	xhttp.open(\"Get\", \"Delete.php\", true);
	xhttp.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");
    xhttp.send();
}
</script>
";

echo "<style>table, th, td {border: 1px solid black;}</style>";

echo "<table>";

echo "<tr>";
echo "<th>" . "Temperature" . "</th>";
echo "<th>" . "Pressure" . "</th>";
echo "</tr>";

$file = fopen("data.txt", "r");
if(!$file){
    echo "ERROR with file opening";
    return;
}
$exp = "/Temperature - C: (\d*.?\d*)\sPressure - mb: (\d*.?\d*)/iu";
while(!feof($file)){
    $line = fgets($file);
    $line = trim($line);
    if(strlen($line) == 0){
        break;
    }
    $result = preg_match($exp, $line, $matches);
    if($result == 1){
        echo "<tr>";
        echo "<th>" . $matches[1] . "</th>";
        echo "<th>" . $matches[2] . "</th>";
        echo "</tr>";
    }
}

echo "</table>";

echo "<input type=\"submit\" onClick=\"del()\">";
fclose($file);


