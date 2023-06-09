<!DOCTYPE html>
<html lang="es">
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <title>Actualizar la embarcación</title>

    </head>
    <body>
        <h1> Actualización de la embarcación</h1>
        <?php 
        if ($actualizado == false){
            echo "No existe la embarcación de id: '".$id."' o no se ha podido actualizar.";
        }
        else{
            echo "Embarcación de id: '".$id."' actualizada al año ".$anyo.".";
        }
        ?>
    </body>
</html>