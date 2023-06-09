<?php
include  './pageBuilder.php';
include './seguridad.php';
include './conexionMySQLi.php';

$tabla=$_POST["tabla"];

if ($tabla!="detalle_factura") {
    header("Location: ../administrar.php");
}
// Leemos los campos de la base de datos
include './obtener_campos.php';
$fields = obtener_campos($tabla, $conexion);

$fs = [];
$jpg = null;

// Estos son los campos de la tabla detalle_factura
foreach ($fields as $field) {
    $f=$_POST[$field];
    $fs[]=$f;
}

// Tomamos el id del repuesto con el que está relacionado (Campo REFERENCIA) haciendo una consulta
$sql_repuesto="SELECT Importe FROM repuestos WHERE Referencia = '$fs[2]';";
$resultado = $conexion->query($sql_repuesto);
if (!$resultado) {
    echo "Volver a la <a href='../index.php'>Página principal</a>";
    echo "<p class='error'>Error en la consulta de importe.</p>";
    echo "<p class='error'>Error: " . mysqli_error($conexion) . "</p>";
    exit();
}
// Con este producto, obtenemos el importe
$importe = (float)$resultado->fetch_assoc();


// Tomamos el id de la factura con el que está relacionado (Campo Numero_Factura) haciendo una consulta
$sql_factura="SELECT Base_Imponible, IVA FROM factura WHERE Numero_Factura = '$fs[1]';";
$resultado = $conexion->query($sql_factura);
if (!$resultado) {
    echo "Volver a la <a href='../index.php'>Página principal</a>";
    echo "<p class='error'>Error en la consulta de factura.</p>";
    echo "<p class='error'>Error: " . mysqli_error($conexion) . "</p>";
    exit();
}
// Obtenemos la base imponible y el IVA de la factura
$factura = [];
$factura = $resultado->fetch_assoc();

$base_imponible = (float)$factura["Base_Imponible"];
$IVA = (int)$factura["IVA"];

// Le sumamos el importe del producto
$base_imponible += $importe;
// Actualizamos la base imponible de la factura
$sql_factura="UPDATE factura SET Base_Imponible='$base_imponible' WHERE Numero_Factura='$fs[1]';";
$resultado = $conexion->query($sql_factura);
// Actualizamos el total de la factura
$total = (($base_imponible * (float)$IVA/100) + $base_imponible) * $_POST["Unidades"];
$sql_factura="UPDATE factura SET Total='$total' WHERE Numero_Factura='$fs[1]';";
$resultado = $conexion->query($sql_factura);

// Ejecutamos la consulta de insertar la linea de factura
// Preparamos la consulta
$consulta="INSERT INTO $tabla VALUES (";
foreach ($fs as $f) {
    $consulta.="'$f',";
}

if ($jpg!=null) {
    $consulta.="'$jpg',";
}
// Le quitamos la ultima coma
if ($f == end($fs)) {
    $consulta = substr($consulta, 0, -1);
}

$consulta.=");";

$resultado = $conexion->query($consulta);
if (!$resultado) {
    echo "Volver a la <a href='../index.php'>Página principal</a>";
    echo "<p class='error'>Error en la consulta.</p>";
    echo "<p class='error'>Error: " . mysqli_error($conexion) . "</p>";
    exit();
}

header("Location: ../editar.php?tabla=factura&id=".$fs[1]);

