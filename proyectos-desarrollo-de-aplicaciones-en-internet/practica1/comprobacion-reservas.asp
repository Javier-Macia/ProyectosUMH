<!-- #include file=./scripts/includes/conexion.inc -->
<!-- #include file=./scripts/includes/pagebuilder.inc -->

<%
    dni= request.queryString("dni")
    codigo_rec = request.queryString("codigo")
    codigo = CInt(codigo_rec)

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
    <link rel="stylesheet" href="./style/comprobar-reserva.css">

    <!-- Fuentes-->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Lato&family=Work+Sans&display=swap" rel="stylesheet">
    
    <!--Conexion a fontawesome-->
    <script src="https://kit.fontawesome.com/2c36e9b7b1.js" crossorigin="anonymous"></script>


</head>
<body>

    <% build_header() %>

    <main>
        <form id="form-comprobar" class="card-style" method="GET" action="comprobacion-reservas.asp">
            <h1>Comprobar reserva</h1>
            <div class="input-container">
                <label for="codigo">Código de reserva</label>
                <input type="number" id="codigo" name="codigo" min="1" required>
                <label for="dni">DNI</label>
                <input type="text" id="dni" name="dni" required>
            </div>
            <div class="input-container">
                <input class="btn-primary" type="submit" value="Comprobar">
            </div>
        </form>
        <div id="resultados-comprobar" class="card-style">
            <h1>Resultado de la consulta</h1>
            <%
                If codigo = 0 Or dni = "" Then
                    Response.Write("<p>Introduce los datos de la reserva para comprobarla</p>")
                Else
                    ''Set rs = Server.CreateObject("ADODB.Recordset")
                    sql = "SELECT * FROM listado_reservas WHERE NIF='" & dni & "' AND IDRESERVA=" & codigo_rec  
                    Set rs = Conexion.Execute(sql)
                    If rs.EOF Then
                        Response.Write("<p>No se ha encontrado ninguna reserva con esos datos</p>")
                    Else
                        Response.Write("<p>Se ha encontrado una reserva con esos datos</p>")
                        Response.Write("<p>Fecha del vuelo: " & rs("fecha_vuelo") & "</p>")
                        Response.Write("<p>Origen: " & rs("ciudad_origen") & "</p>")
                        Response.Write("<p>Destino: " & rs("ciudad_destino") & "</p>")
                        
                        Response.Write("<p>Cancelado: ")
                        If rs("cancelado") = "1" Then
                            Response.Write("Sí") 
                        Else 
                            Response.Write("No")
                        End If
                        Response.Write("</p>")
                        Response.Write("<p>Compañía: " & rs("compania") & "</p>")
                        Response.Write("<p>DNI: " & rs("nif") & "</p>")
                    End If
                    rs.Close
                End If
            %>
        </div>
    </main>

    <% build_footer()%>
</body>

<script src="./js/headerAnimations.js"></script>
</html>

<%' response.cookies("vuelta") = "false" %>
<%' response.cookies("reserva_sin_vuelta") = "false" %>
