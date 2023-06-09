<?php
    foreach(glob("./temporales/*") as $nombrearchivo)
    {
        unlink($nombrearchivo);
    }
?>