<?php
    if (isset($_POST['usuario']) && isset($_POST['password'])) {
        $usu = $_POST['usuario'];
        $pass = $_POST['password'];
        if ($usu=="admin" && $pass=="1234"){
            session_start();
            $_SESSION['auth'] = 1;
            header("Location: ../menu.php");

        }else{
            header("Location: ../login.php");
        }
    }
?>