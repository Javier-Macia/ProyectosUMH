<?php
include  './pageBuilder.php';
include './seguridad.php';
include './conexionMySQLi.php';

$tabla=$_POST["tabla"];
if (empty($tabla)){
    header("Location: ../administrar.php");
}
// Leemos los campos de la base de datos
include './obtener_campos.php';
$fields=obtener_campos($tabla,$conexion);

$array_borrados=$_POST["borrar"];
if (empty($array_borrados)){
    header("Location: ../administrar.php?tabla=".$tabla);
}
$error=0;
for($i=0;$i<count($array_borrados);$i++){
    $SentenciaSQL="DELETE FROM ". $tabla ." WHERE " . $fields[0] ."='$array_borrados[$i]'";
    $result=$conexion->query($SentenciaSQL);
    if(!$result){
        $error=1;
    }

    // Si la tabla es de facturas, las lineas asociadas a la factura se borran también
    if ($tabla == "factura"){
        $SentenciaSQL="DELETE FROM detalle_factura WHERE Numero_Factura='$array_borrados[$i]'";
        $result=$conexion->query($SentenciaSQL);
        if(!$result){
            $error=1;
        }
    }

    if($error==0){
        if ($tabla == "detalle_factura"){
            header("Location: ../administrar.php?tabla=facturas&id=".$_POST["id_factura"]);
        }
        header("Location: ../administrar.php?tabla=".$tabla);
    }else{
        echo "<p>Error al borrar el/los registro/s</p>";
        echo "<p>".$SentenciaSQL."</p>";
        echo "<a href='../administrar.php?tabla=".$tabla."'>Volver</a>";
    }
}


