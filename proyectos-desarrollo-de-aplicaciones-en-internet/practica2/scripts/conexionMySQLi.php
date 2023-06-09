<?php
$conexion = new mysqli("localhost", "root", "", "taller_embarcaciones");
/* verificar la conexión */
if (mysqli_connect_errno()) {
    printf("Falló la conexión: %s\n", $mysqli->connect_error);
    exit();
}
?>