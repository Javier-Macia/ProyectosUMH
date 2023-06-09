<!DOCTYPE html>
<html lang="es">
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <title>Datos del cliente</title>

    </head>
    <body>
        <h1> Datos del cliente</h1>
        <?php foreach ($clientes as $cliente){
            echo $cliente->Id_Cliente." <br>";
            echo $cliente->DNI." <br>";
            echo $cliente->Nombre." ".$cliente->Apellido1." ". $cliente->Apellido2 ."<br>";
            echo $cliente->Direccion." <br>";
            echo $cliente->CP." <br>";
            echo $cliente->Poblacion." <br>";
            echo $cliente->Provincia." <br>";
            echo $cliente->Telefono." <br>";
            echo $cliente->Email." <br>";
            echo "<br>";
        }?>
    </body>
</html>