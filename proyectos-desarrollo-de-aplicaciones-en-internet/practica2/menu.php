<?php
include("./scripts/pageBuilder.php");
include("./scripts/seguridad.php");
?>
<!DOCTYPE html>
<html lang="es">
<head>
    <?php buildMeta(); ?>
    <title>Menu</title>
    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="bootstrap-5.3.0/css/bootstrap.min.css">
    <!--Bootstrap icons-->
    <link rel="stylesheet" href="bootstrap-5.3.0/icons/font/bootstrap-icons.css">

    <!--Favicon-->
    <link rel="icon" href="resources/icons/favicon.png" type="image/png" sizes="16x16">

    <!--Mi estilo-->
    <link rel="stylesheet" href="css/general.css">
    <link rel="stylesheet" href="css/menu.css">
</head>
<body>
    <?php buildHeader(); ?>
    <main>
        <h1 id="page-title">Panel de administrador</h1>
        <div class="container">
            <div id="admin-options-wrapper" class="row text-center">
                <div class="admin-option col-md-4">
                    <a href="administrar.php?tabla=clientes" class="admin-option">
                        <div class="card-animated">
                            <i class="bi bi-person-circle"></i>
                            <h2>Administrar clientes</h2>
                        </div>
                    </a>
                </div>

                <div class="admin-option col-md-4">
                    <a href="administrar.php?tabla=embarcaciones" class="admin-option">
                        <div class="card-animated">
                            <i class="bi bi-life-preserver"></i>
                            <h2>Administrar embarcaciones</h2>
                        </div>
                    </a>
                </div>

                <div class="admin-option col-md-4">
                    <a href="administrar.php?tabla=repuestos" class="admin-option">
                        <div class="card-animated">
                            <i class="bi bi-wrench-adjustable-circle"></i>
                            <h2>Administrar repuestos</h2>
                        </div>
                    </a>
                </div>

                <div class="admin-option col-md-4">
                    <a href="administrar.php?tabla=factura" class="admin-option">
                        <div class="card-animated">
                            <i class="bi bi-receipt"></i>
                            <h2>Administrar facturas</h2>
                        </div>
                    </a>
                </div>
            </div>
            
        </div>
    </main>
    <?php buildFooter(); ?>
</body>
</html>
