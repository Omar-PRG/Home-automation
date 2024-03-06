<?php

 
$hostname = "localhost"; 
$username = "root"; 
$password = ""; 
$database = "esp32_data"; 

$conn = mysqli_connect($hostname, $username, $password, $database);

if (!$conn) { 
	die("Connection failed: " . mysqli_connect_error()); 
} 

echo "Database connection is OK<br>"; 
/*
	$t = 22;
 	$h = 23;
 	$l =24;
 	$g = 25;
 	$p = 26;
 	$x = "Printer";
	$y = "Ventilator";
	$z = "Led1";
	$w = "Led2";
 	
	*/
	if(isset($_POST["Temperature"]) && isset($_POST["Humidity"]) && isset($_POST["Luminosity"]) && isset($_POST["Gaz"]) && isset($_POST["Pressure"]) && isset($_POST["Printer"]) &&  isset($_POST["Ventilator"]) &&  isset($_POST["Led1"]) &&  isset($_POST["Led2"]))
	{
	$t = $_POST["Temperature"];
 	$h = $_POST["Humidity"];
 	$l = $_POST["Luminosity"];
 	$g = $_POST["Gaz"];
 	$p = $_POST["Pressure"];
 	$x = $_POST["Printer"];
	$y = $_POST["Ventilator"];
	$z = $_POST["Led1"];
	$w = $_POST["Led2"];
 
	

	
	$sql = "INSERT INTO esp32_data (Temperature, Humidity,Luminosity,Gaz,Pressure,Printer,Ventilator,Led1,Led2) VALUES (".$t.", ".$h.", ".$l.", ".$g.", ".$p.",'$x','$y','$z','$w')";

	if ($conn->query($sql)==TRUE)
		
	{
		echo "New record created successfully";
	}
	else 
	{ 

		echo "Error: " . $conn->error;	
		
	}
		
		$conn->close();
	
	}

?>