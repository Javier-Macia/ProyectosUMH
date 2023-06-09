<?php include 'scripts/pageBuilder.php'; ?>
<!DOCTYPE html>
<html lang="es">
<head>
    <?php buildMeta(); ?>
    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="bootstrap-5.3.0/css/bootstrap.min.css">
    <!--Bootstrap icons-->
    <link rel="stylesheet" href="bootstrap-5.3.0/icons/font/bootstrap-icons.css">

    <!--Favicon-->
    <link rel="icon" href="resources/icons/favicon.png" type="image/png" sizes="16x16">

    <!--Mi estilo-->
    <link rel="stylesheet" href="css/general.css">
    <link rel="stylesheet" href="css/index.css">

    
    <title>The Sailor's Wrench</title>
</head>
<body>
    <?php buildHeader(); ?>
    <main>
        <h1 id="page-title" class="text-center">The Sailor's Wrench</h1>
        <div id="presentation-div" class="row"></div>
        <section class="container p-4">
            <h2 class="text-center">Tu taller de embarcaciones</h2>
            <article class="row">
                <div class="col-md-4">
                    <div class="col-md-12 card p-4">
                        <p class="icon-text"> <i class="bi bi-brightness-alt-high"></i> </p>
                        <h3 class="text-center">Objetivo</h3>
                        <p>Lorem, ipsum dolor sit amet consectetur adipisicing elit. Suscipit velit excepturi cumque illo. Autem at odio iure repellendus blanditiis, nesciunt excepturi eaque voluptatibus ducimus cumque sunt saepe velit, unde sint?</p>
                    </div>
                </div>
                <div class="col-md-4">
                    <div class="col-md-12 card p-4">
                        <p class="icon-text"> <i class="bi bi-person-hearts"></i> </p>
                        <h3 class="text-center">Por qué elegirnos</h3>
                        <p>Lorem, ipsum dolor sit amet consectetur adipisicing elit. Suscipit velit excepturi cumque illo. Autem at odio iure repellendus blanditiis, nesciunt excepturi eaque voluptatibus ducimus cumque sunt saepe velit, unde sint?</p>
                    </div>
                </div>
                <div class="col-md-4">
                    <div class="col-md-12 card p-4">
                        <p class="icon-text"> <i class="bi bi-trophy"></i> </p>
                        <h3 class="text-center">Premio 2023</h3>
                        <p>Lorem, ipsum dolor sit amet consectetur adipisicing elit. Suscipit velit excepturi cumque illo. Autem at odio iure repellendus blanditiis, nesciunt excepturi eaque voluptatibus ducimus cumque sunt saepe velit, unde sint?</p>
                    </div>
                </div>
            </article>
        </section>

    </main>

    <?php buildFooter(); ?>
</body>
</html>