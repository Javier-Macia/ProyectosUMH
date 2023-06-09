<?php
function obtener_campos($tabla, $conexion){
    // Obtenemos los campos de la tabla
    $sql_get_fields = "SHOW COLUMNS FROM $tabla";
    $result = mysqli_query($conexion, $sql_get_fields);
    $fields = [];
    while ($row = mysqli_fetch_assoc($result)) {
        $fields[] = $row["Field"];
    }
    return $fields;
}


function obtener_tipos($tabla, $conexion){
    // Obtenemos los types de los campos de la tabla
    $sql_get_types = "SELECT COLUMN_NAME, DATA_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '$tabla'";
    $result = mysqli_query($conexion, $sql_get_types);
    $types = [];
    while ($row = mysqli_fetch_assoc($result)) {
        $types[$row["COLUMN_NAME"]] = $row["DATA_TYPE"];
    }
    return $types;
}

?>