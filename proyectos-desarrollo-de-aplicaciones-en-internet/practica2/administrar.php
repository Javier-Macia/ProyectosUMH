<?php
include  './scripts/pageBuilder.php';
include './scripts/seguridad.php';
include './scripts/conexionMySQLi.php';
include './scripts/eliminar_temporales.php';

$tabla = $_GET["tabla"];
$tablas = [
    "clientes", "embarcaciones", "repuestos", "factura"
];

// Si la tabla no existe, redirigimos al menu
if (!in_array($tabla, $tablas)) {
    header("Location: menu.php");
}


include './scripts/obtener_campos.php';
$fields = obtener_campos($tabla, $conexion);
$types = obtener_tipos($tabla, $conexion);


?>
<!DOCTYPE html>
<html lang="es">
<head>
    <?php buildMeta(); ?>
    <title>Administrar <?= $tabla ?></title>

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="bootstrap-5.3.0/css/bootstrap.min.css">
    <!--Bootstrap icons-->
    <link rel="stylesheet" href="bootstrap-5.3.0/icons/font/bootstrap-icons.css">

    <!--Favicon-->
    <link rel="icon" href="resources/icons/favicon.png" type="image/png" sizes="16x16">

    <!--Mi estilo-->
    <link rel="stylesheet" href="css/general.css">
    <link rel="stylesheet" href="css/administrar.css">
</head>
<body>
    <?php buildHeader(); ?>
    <main>
        <h1 id="page-title">Administrar <?= $tabla ?></h1>
        <div class="table-responsive">

        <?php
                // Código para mostrar filtros en la búsqueda de las tablas
                if ($tabla == $tablas[1] || $tabla == $tablas[3]){
                    echo "<div class='filters-container'>";
                    echo "  <form action='administrar.php'  method='GET'>";
                    echo "    <h3>Filtrar</h3>";
                    echo "    <div class='filters'>";
                    // Si la tabla es embarcaciones
                    if ($tabla == $tablas[1]){
                        echo "      <input type='hidden' name='tabla' value='$tabla'>";
                        echo "      <div class='filter'>";
                        echo "          <label for='Longitud'>Longitud</label>";
                        echo "          <input type='text' name='Longitud' id='Longitud'>";
                        echo "      </div>";
                        echo "      <div class='filter'>";
                        echo "          <label for='Potencia'>Potencia</label>";
                        echo "          <input type='number' name='Potencia' id='Potencia'>";
                        echo "      </div>";
                        echo "      <div class='filter'>";  
                        echo "          <label for='Motor'>Motor</label>";
                        echo "          <select name='Motor' id='Motor'>";
                        echo "              <option selected value=''>--</option>";
                        echo "              <option value='Diesel'>Diesel</option>";
                        echo "              <option value='Eléctrico'>Eléctrico</option>";
                        echo "              <option value='Gas'>Gas</option>";
                        echo "              <option value='Gasolina'>Gasolina</option>";
                        echo "              <option value='Híbrido'>Híbrido</option>";
                        echo "          </select>";
                        echo "      </div>";
                        echo "      <div class='filter'>";
                        echo "          <label for='Año'>Año</label>";
                        echo "          <input type='number' name='Año' id='Año' min=1900 max=".date("Y").">";
                        echo "      </div>";
                        echo "      <div class='filter'>";
                        echo "          <label for='Material'>Material</label>";
                        echo "          <select name='Material' id='Material'>";
                        echo "              <option selected value=''>--</option>";
                        echo "              <option value='Acero'>Acero</option>";
                        echo "              <option value='Aluminio'>Aluminio</option>";
                        echo "              <option value='Carbono'>Carbono</option>";
                        echo "              <option value='Fibra de Vidrio'>Fibra de Vidrio</option>";
                        echo "              <option value='Polietileno'>Polietileno</option>";
                        echo "          </select>";
                        echo "      </div>";

                    }
                    // Si la tabla es factura
                    if ($tabla == $tablas[3]){
                        /**
                         * Listado de Facturas Emitidas entre dos fechas.
                         * Listado de Facturas pendientes de Pago.
                         * Listado de Facturas de un cliente determinado. La aplicación deberá permitirnos
                         * seleccionar un cliente de la BD y nos mostrará un listado de todas sus facturas.
                         */
                        echo "      <input type='hidden' name='tabla' value='$tabla'>";
                        echo "      <div class='filter'>";
                        echo "          <label for='Fecha'>Fecha de emisión</label>";
                        echo "          <input type='date' name='Fecha' id='Fecha'>";
                        echo "      </div>";
                        echo "      <div class='filter'>";
                        echo "          <label for='Fecha2'>Fecha de pago</label>";
                        echo "          <input type='date' name='Fecha2' id='Fecha2'>";
                        echo "      </div>";
                        echo "      <div class='filter'>";
                        echo "          <label for='Cliente'>Cliente</label>";
                        echo "          <select name='Cliente' id='Cliente'>";
                        echo "              <option selected value=''>--</option>";
                        $query = "SELECT * FROM clientes";
                        $result = mysqli_query($conexion, $query);
                        while ($row = mysqli_fetch_assoc($result)) {
                            echo "<option value='".$row['Id_Cliente']."'>".$row['Nombre']."</option>";
                        }
                        echo "          </select>";
                        echo "      </div>";
                    }

                    echo "        <div class='filtrar-wrapper'>";
                    echo "            <label for='submit-filtro'><i class='bi bi-search'></i></label>";
                    echo "            <input type='submit' id='submit-filtro' value='Filtrar'>";
                    echo "        </div>";
                    echo "    </div>";
                    echo "  </form>";
                    echo "  <form id='limpiar-filtros' action='administrar.php' method='GET'>";
                    echo "      <input type='hidden' name='tabla' value='$tabla'>";
                    echo "      <input type='submit' value='Limpiar filtros'>";
                    echo "  </form>";
                    echo "</div>";
                }
                /**
                 * TODO: Problema con el formulario de filtrado, corta al de borrar
                 */
                ?>


            <form method=post action="scripts/eliminar.php?tabla=<?=$tabla?>">
                <input type=hidden name=tabla value="<?=$tabla?>">
                <div class="btn-container">
                    <a href="agregar.php?tabla=<?= $tabla ?>" class="primary-button btn-admin">Agregar un nuevo registro</a>
                    <input type=submit class="secondary-button btn-admin" value="Eliminar registros seleccionados">
                    <input type=reset  class="secondary-button btn-admin" value="Deseleccionar todos los registros">
                </div>

                <table class="TSW-table">
                    <thead>
                        <tr>
                            <?php 
                            foreach ($fields as $field) {
                                echo "<th>$field</th>";
                            }
                            ?>
                        <th>Eliminar</th>
                        </tr>
                    </thead>
                    <tbody>
                        <?php
                        $sql = "SELECT * FROM $tabla ";
                        // TABLA DE EMBARCACIONES
                        if ($tabla == $tablas[1] && isset($_GET['Longitud']) && isset($_GET['Potencia']) && isset($_GET['Motor']) && isset($_GET['Año']) && isset($_GET['Material'])){
                            if ($_GET['Longitud'] != ""){
                                // Longitud es un decimal y hay que convertirlo primero a string
                                if (strpos($sql, "WHERE") == false){
                                    $sql = $sql . " WHERE CONVERT(Longitud,CHAR) = CONVERT(" . $_GET['Longitud'] . ", CHAR)";
                                } else {
                                    $sql = $sql . " AND CONVERT(Longitud,CHAR) = CONVERT(" . $_GET['Longitud'] . ", CHAR)";
                                }
                            }
                            if ($_GET['Potencia'] != ""){
                                if (strpos($sql, "WHERE") == false){
                                    $sql = $sql . " WHERE Potencia = " . $_GET['Potencia'];
                                } else {
                                    $sql = $sql . " AND Potencia = " . $_GET['Potencia'];
                                }
                            }
                            if ($_GET['Motor'] != ""){
                                if (strpos($sql, "WHERE") == false){
                                    $sql = $sql . " WHERE Motor = '" . $_GET['Motor'] . "'";
                                } else {
                                    $sql = $sql . " AND Motor = '" . $_GET['Motor'] . "'";
                                }
                            }
                            if ($_GET['Año'] != ""){
                                if (strpos($sql, "WHERE") == false){
                                    $sql = $sql . " WHERE Año = " . $_GET['Año'];
                                } else {
                                    $sql = $sql . " AND Año = " . $_GET['Año'];
                                }
                            }
                            if ($_GET['Material'] != ""){
                                if (strpos($sql, "WHERE") == false){
                                    $sql = $sql . " WHERE Material = '" . $_GET['Material'] . "'";
                                } else {
                                    $sql = $sql . " AND Material = '" . $_GET['Material'] . "'";
                                }
                            }
                        }

                        // TABLA DE LAS FACTURAS
                        if ($tabla == $tablas[3] && isset($_GET['Fecha']) && isset($_GET['Fecha2']) && isset($_GET['Cliente'])){
                            if ($_GET['Cliente'] != ""){
                                // Seleccionamos relacionamos el id del cliente de la tabla de embarcaciones
                                // con la matricula de la factura
                                $sql = $sql . " INNER JOIN embarcaciones ON embarcaciones.Matricula = factura.Matricula";
                                if (strpos($sql, "WHERE") == false){
                                    $sql = $sql . " WHERE embarcaciones.Id_Cliente = " . $_GET['Cliente'];
                                } else {
                                    $sql = $sql . " AND embarcaciones.Id_Cliente = " . $_GET['Cliente'];
                                }
                            }
                            if ($_GET['Fecha'] != ""){
                                if (strpos($sql, "WHERE") == false){
                                    $sql = $sql . " WHERE Fecha_Emision >= '" . $_GET['Fecha'] . "'";
                                } else {
                                    $sql = $sql . " AND Fecha_Emision >= '" . $_GET['Fecha'] . "'";
                                }
                            }
                            if ($_GET['Fecha2'] != ""){
                                if (strpos($sql, "WHERE") == false){
                                    $sql = $sql . " WHERE Fecha_Pago <= '" . $_GET['Fecha2'] . "'";
                                } else {
                                    $sql = $sql . " AND Fecha_Pago <= '" . $_GET['Fecha2'] . "'";
                                }
                            }
                        }


                        $sql = $sql . " ORDER BY $fields[0] ASC";
                        $consulta = $conexion->prepare($sql);
                        $consulta->execute();
                        $resultado = $consulta->get_result();
                        $rows = $resultado->fetch_all(MYSQLI_ASSOC);

                        foreach ($rows as $row){
                            echo "<tr>";
                            foreach ($fields as $field) {
                                // Si el campo es una imagen, nos vamos a los archivos temporales y la mostramos
                                if ($field == "Fotografia") {
                                    $foto=$row['Fotografia'];
                                    
                                    $imagen=basename(tempnam(getcwd()."/temporales","temp"));
                                    $imagen.=".jpg";
                                    $fichero=fopen("./temporales/".$imagen,"w");
                                    // escribimos en el fichero el contenido del campo de la base de datos
                                    fwrite($fichero,$foto);
                                    //cerramos el fichero
                                    fclose($fichero);
                                    echo "<td >
                                        <div href=temporales/$imagen>
                                            <img src=temporales/$imagen width=84 border=0>
                                        </div>
                                    </td>";
                                }
                                else{
                                    echo "<td onClick='editarRegistro( \"". $tabla ."\", \"" . $row[$fields[0]] . "\")' >$row[$field]</td>";
                                }
                                
                            }
                            echo "
                                <td class='delete-registro'>
                                    <label><input type=checkbox name=borrar[] value=".$row[$fields[0]]."></label>
                                </td>";
                            echo "</tr>";
                        }
                        ?>
                    </tbody>
                </table>
                </form>
        </div>
    </main>
    <?php buildFooter(); ?>
    
</body>
</html>

<script src="js/administrar.js"></script>