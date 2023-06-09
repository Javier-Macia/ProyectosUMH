<?php
/**
* CÓDIGO DE LAS LÍNEAS DE FACTURA
* ÚNICAMENTE PARA "AGREGAR.PHP"
*/
// Si es una factura, se muestran nuevos campos de LINEA DE CLIENTE, y se nos permite añadir nuevas líneas

if ($tabla == $tablas[3]){
    // Si existe el id, es que estamos editando una factura
    if (!isset($_GET["id"])) {
        $id = null;
    }
    
    echo "<div class='col-md-12 text-center p-4'>";
    echo "  <button class='primary-button' onClick='agregarLinea(\"$id\")'>Añadir línea</button>";
    echo "</div>";
    
    if ($id != null) {
        
        // Seleccionamos las líneas de factura de la factura que tenemos
        $query_2 = "SELECT * FROM detalle_factura WHERE Numero_Factura = $id ORDER BY Id_Det_Factura ASC";
        $result_2 = mysqli_query($conexion, $query_2);
        if ($result_2) {
            $num_rows = mysqli_num_rows($result_2);
        }else{
            $num_rows = 0;
        }
        
        if ($num_rows > 0) {
            echo "<h4 class='p-2'> Líneas de factura</h4>";
            echo "<table class='TSW-table'>";
            echo "  <tr>";
            echo "      <th>Id Detalle Factura</th>";
            echo "      <th>Id Factura</th>";
            echo "      <th>Referencia</th>";
            echo "      <th>Unidades</th>";
            echo "      <th>Modificar</th>";
            echo "  </tr>";
            while ($linea = mysqli_fetch_assoc($result_2)) {
                echo "  <tr>";
                echo "    <td>". $linea["Id_Det_Factura"] ."</td>";
                echo "    <td>". $linea["Numero_Factura"] ."</td>";
                echo "    <td>". $linea["Referencia"]."</td>";
                echo "    <td>". $linea["Unidades"] ."</td>";
                echo "    <td>"; // Boton de modificar
                echo "      <form action='editar.php?tabla=detalle_factura&id=". $linea["Id_Det_Factura"]. " method='post'>";
                echo "          <input type='hidden' name='tabla' value='detalle_factura'>";
                echo "          <input type='hidden' name='id' value='". $linea["Id_Det_Factura"] ."'>";
                echo "          <input type='hidden' name='numero_factura' value='". $linea["Numero_Factura"] ."'>";
                echo "          <input type='hidden' name='referencia' value='". $linea["Referencia"] ."'>";
                echo "          <input type='hidden' name='unidades' value='". $linea["Unidades"] ."'>";
                echo "          <input type='submit' id='submit-detalle-factura-". $linea["Id_Det_Factura"] ."' name='submit-detalle-factura' class='hidden'>";
                echo "          <label class='edit-linea-factura' for='submit-detalle-factura-". $linea["Id_Det_Factura"] ."'><i class='bi bi-pencil-square'></i></label>";
                echo "      </form>";
                echo "    </td>";
                echo "  </tr>";
            }
            echo "</table>";
        }
    }
            
    
}

?>