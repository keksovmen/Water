<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<title>Arduino States</title>
	<?php require_once 'Header.html';?>
</head>
<body>
<div>

<?php

echo "
	<table>
	<tr>
	<th>Arduino ID</th>
	<th>IMEI</th>
	<th>Connected</th>
	<th>Last time observed</th>
</tr>";

include_once 'DatabaseHandler.php';
$result = readActive();
if(!$result){
	echo "Error with SQL part";
	return;
}

while ($row = $result->fetch()) {
	$id = $row['arduino_id'];
	$ref = "<a href=\"ReadArduino.php?id=$id\">";
	echo "
		<tr>
		<td>" . $ref . $id . "</td>
		<td>" . $row['imei'] . "</td>
		<td>" . $row['current_state'] . "</td>
		<td>" . $row['last_time_update'] . "</td>
		</tr>
	";
}

?>

</div>
<div>
<input type="submit" value="Delete all from data base" onclick="del('arduino')">
</div>