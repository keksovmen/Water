<?php

require_once 'ReadTimeStamps.php';
require_once 'Header.html';


function formatCell($data) : string{
	if(is_null($data)){
		return "<td class=\"empty\">NAN</td>";
	}
	
	return "<td>$data</td>";
	
}



if(count($TIME_ENTRIES) == 0){
	echo "NO ENTRIES YET<br/>";
	return;
}


echo "<table>";

//4 columns
echo "<tr>";
echo "<th>" . "Temperature Up" . "</th>";
echo "<th>" . "Temperature Down" . "</th>";
echo "<th>" . "Start Time" . "</th>";
echo "<th>" . "End Time" . "</th>";
echo "</tr>";


foreach ($TIME_ENTRIES as $tmp){
	echo "<tr>";
	echo formatCell($tmp->tu);
	echo formatCell($tmp->td);
	echo formatCell($tmp->start);
	echo formatCell($tmp->end);
	echo "</tr>";
}

echo "</table>";
echo "<input type=\"submit\"value=\"Delete\" onClick=del(\"time\")>";