<?php
function dbConnect(){
    $conexion = null;
    $host = 'localhost';
    $db = 'taller_embarcaciones';
    $user = 'root';
    $pwd = '';
    $charset = 'UTF8';

    try {
        $conexion = new PDO('mysql:host='.$host.';dbname='.$db, $user,
        $pwd,array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
        //echo 'Conectado con exito.<br/>';
    }
    catch(PDOException $e){
        echo '<p>No se puede conectar con la base de datos.</p>';
        exit;
    }
    return $conexion;
}
$conexion = dbConnect();
?>