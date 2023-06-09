<?php
include  './pageBuilder.php';
include './seguridad.php';
include './conexionMySQLi.php';

$tabla=$_POST["tabla"];
if (empty($tabla)) {
    header("Location: ../administrar.php");
}
// Leemos los campos de la base de datos
include './obtener_campos.php';
$fields = obtener_campos($tabla, $conexion);

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
    }else{
        $f=$_POST[$field];
        $fs[]=$f;
    }
}



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

// Ejecutamos la consulta
$resultado = $conexion->query($consulta);
if (!$resultado) {
    echo "Volver a la <a href='index.php'>Página principal</a>";
    echo "<p class='error'>Error en la consulta.</p>";
    echo "<p class='error'>Error: " . mysqli_error($conexion) . "</p>";
    exit();
}

if ($tabla == "factura"){
    // Redirigimos a la página de editar factura
    header("Location: ../editar.php?tabla=factura&id=".$fs[0]);
}
else if ($tabla == "detalle_factura"){
   header("Location: ../editar.php?tabla=factura&id=".$fs[1]);
}
else {
    header("Location: ../administrar.php?tabla=$tabla");
}
echo "<br><a href='../administrar.php?tabla=".$tabla."'>Volver al Inicio</a>";

