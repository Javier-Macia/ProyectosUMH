<!-- #include file=./scripts/includes/conexion.inc -->
<!-- #include file=./scripts/includes/pagebuilder.inc -->

<%
    idVueloRecieved=request.querystring("idVuelo")
    idReservaRecieved=request.querystring("idReserva")
    idVuelo = CInt(idVueloRecieved)
    idReserva = CInt(idReservaRecieved)

    sql = "SELECT MAX(IDVUELO) AS maximo FROM VUELO"
    Set rs = conexion.execute(sql)
    maxIdVuelo = CInt(rs("maximo"))
    conexion.close
    set conexion = nothing

    If idVueloRecieved = "" or idReservaRecieved="" Then
        response.redirect("./index.asp")
    ElseIf (idVuelo > maxIdVuelo) Or (idVuelo < "0") Then
        response.redirect("./login.asp")
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
            <h1> Se ha realizado la reserva con éxito </h1>
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
                <div id="id-reserva">
                    <h2> <b> ID de tu reserva: </b> <%=idReserva%></h2>
                </div>
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

<%' response.cookies("vuelta") = "false" %>
<%' response.cookies("reserva_sin_vuelta") = "false" %>
