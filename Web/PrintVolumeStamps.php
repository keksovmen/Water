<?php

require_once 'ReadVolumeStamps.php';
require_once 'Header.html';


function formatCellVolume($data) : string{
	if(is_null($data)){
		return "<td class=\"empty\">NAN</td>";
	}
	
	return "<td>$data</td>";
	
}



if(count($VOLUME_ENTRIES) == 0){
	echo "NO ENTRIES YET<br/>";
	return;
}


echo "<table>";

//4 columns
echo "<tr>";
echo "<th>" . "Date" . "</th>";
echo "<th>" . "Time" . "</th>";
echo "<th>" . "Volume" . "</th>";
echo "</tr>";


foreach ($VOLUME_ENTRIES as $tmp){
	echo "<tr>";
	echo formatCellVolume($tmp->date);
	echo formatCellVolume($tmp->time);
	echo formatCellVolume($tmp->volume);
	echo "</tr>";
}

echo "</table>";
echo "<input type=\"submit\"value=\"Delete\" onClick=del(\"volume\")>";