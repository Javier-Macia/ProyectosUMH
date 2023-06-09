
<!-- #include file=./scripts/includes/pagebuilder.inc -->
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="./favicon/favicon.ico" type="image/x-icon">

    <title>Registro</title>
    
    <!-- Estilos -->
    <link rel="stylesheet" href="./style/general.css">
    <link rel="stylesheet" href="./style/login-register.css">

    <!-- Fuentes-->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Lato&family=Work+Sans&display=swap" rel="stylesheet">

    <!--Conexion a fontawesome-->
    <script src="https://kit.fontawesome.com/2c36e9b7b1.js" crossorigin="anonymous"></script>
</head>

<body>
    <% build_header() %>

    <main class="login-register-main">
        <div class="login-register-wrapper">
            <div id="login-welcome-wrapper">
                <h1>Registro</h1>
                <p>¡Te damos la bienvenida! Introduce tus datos para crear tu perfil.</p>
            </div>

            <form action="" method="post" class="login-register-form">
                <div class="form-section-wrapper">
                    <div class="input-wrapper">
                        <label for="usuario"> <i class="fa fa-user"></i> </label>
                        <input type="text" name="usuario" id="usuario" placeholder="Usuario">
                    </div>
                    <div class="input-wrapper">
                        <label for="pass"> <i class="fa fa-lock"></i> </label>
                        <input type="password" name="pass" id="pass" placeholder="Contraseña">
                    </div>
                </div>
                <div class="form-section-wrapper" id="submit-btn-wrapper">
                    <input type="submit" name="submit" class="btn btn-primary" id="submit" value="Registrarse">
                </div>
            </form>

            <div id="opposite-action-wrapper">
                <p> ¿Ya tienes cuenta? </p>
                <button class="btn btn-terciary" id="opposite-action-btn" onclick=" ">Inicia sesión</button>

            </div>
        </div>
    </main>

    <% build_footer() %>

</body>
</html>

<script src="./js/headerAnimations.js"></script>
<script type="text/javascript">
    document.getElementById("opposite-action-btn").onclick = function () {
        location.href = "login.asp";
    };
</script>