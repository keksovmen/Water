<?php

require_once 'ReadDataEntry.php';
require_once 'ReadTimeStamps.php';
require_once 'Header.html';


if(count($DATA_ENTRIES) == 0){
	echo "NO ENTRIES YET<br/>";
	return;
}

if(count($TIME_ENTRIES) == 0){
	echo "NO TIME STAMPS FOR RED COLOR<br/>";
}

echo "<table>";

//4 columns
echo "<tr>";
echo "<th>" . "Arduino ID" . "</th>";
echo "<th>" . "Temperature Up" . "</th>";
echo "<th>" . "Temperature Down" . "</th>";
echo "<th>" . "Date and Time" . "</th>";
// echo "<th>" . "Time" . "</th>";
echo "</tr>";


function formatDataCell($data, $isRed = false) : string{
	if($isRed){
		return "<td class=\"red\">$data</td>";
	}
	
	if(is_null($data)){
		return "<td class=\"empty\">NAN</td>";
	}
	
	return "<td>$data</td>";
	
}


foreach ($DATA_ENTRIES as $tmp){
	$isRed = false;
	
	if(!is_null($tmp->dateAndTime)){
		$currentDate = date($tmp->dateAndTime);
		foreach($TIME_ENTRIES as $time){
			
			$startDate = date($time->start);
			
			if($currentDate >= $startDate){
				if(is_null($time->end)){
					if((float)$tmp->tempUp > (float)$time->tu || 
						(float)$tmp->tempUp < (float)$time->td)
					{
						$isRed = true;
					}
				}else{
					$endDate = date($time->end);
					if($currentDate <= $endDate){
						if((float)$tmp->tempUp > (float)$time->tu ||
							(float)$tmp->tempUp < (float)$time->td)
						{
							$isRed = true;
						}
					}
				}
			}
		}
	}
	
	
	echo "<tr>";
	echo formatDataCell($tmp->id);
	echo formatDataCell($tmp->tempUp, $isRed);
	echo formatDataCell($tmp->tempDown);
	echo formatDataCell($tmp->dateAndTime);
// 	echo formatDataCell($tmp->time);
	echo "</tr>";
}

echo "</table>";
echo "<input type=\"submit\"value=\"Delete\" onClick=del(\"data\")>";