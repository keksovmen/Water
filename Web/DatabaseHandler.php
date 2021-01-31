<?php

// try{
// 	$db = new PDO("mysql:host=localhost;dbname=arduino", "php", "1234");
// }catch (PDOException $e){
// 	echo "Connection failed due to: " . $e->getMessage();
// }

class DBHandler
{
	
	private $connection = null;
	private $error = null;
	
	
	
	public function __construct(){
		try{
			$this->connection = new PDO("mysql:host=localhost;dbname=arduino", "php", "1234");
		}catch (PDOException $e){
			// 		echo "Connection failed due to: " . $e->getMessage();
			$this->error = $e;
			$this->connection = null;
		}
	}
	
	public function is_init(){
		return !is_null($this->connection);
	}
	
	public function execute(string $str) : bool
	{
		$result = $this->connection->exec($str);
		return $result !== false;
	}
	
	public function query(string $str)
	{
		return $this->connection->query($str);
	}
	
	public function error_message() : string
	{
		if(is_null($this->error)){
			return "No errors was found";
		} else {
			return "Failed due to: " . $this->error->getMessage();
		}
	}
	
}


function convertTimeInSQL(string $time) : string
{
	return "STR_TO_DATE('$time', '%d:%m:%y:%H:%i:%s')";
}


function sendParams($id, $time, $tu, $td) : bool
{
	
	$connection = new DBHandler();
	
	if(!$connection->is_init()){
		return false;
	}
	
	
	$query = "INSERT INTO sensor_entries (arduino_id, time, temp_up, temp_down) " .
		"values($id," . convertTimeInSql($time) . " , $tu, $td)";
	
	return $connection->execute($query);
}


function readParams($where)
{
	$connection = new DBHandler();
	
	if(!$connection->is_init()){
		echo $connection->error_message();
		return false;
	}
	
	$query = "SELECT arduino_id, time, temp_up, temp_down, update_time FROM sensor_entries" . 
			(is_null($where) ? "" : " WHERE " . $where);
	return $connection->query($query);
}


function readActive()
{
	$connection = new DBHandler();
	
	if(!$connection->is_init()){
		echo $connection->error_message();
		return false;
	}
	
	$query = "SELECT arduino_id, imei, (CASE is_active WHEN 1 THEN 'Yes' ELSE 'No' end) " .
		"current_state, last_time_update FROM arduino " . 
		"ORDER BY is_active DESC, last_time_update DESC";
	return $connection->query($query);
}


function deleteEverethingFrom(string $table) : bool
{
	$connection = new DBHandler();
	
	if(!$connection->is_init()){
		echo $connection->error_message();
		return false;
	}
	
	return $connection->execute("DELETE FROM " . $table);
}