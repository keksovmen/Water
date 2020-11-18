<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<title>Set Temperature</title>
	<?php require_once 'Header.html';?>
</head>
<body>
<div>
    Temperature Up: <input type="text" name="tu" id="tu"/><br/>
    Temperature Down: <input type="text" name="td" id="td"/><br/>
    <input type="submit" value="Send" onClick="sendTemperatures()"/>
</div>
<div>
	<input type="text" name="date" id="date"
			pattern="\d{2}:\d{2}:\d{2}:\d{2}:\d{2}:\d{2}"/>
	<input type="submit" value="Send date" onClick="sendDate()"/>
</div>
<div>
	<?php require_once 'PrintTimeStamps.php';?>
</div>
<div>
	<?php require_once 'PrintDataStamps.php';?>
</div>
<div>
	<?php require_once 'PrintVolumeStamps.php';?>
</div>
</body>
</html>