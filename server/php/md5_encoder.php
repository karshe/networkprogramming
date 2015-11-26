<?php
	header('Content-Type: text/plain; charset=utf-8');
	echo md5($_POST['msg']);
?>