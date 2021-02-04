

create database arduino;

create user 'php'@'localhost' identified by '1234';
create user 'java'@'localhost' identified by '1234';
grant all on arduino to 'php'@'localhost';
grant all on arduino to 'java'@'localhost';


create table arduino (
	arduino_id INT NOT NULL,
	imei varchar(20) NOT NULL,
	is_active tinyint(1) NOT NULL,
	last_time_update DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	CONSTRAINT pk_arduino PRIMARY KEY (arduino_id)
	
);
	
create table sensor_entries (
	arduino_id INT NOT NULL,
	time DATETIME NOT NULL,
	temp_up FLOAT NOT NULL,
	temp_down FLOAT NOT NULL,
	upload_time DATETIME DEFAULT CURRENT_TIMESTAMP,
	CONSTRAINT pk_sensor PRIMARY KEY (arduino_id, time),
	CONSTRAINT fk_sensor FOREIGN KEY (arduino_id) REFERENCES arduino (arduino_id)
	
);

