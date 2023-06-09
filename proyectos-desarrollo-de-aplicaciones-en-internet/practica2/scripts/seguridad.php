<?php
if ($_SESSION["auth"] != 1){
    header("Location: login.php");
}
?>