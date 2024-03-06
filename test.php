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
	$tt = 22;
 	$hh = 23;
 	$ll =24;
 	$gg = 25;
 	$pp = 26;
 	$xx = "on";
	$yy = "on";
	$zz = "on";
	$ww = "on";
 	
	
	*/
	if(isset($_POST["Temperature"]) && isset($_POST["Humidity"]) && isset($_POST["Luminosity"]) && isset($_POST["Gaz"]) && isset($_POST["Pressure"]) && isset($_POST["Printer"]) &&  isset($_POST["Ventilator"]) &&  isset($_POST["Led1"]) &&  isset($_POST["Led2"]))
	{
	$tt = $_POST["Temperature"];
 	$hh = $_POST["Humidity"];
 	$ll = $_POST["Luminosity"];
 	$gg = $_POST["Gaz"];
 	$pp = $_POST["Pressure"];
 	$xx = $_POST["Printer"];
	$yy = $_POST["Ventilator"];
	$zz = $_POST["Led1"];
	$ww = $_POST["Led2"];
 
	

	
	$sql = "INSERT INTO esp32_data (Temperature, Humidity,Luminosity,Gaz,Pressure,Printer,Ventilator,Led1,Led2) VALUES (".$tt.", ".$hh.", ".$ll.", ".$gg.", ".$pp.",'$xx','$yy','$zz','$ww')";

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