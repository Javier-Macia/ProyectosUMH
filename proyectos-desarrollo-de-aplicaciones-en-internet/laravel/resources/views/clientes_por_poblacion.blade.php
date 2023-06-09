<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>Listado de clientes por población</title>
    </head>

    <body>
        <?php
        $clientes = App\Cliente::where([
                        ['Poblacion', '=', $poblacion ],
                        ['Apellido1', '<>', 'Moreno'],
                    ])
                    ->orderBy('Nombre', 'desc')
                    ->take(2)
                    ->get();

        echo "<h1>Listado de clientes de $poblacion</h1>";
        foreach ($clientes as $cliente) {
            echo $cliente->Nombre . " " . $cliente->Apellido1;

            echo "<br>";
        }
        ?>
    </body>
</html>