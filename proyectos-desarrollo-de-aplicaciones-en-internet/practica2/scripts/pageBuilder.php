<?php
    session_start();
    function buildHeader() {
        echo "
        <header>
            <div class='row'>
                <div id='logo-wrapper' class='col-md-4 col-sm-12 text-center'>
                    <a id='logo' href='index.php'>The Sailor's Wrench</a>
                </div>

                <div id='header-options-wrapper'class='col-md-8 col-sm-12 text-center'>
                    <label id='menu-icon' for='menu-toggle'><i class='bi bi-list'></i></label>
                    <nav id='menu'>
                        <input type='checkbox' hidden id='menu-toggle'/>
                        <ul id='menu-list'>
                            <li><a class='menu-option'href='index.php'>Inicio</a></li>
                            ";
                            if(isset($_SESSION['auth'])){
                                echo "
                                <li><a class='menu-option'href='menu.php'>Administrador</a></li>
                                ";
                            }
                            else
                                echo "
                                <li><a class='menu-option'href='login.php'>Inicia sesión</a></li>
                                ";
                            if (isset($_SESSION['auth'])) {
                                echo "
                                <li><a class='menu-option'href='scripts/logout.php'>Cerrar sesión</a></li>
                                ";
                            }
                            echo "
                        </ul>
                    </nav>
                </div>
            </div>
        </header>
        ";
    }

    function buildFooter() {
        echo "
        <footer>
            <div id='footer-content' class='row text-center p-5'>
                <div class='col-md-4' id='footer-logo-wrapper'>
                    <a id='footer-logo' href='index.php'>The Sailor's Wrench</a>
                </div>
                <div class='col-md-4' id='footer-info-wrapper'>
                    <p>© 2023 The Sailor's Wrench</p>
                    <p>Desarrollado por Javier Maciá Sempere</p>
                </div>
                <div class='col-md-4' id='footer-social-wrapper'>
                    <a href='https://www.facebook.com/'><i class='bi bi-facebook'></i></a>
                    <a href='https://www.instagram.com/'><i class='bi bi-instagram'></i></a>
                    <a href='https://www.twitter.com/'><i class='bi bi-twitter'></i></a>
                </div>
            </div>
        </footer>
        ";
    }

    function buildMeta(){
        echo "
        <meta charset='UTF-8'>
        <meta http-equiv='X-UA-Compatible' content='IE=edge'>
        <meta name='viewport' content='width=device-width, initial-scale=1.0'>
        ";
    }

?>