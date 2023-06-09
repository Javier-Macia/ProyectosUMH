<?php
include  './pageBuilder.php';
include './seguridad.php';
include './conexionMySQLi.php';

$tabla=$_POST["tabla"];
$id=$_POST["id"];
// Leemos los campos de la base de datos
include './obtener_campos.php';

$fields = obtener_campos($tabla, $conexion);
$types = obtener_tipos($tabla, $conexion);

$fs = [];
$jpg = null;

foreach ($fields as $field) {
    if (strpos($field, "Fotografia") !== false) {
        // Si existe el campo Fotografia, lo tratamos
        if (is_uploaded_file($_FILES['Fotografia']['tmp_name'])){
            $foto=$_FILES['Fotografia']['tmp_name'];
            // Tratamiento necesario para introducir la imagen en la base de datos
            $fotografia=imagecreatefromjpeg($foto);
            ob_start(); // abrimos el buffer interno
            // obtenemos el fichero jpg-binario del buffer y lo almacena en la variable jpg
            imagejpeg($fotografia, null, 100);
            $jpg=ob_get_contents();
            //cerramos el buffer
            // preparamos la variable para usarla en una consulta sql
            ob_end_clean();
            $intermedio = addslashes(trim($jpg));
            $jpg=str_replace('##','\##',$intermedio);
        }
    }
    else if((strpos($field, "Total") !== false) && ($tabla == "factura")){
        // Si la tabla es factura, el total se calcula
        $f = (($_POST["Base_Imponible"] * (float)$_POST["IVA"] / 100) + $_POST["Base_Imponible"]);
        $fs[] = $f;
    }
    else{
        $f=$_POST[$field];
        $fs[]=$f;
    }
}


// Preparamos la consulta
$consulta = "UPDATE $tabla SET ";
for ($i = 0; $i < count($fields); $i++) {
    $consulta .= "$fields[$i] = '$fs[$i]',";
}

if ($jpg!=null) {
    $consulta.="Fotografia = '$jpg',";
}
// Le quitamos la ultima coma
if ($f == end($fs)) {
    $consulta = substr($consulta, 0, -1);
}

$consulta .= " WHERE $fields[0] = '$id'";


// Ejecutamos la consulta
$resultado = $conexion->query($consulta);

if (!$resultado) {
    echo "Volver a la <a href='index.php'>Página principal</a>";
    echo "<p class='error'>Error en la consulta.</p>";
    echo "<p class='error'>Error: " . mysqli_error($conexion) . "</p>";
    exit();
}

if ($tabla == "detalle_factura"){
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

    header("Location: ../editar.php?tabla=factura&id=" . $_POST["Numero_Factura"]);
}
else {
    header("Location: ../administrar.php?tabla=$tabla");
}
?>

