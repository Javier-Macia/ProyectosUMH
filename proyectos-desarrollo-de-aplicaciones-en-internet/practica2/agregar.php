<?php
include  './scripts/pageBuilder.php';
include './scripts/seguridad.php';
include './scripts/conexionMySQLi.php';

$tabla = $_GET["tabla"];
$tablas = [
    "clientes", "embarcaciones", "repuestos", "factura", "detalle_factura"
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
    <title>Agregar <?= $tabla ?></title>

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="bootstrap-5.3.0/css/bootstrap.min.css">
    <!--Bootstrap icons-->
    <link rel="stylesheet" href="bootstrap-5.3.0/icons/font/bootstrap-icons.css">

    <!--Favicon-->
    <link rel="icon" href="resources/icons/favicon.png" type="image/png" sizes="16x16">

    <!--Mi estilo-->
    <link rel="stylesheet" href="css/general.css">
    <link rel="stylesheet" href="css/agregar.css">
</head>
<body>
    <?php buildHeader(); ?>
    <main>
        <h1 id="page-title">Agregar <?= $tabla ?></h1>
        <form id="form-add" method=post action=<?php
        if ($tabla == $tablas[4]){
            echo "scripts/insertar_detalle_factura.php";
        } else {
            echo "scripts/insertar.php";
        }
        ?> 
        enctype="multipart/form-data">
            <input type="hidden" id="tabla" name="tabla" value="<?= $tabla ?>">
            
            <div id="form-content" class="row">
                <?php
                foreach ($fields as $field){
                    /*
                    * Comportamiento especial para los USUARIOS.
                    * Su id_cliente es autoincremental
                    */
                    if ($field == "Id_Cliente" && $tabla == $tablas[0]) {
                        // Se obtiene el ultimo id_cliente
                        $query = "SELECT MAX(Id_Cliente) AS Id_Cliente FROM clientes";
                        $result = $conexion->query($query);
                        $row = $result->fetch_assoc();
                        $id_cliente = $row["Id_Cliente"];
                        // Se le suma 1
                        $id_cliente++;
                        // Se pone el campo 
                        echo "<input hidden required type='text' name='$field' id='$field' value='$id_cliente' readonly>";

                        continue;
                    }


                    /**
                    * Comportamiento especial para las EMBARCACIONES.
                    * El dueño del barco es un select y sus opciones son los ids de los clientes
                    */
                    if ($field == "Id_Cliente" && $tabla == $tablas[1]){
                        // Se obtienen los ids de los clientes
                        $query = "SELECT Id_Cliente FROM clientes";
                        $result = $conexion->query($query);
                        // Se crea el select
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field</label>";
                        echo "  <select required name='$field' id='$field'>";
                        while ($row = $result->fetch_assoc()) {
                            echo "<option value='".$row["Id_Cliente"]."'>".$row["Id_Cliente"]."</option>";
                        }
                        echo "  </select>";
                        echo "</div>";

                        continue;
                    }
                    /** El tipo de motor es también un select con los valores Diesel,
                     *  Eléctrico, Gas, Gasolina o Híbrido
                    */
                    if ($field == "Motor" && $tabla == $tablas[1]){
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field</label>";
                        echo "  <select required name='$field' id='$field'>";
                        echo "      <option value='Diesel'>Diesel</option>";
                        echo "      <option value='Eléctrico'>Eléctrico</option>";
                        echo "      <option value='Gas'>Gas</option>";
                        echo "      <option value='Gasolina'>Gasolina</option>";
                        echo "      <option value='Híbrido'>Híbrido</option>";
                        echo "  </select>";
                        echo "</div>";
                        continue;
                    }
                    /** Lo mismo ocurre con el material, Acero, Aluminio, Carbono, Fibra de Vidrio,Polietileno */
                    if ($field == "Material" && $tabla == $tablas[1] ){
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field</label>";
                        echo "  <select required name='$field' id='$field'>";
                        echo "      <option value='Acero'>Acero</option>";
                        echo "      <option value='Aluminio'>Aluminio</option>";
                        echo "      <option value='Carbono'>Carbono</option>";
                        echo "      <option value='Fibra de Vidrio'>Fibra de Vidrio</option>";
                        echo "      <option value='Polietileno'>Polietileno</option>";
                        echo "  </select>";
                        echo "</div>";
                        continue;
                    }
                    /**
                     * EXTRA: el año tiene que ser un número entre 1900 y la fecha de hoy
                     */
                    if ($field == "Año" && $tabla == $tablas[1]){
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field</label>";
                        echo "  <input required type='number' name='$field' id='$field' min=1900 max=".date("Y").">";
                        echo "</div>";
                        continue;
                    }


                    /**
                     * Comportamiento especial para los REPUESTOS. 
                     * Para cada repuesto podrá especificar un Importe (en Euros) y un porcentaje de Ganancia que
                     * es el beneficio sobre el importe que obtendrá el taller al incluirlo en una reparación.
                    */
                    if ($field == "Importe" && $tabla == $tablas[2]){
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field (en euros)</label>";
                        echo "  <input required type='number' name='$field' id='$field' min=0>";
                        echo "</div>";
                        continue;
                    }

                    if ($field == "Ganancia" && $tabla == $tablas[2]){
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field (en porcentaje)</label>";
                        echo "  <input required type='number' name='$field' id='$field' min=0 max=100>";
                        echo "</div>";
                        continue;
                    }

                    /**
                     * Comportamiento especial para las FACTURAS.
                     * El matrícula de la embarcación es un select con las matrículas de las embarcaciones
                     */
                    if ($field == "Matricula" && $tabla == $tablas[3]){
                        // Se obtienen las matriculas de las embarcaciones
                        $query = "SELECT Matricula FROM embarcaciones";
                        $result = $conexion->query($query);
                        // Se crea el select
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field</label>";
                        echo "  <select required name='$field' id='$field'>";
                        while ($row = $result->fetch_assoc()) {
                            echo "<option value='".$row["Matricula"]."'>".$row["Matricula"]."</option>";
                        }
                        echo "  </select>";
                        echo "</div>";

                        continue;
                    }

                    // La mano de obra es un numero de horas
                    if ($field == "Mano_de_Obra" && $tabla == $tablas[3]){
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field (en horas)</label>";
                        echo "  <input required type='number' name='$field' id='$field' min=0>";
                        echo "</div>";
                        continue;
                    }

                    // El IVA va del 0 al 100
                    if ($field == "IVA" && $tabla == $tablas[3]){
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>% de $field</label>";
                        echo "  <input required type='number' name='$field' id='$field' min=0 max=100>";
                        echo "</div>";
                        continue;
                    }

                    // La base imponible está oculta
                    if ($field == "Base_Imponible" && $tabla == $tablas[3]){
                        echo "<input required hidden name='$field' id='$field' value='0'>";
                        continue;
                    }

                    // El total está oculto
                    if ($field == "Total" && $tabla == $tablas[3]){
                        echo "<input hidden name='$field' id='$field' value='0'>";
                        continue;
                    }

                    // El número de factura es un número que se incrementa automáticamente. Se coge el último número de factura y se incrementa en 1
                    if ($field == "Numero_Factura" && $tabla == $tablas[3]){
                        // Se obtiene el último número de factura
                        $query_2 = "SELECT Numero_Factura FROM factura ORDER BY Numero_Factura DESC LIMIT 1";
                        $result_2 = $conexion->query($query_2);
                        $ult_num = $result_2->fetch_assoc()["Numero_Factura"];
                        // Se incrementa en 1
                        $numero_factura = (int)$ult_num + 1;
                        // Se crea el input
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field</label>";
                        echo "  <input required type='number' name='$field' id='$field' value='$numero_factura' readonly>";
                        echo "</div>";

                        continue;
                    }
                    


                    /**
                     * Comportamiento especial para las LINEAS DE FACTURAS.
                     * El Id_Det_Factura se coge por GET y es un select
                     */
                    if ($field == "Numero_Factura" && $tabla == $tablas[4]){
                        // Se obtienen los ids de las facturas
                        $query = "SELECT Numero_Factura FROM $tablas[3]";
                        $result = $conexion->query($query);
                        // Se crea el select
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field</label>";
                        echo "  <select required name='$field' id='$field'>";
                        while ($row = $result->fetch_assoc()) {
                            echo "<option ";
                            if (isset($_GET["id_factura"])){
                                if ($_GET["id_factura"] == $row["Numero_Factura"]){
                                    echo "selected ";
                                }
                            }
                            echo "value='".$row["Numero_Factura"]."'>".$row["Numero_Factura"]."</option>";
                        }
                        echo "  </select>";
                        echo "</div>";

                        continue;
                    }

                    /**
                     * El Id_Detalle_Factura es readonly y autoincremental
                     */
                    if ($field == "Id_Det_Factura" && $tabla == $tablas[4]){
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field</label>";
                        echo "  <input readonly ";
                        // Consultamos la BD
                        $query = "SELECT MAX(Id_Det_Factura) as max FROM $tablas[4]";
                        $result = $conexion->query($query);
                        // Si hay resultados, se pone el valor siguiente
                        if ($result->num_rows > 0) {
                            $row = $result->fetch_assoc();
                            echo "value='".($row["max"]+1)."'";
                        }
                        else{
                            echo "value='1'";
                        }
                        echo " type='number' name='$field' id='$field'>";
                        echo "</div>";
                        continue;
                    }
                    /**
                     * Las referencias a los respuestos deben existir y se cogen por select
                     */
                    if ($field == "Referencia" && $tabla == $tablas[4]){
                        // Se obtienen las referencias de los respuestos
                        $query = "SELECT Referencia FROM $tablas[2]";
                        $result = $conexion->query($query);
                        // Se crea el select
                        echo "<div class='col-md-6'>";
                        echo "  <label for='$field'>$field</label>";
                        echo "  <select required name='$field' id='$field'>";
                        while ($row = $result->fetch_assoc()) {
                            echo "<option ";
                            if (isset($_GET["id_referencia"])){
                                if ($_GET["id_referencia"] == $row["Referencia"]){
                                    echo "selected ";
                                }
                            }
                            echo "value='".$row["Referencia"]."'>".$row["Referencia"]."</option>";
                        }
                        echo "  </select>";
                        echo "</div>";

                        continue;
                    }

                    
                    /**
                     * 
                     * COMPORTAMIENTO GENERAL
                     * 
                     */
                    echo "<div class='col-md-6'>";
                    echo "  <label for='$field'>$field</label>";
                    // Si es una imagen, se pone como tal
                    if (strpos($field, "Fotografia") !== false) {
                        echo "  <input required type='file' name='$field' id='$field'>";
                        echo "  <small class='form-text text-muted'>Solo se aceptan archivos .jpg</small>";
                    }else{
                        echo "  <input required type='";
                        // Dependiendo del tipo de dato, se pone el tipo de input
                        switch ($types[$field]) {
                            case "int":
                                echo "number";
                                break;
                            case "date":
                                echo "date";
                                break;
                            case "varchar":
                                echo "text";
                                break;
                            default:
                                echo "text";
                                break;
                        }

                        echo"' name='$field' id='$field'";
                        // Si es un campo numérico, no se permite que sea negativo
                        if ($types[$field] == "int") {
                            echo " min=0";
                        }
                        echo">";
                    }
                    echo "</div>";
                }
                ?>
            </div>

            <div class="row p-3 text-center" id="act-can-wrapper">
                <div class="col-md-6 text-end">
                    <button class="primary-button">Insertar</button>
                </div>

                <div class="col-md-6 text-start">
                    <a href="administrar.php?tabla=<?=$tabla?>" class="secondary-button">Cancelar</a>
                </div>
            </div>
        </form>
    </main>
    <?php buildFooter(); ?>
    
</body>
</html>

<script src="js/editar-agregar.js"></script>
<script src="js/facturas.js"></script>