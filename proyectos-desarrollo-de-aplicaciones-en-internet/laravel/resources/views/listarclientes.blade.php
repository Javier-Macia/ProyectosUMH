<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>Listado de Clientes</title>
    </head>

    <body>
        <?php
        $clientes = App\Cliente::all();

        foreach ($clientes as $cliente) {
            echo $cliente->Nombre;
            echo "<br>";
        }
        ?>
    </body>
</html>