<?php


class TCPHandler {
	
	public function init() : bool{
		$this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		return $this->socket !== false;
	}
	
	public function connect($ip, $port) : bool {
		return socket_connect($this->socket, $ip, $port) !== false;
	}
	
	public function send($str) : bool{
		$result = socket_write($this->socket, $str);
		if($result === false){
			return false;
		}
		
		$result = socket_read($this->socket, 1);
		if($result === false){
			return false;
		}
		
		return $result == 1;
	}
	
	public function close(){
		socket_close($this->socket);
	}
	
	private $socket;
}


// $connection = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
// if($connection === false){
// 	die("Can't create socket");
// }

// if(socket_connect($connection, "127.0.0.1", 8188) === false){
// 	die("Can't connect to the server");
// }


// $text = "-1\n";
// $result = socket_write($connection, $text);
// if($result === false){
// 	die("Coudn't write data in to the connection");
// }

// echo "Written amount: $result";
// echo "\nActual Length: " . strlen($text);


// // socket_recv($connection, $result, $len, $flags)
// $result = socket_read($connection, 6);
// if($result === false){
// 	die("Coudn't read data from server");
// }

// $result = socket_write($connection, "0\n");
// if($result === false){
// 	die("Coudn't respond with PONG");
// }

// echo "\nRead data is: $result";

// socket_close($connection);