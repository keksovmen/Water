<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<title>Arduino <?php echo $_GET["id"]?></title>
	<?php require_once 'Header.html';?>
</head>
<body>
<div>
<?php

echo "
	<table>
	<tr>
	<th>Arduino ID</th>
	<th>Temperature Up</th>
	<th>Temperature Down</th>
	<th>When taken</th>
	<th>When loaded into DB</th>
</tr>";

include_once 'DatabaseHandler.php';
$result = readParams("arduino_id = " . $_GET["id"]);
if(!$result){
	echo "Error with SQL part";
	return;
}

while ($row = $result->fetch()) {
	echo "
		<tr>
		<td>" . $row['arduino_id'] . "</td>
		<td>" . $row['temp_up'] . "</td>
		<td>" . $row['temp_down'] . "</td>
		<td>" . $row['time'] . "</td>
		<td>" . $row['update_time'] . "</td>
		</tr>
	";
}

?>

</div>
<!-- <div> -->
<input type="submit" value="Delete all from data base" onclick="del('arduino')">
<!-- </div> -->