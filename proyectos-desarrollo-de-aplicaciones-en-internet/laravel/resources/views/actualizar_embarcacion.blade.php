<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>Modificar cliente</title>
    </head>

    <body>
        <?php
        App\Cliente::where('DNI', $dni)
                        ->update(['Nombre' => $nombre, ]);
        echo "Se ha modificado el registro correctamente."
        ?>
    </body>
</html>