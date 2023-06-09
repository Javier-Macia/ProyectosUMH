<?php include 'scripts/pageBuilder.php'; ?>
<!DOCTYPE html>
<html>
    <head>
        <?php buildMeta(); ?>
        <title>Login</title>
        <!-- Bootstrap CSS -->
        <link rel="stylesheet" href="bootstrap-5.3.0/css/bootstrap.min.css">
        <!--Bootstrap icons-->
        <link rel="stylesheet" href="bootstrap-5.3.0/icons/font/bootstrap-icons.css">
        
        <!--Favicon-->
        <link rel="icon" href="resources/icons/favicon.png" type="image/png" sizes="16x16">

        <!--Mi estilo-->
        <link rel="stylesheet" href="css/general.css">
        <link rel="stylesheet" href="css/login.css">
    </head>
    <body>
        <?php buildHeader();?>
        <main>
            <h1 id="page-title" class="text-center">Login</h1>
            <div id="form-wrapper">
                <form id="login-form" action="scripts/control.php" method="POST">
                    <div class="input-wrapper">
                        <label for="usuario"><i class="bi bi-person"></i></label>
                        <input type="text" id="usuario" placeholder="Usuario" name="usuario" required>
                    </div>
                    <div class="input-wrapper">
                        <label for="pass"><i class="bi bi-lock"></i></label>
                        <input type="password" id="password" placeholder="Contraseña" name="password" required>
                    </div>
                    <input class="primary-button" type="submit" id="enviar" value="Enviar"> 
                </form>
            </div>
        </main>
        <?php buildFooter();?>

    </body>
</html>