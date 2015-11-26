<?php
	header('Content-Type: text/plain; charset=utf-8');

	$EveDay = strtotime("2015-12-25");
	$futureEve = date('Y-m-d', strtotime('+1 year',$EveDay));
	$Today = strtotime("today");
	
	if($EveDay > $Today) {
		$diff = $EveDay - $Today;
		echo "Hey Java Agent, you know only ";
		echo floor($diff / 86400);
		echo " days left for Chirstmas!";
	}else if($Today == strtotime("2015-12-25") || $Today == $futureEve ){
		echo "Today is Chirstmas, Java Agent! Merry Chirstmas!";
	}else{
		$diff = strtotime($futureEve) - $Today;	
		echo "Hey Java Agent, you know only ";
		echo floor($diff / 86400);
		echo " days left for Chirstmas!";
	}
?>