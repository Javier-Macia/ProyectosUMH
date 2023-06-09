<!-- #include file=./scripts/includes/conexion.inc -->
<!-- #include file=./scripts/includes/pagebuilder.inc -->

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="./favicon/favicon.ico" type="image/x-icon">
    <title>Javifly</title>


    <!-- Estilos -->
    <link rel="stylesheet" href="./style/general.css">
    <link rel="stylesheet" href="./style/index.css">

    <!-- Fuentes-->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Lato&family=Work+Sans&display=swap" rel="stylesheet">
    
    <!--Conexion a fontawesome-->
    <script src="https://kit.fontawesome.com/2c36e9b7b1.js" crossorigin="anonymous"></script>


</head>
<body>
    <% build_header() %>
    <main id="index-content">
        <div id="buscador-vuelos-wrapper">
            <div id="buscador-vuelos">
                <div id="buscador-vuelos-titulo-wrapper">
                    <h2>Tu aventura comienza aquí </h2>
                </div>
                <form id="form-buscador" onsubmit="return mostrar_vuelos();">
                    <div id="form-content-wrapper">
                        <div class="vuelos-input-wrapper">
                            <div id="origen-destino-wrapper">
                                <div class="input-wrapper" id="origen-wrapper">
                                    <label for="origen"> <i class="fa fa-plane-departure"></i></label>
                                    <select name="origen" id="origen" onclick='return muestra_opciones("origen")' onchange='return mostrar_vuelos()'> 
                                    <option value="-1" selected disabled>Origen</option>
                                    <option value="0">Cualquier origen</option>
                                    <% 
                                    Set origen = Conexion.Execute("select IDCIUDAD,CIUDAD, TASA_AEROPUERTO from CIUDAD")
                                    do while not origen.EOF
                                        Response.Write("<option value='" & origen("IDCIUDAD") & "'>" & origen("CIUDAD") & "</option>")
                                        origen.MoveNext
                                    loop
                                    %>
                                    </select>   
                                </div>
                                <div class="input-wrapper" id="destino-wrapper">
                                    <label for="destino"> <i class="fa fa-plane-arrival"></i></label>
                                    <select name="destino" id="destino" onclick='return muestra_opciones("destino")' onchange='return mostrar_vuelos()'>
                                    <option value="-1" selected disabled>Destino</option>
                                    <option value="0">Cualquier destino</option>
                                    <% 
                                    Set destino = Conexion.Execute("select IDCIUDAD,CIUDAD, TASA_AEROPUERTO from CIUDAD")
                                    do while not destino.EOF
                                        Response.Write("<option value='" & destino("IDCIUDAD") & "'>" & destino("CIUDAD") & "</option>")
                                        destino.MoveNext
                                    loop
                                    %>
                                    </select>
                                </div>
                            </div>
                            <div id = "intercambiar-vuelos">
                                <button class="btn-secondary"id="intercambiar-vuelos-boton" onclick='intercambiar_vuelos()'>
                                    <i class="fas fa-exchange-alt"></i>
                                </button>    
                            </div>
                        </div>
                        
                    </div>
                    <div class="input-wrapper" id="buscar-vuelos-wrapper">
                        <input type="submit" name="enviar" class="btn-secondary" id="enviar" value="Buscar">
                    </div>
                </form>    
            </div>
        </div>
        
        <div id="resultados-vuelos-wrapper">
            <div class="card-style hidden" id="resultados-vuelos">
                <div id="resultados-vuelos-titulo-wrapper">
                    <h2>Resultados de la consulta</h2>
                </div>
                <div id="resultados-vuelos-contenido-wrapper">
                    <table id="resultados-vuelos-contenido">

                    </table>  
                </div>
            </div>
        </div>

        <div id="consulta-reservas-wrapper">
            <div class="card-style">
                <h2> Consulta tu vuelo </h2>
                <button class="btn btn-secondary" id="check-reserve-btn" onclick="visitar_comprobacion_reservas()">Pulsando aquí</button>
                <button class="btn btn-primary" id="check-reservejsp-btn" onclick="visitar_comprobacion_reservas_jsp()">Pulsando aquí lo harás con JSP</button>
            </div>
        </div>

        <div id="compromiso-wrapper">
            <div class="card-style" id="compromiso">
                <div id="compromiso-titulo-wrapper">
                    <h2>Compromiso <i class="fa fa-seedling"></i> </h2>
                </div>
                <div id="compromiso-sections">
                    <div class="compromiso-contenido-row">
                        <div class="compromiso-contenido">
                            <p> La aerolínea Javifly se compromete firmemente con la protección del medio ambiente y ha adoptado una serie de medidas para minimizar su impacto en el planeta. 
                            Entre ellas se encuentra el uso de tecnologías de vanguardia en sus aviones, como motores más eficientes en el consumo de combustible y materiales más ligeros en la
                            construcción de la aeronave. </p>
                        </div>
                        <div class="compromiso-image" id="compromiso-image-1">
                        </div>
                    </div>

                    <div class="compromiso-contenido-row">
                        <div class="compromiso-contenido">
                            <p>Además, la aerolínea ha implementado un programa de gestión de residuos que se centra en reducir, reutilizar y reciclar los materiales que se utilizan 
                            a bordo de sus vuelos. Se han instalado sistemas de filtración de agua para reducir la cantidad de residuos sólidos y líquidos que se generan a bordo,
                            y se han establecido acuerdos con proveedores de alimentos y bebidas para reducir la cantidad de envases y utensilios de plástico que se utilizan.</p>
                        </div>
                        <div class="compromiso-image" id="compromiso-image-2">
                        </div>
                    </div>

                    <div class="compromiso-contenido-row">
                        <div class="compromiso-contenido">
                            <p>La aerolínea Javifly también ha adoptado prácticas de conservación de energía, como el uso de sistemas de iluminación LED de bajo consumo y la 
                            optimización de las rutas de vuelo para minimizar el consumo de combustible. Asimismo, la empresa ha implementado un programa de compensación de
                            carbono, a través del cual los clientes pueden contribuir a la reducción de las emisiones de gases de efecto invernadero generadas por sus vuelos. </p>
                        </div>
                        <div class="compromiso-image" id="compromiso-image-3">
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </main>
    <% build_footer() %>
</body>

</html>

<script src="./js/headerAnimations.js"></script>
<script type="text/javascript">
    function visitar_comprobacion_reservas () {
        location.href = "comprobacion-reservas.asp";
    };

    function visitar_comprobacion_reservas_jsp(){
        location.href = "http://localhost:8080/practica1-jsp/comprobacion-reservas.html";
    };
</script>
<script src="./js/indexUI.js"></script>