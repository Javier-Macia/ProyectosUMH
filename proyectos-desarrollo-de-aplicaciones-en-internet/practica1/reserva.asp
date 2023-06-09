<!-- #include file=./scripts/includes/conexion.inc -->
<!-- #include file=./scripts/includes/pagebuilder.inc -->

<%
    idVueloRecieved=request.querystring("idVuelo")
    idVuelo = CInt(idVueloRecieved)
    sql = "SELECT MAX(IDVUELO) AS maximo FROM VUELO"
    Set rs = conexion.execute(sql)
    maxIdVuelo = CInt(rs("maximo"))
    conexion.close
    set conexion = nothing

    If idVueloRecieved = "" Then
        response.redirect("./index.asp")
    ElseIf (idVuelo > maxIdVuelo) Or (idVuelo < "0") Then
        response.redirect("./index.asp")
    End If
%>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="./favicon/favicon.ico" type="image/x-icon">
    <title>Javifly</title>


    <!-- Estilos -->
    <link rel="stylesheet" href="./style/general.css">
    <link rel="stylesheet" href="./style/reserva.css">

    <!-- Fuentes-->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Lato&family=Work+Sans&display=swap" rel="stylesheet">
    
    <!--Conexion a fontawesome-->
    <script src="https://kit.fontawesome.com/2c36e9b7b1.js" crossorigin="anonymous"></script>


</head>
<body>

    <% build_header() %>

    <%
        Set Conexion = Server.CreateObject("ADODB.Connection")
        connect_db()
        sql = "SELECT * FROM LISTA_VUELOS_PRECIO WHERE IDVUELO = "& idVuelo
        Set rs = conexion.execute(sql)
    %>

    <main>
        <div id="detalles-reserva-main" class="card-style">
            <div>
                <h1> Reservar vuelo 
                    <% response.write(rs("CIUDAD_ORIGEN")) %> -> <% response.write(rs("CIUDAD_DESTINO")) %>
                </h1>
            </div>
            <%
                vuelta = Request.Cookies("vuelta")
                reserva_sin_vuelta = Request.Cookies("reserva_sin_vuelta")

                IF vuelta = "true" THEN
                    response.write("<div id='reserva-la-vuelta-wrapper'>")
                    response.write("    <p> ¿Quieres reservar la vuelta también? </p>")
                    response.write("</div>")
                END IF
            %>


            <div id="detalles-reserva">
                <div id="detalles-reserva-wrapper">
                    <div class="linea-detalle">
                        <div>
                            <p> <b>Fecha</b> </p>
                            <p><%response.write(rs("FECHA"))%></p>
                        </div>
                        <div>
                            <p><b>Compañía</b> </p>
                            <p><%response.write(rs("COMPANIA"))%></p>
                        </div>
                        <div>
                            <p><b> Avion </b></p>
                            <p><%response.write(rs("AVION"))%></p>
                        </div>
                    </div>
                    <div class="linea-detalle">
                        <p> <b>N plazas: </b> <%response.write(rs("N_PLAZAS"))%></p>
                        <p> <b>N plazas disponibles: </b><%response.write(rs("N_PLAZAS_DISPONIBLES"))%></p>
                    </div>
                    <div class="linea-detalle">
                        <p><b> Duración: </b> <%response.write(rs("DURACION"))%></p>
                        <p><b> Precio: </b><%response.write(rs("PRECIO"))%></p>
                    </div>
                </div>

                <form id="accion-reserva-wrapper" action="./scripts/reservar_vuelo.asp" method="POST">
                    <p> ¿Cuántos billetes quieres reservar? </p>
                    <div>
                        <input type = "number" name="nasientos" id="nasientos" required value="1" min="1" max="<%response.write(rs("N_PLAZAS_DISPONIBLES"))%>">
                    </div>
                    <p> Indique su nombre y apellidos: </p>
                    <div>
                        <input type="text" name="nombre" id="nombre" placeholder="Nombre" required>
                    </div>
                    <div>
                        <input type="text" name="apellidos" id="apellidos" placeholder="Apellidos" required>
                    </div>
                    <p> Indique su DNI para poder realizar la reserva: </p>
                    <div>
                        <input type="text" name="dni" id="dni" placeholder="DNI" required 
                            value="<%
                                dniRecieved=request.cookies("dni_vuelta")
                                response.write(dniRecieved)
                            %>">
                    </div>
                    <input type="submit" class="btn-primary" id="reservar" value="Reservar">
                
                    <!-- Oculto -->
                    <input type="hidden" name="idVuelo" value="<%response.write(idVuelo)%>">
                    <input type="hidden" name="origen" value="<% response.write(rs("CIUDAD_ORIGEN")) %>">
                    <input type="hidden" name="destino" value="<% response.write(rs("CIUDAD_DESTINO")) %>">

                </form>
                
            </div>
        </div>
    </main>

    <% build_footer()%>
</body>

<% 
    conexion.close
    set conexion = nothing
%>

<script src="./js/headerAnimations.js"></script>
</html>

<% response.cookies("vuelta") = "false" %>
<% response.cookies("reserva_sin_vuelta") = "false" %>
