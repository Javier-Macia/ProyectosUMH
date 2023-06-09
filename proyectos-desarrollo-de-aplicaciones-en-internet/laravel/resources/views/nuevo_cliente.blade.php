<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>Insertar cliente</title>
    </head>

    <body>
        <?php
        use App\Cliente;
        $cliente = new Cliente;
        $cliente->DNI = $dni;
        $cliente->Nombre = $nombre;
        $cliente->Apellido1 = $apellido1;
        $cliente->Apellido2 = "Apellido2";
        $cliente->Direccion = "Direccion";
        $cliente->CP = "CP";
        $cliente->Poblacion = "Poblacion";
        $cliente->Provincia = "Provincia";
        $cliente->Telefono = "Telefono";
        $cliente->Email = "Email";
        $cliente->Fotografia = "Fotografia";
        $cliente->save();

        echo "<h2> Inserción de cliente</h2>";
        echo "Cliente insertado con id: " . $cliente->id. "($nombre $apellido1)";
        ?>
    </body>
</html>