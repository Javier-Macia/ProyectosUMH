<!-- #include file=./includes/conexion.inc -->

<%
    Set Conexion = Server.CreateObject("ADODB.Connection")
    connect_db()
    origen = request.form("origen")
    destino = request.form("destino")

    ' Intercambiamos los valores de origen y destino
    SentenciaSQL = "SELECT * FROM LISTA_VUELOS_PRECIO WHERE UPPER(CIUDAD_ORIGEN) = UPPER('" & destino & "')"
    SentenciaSQL = SentenciaSQL & " AND UPPER(CIUDAD_DESTINO) = UPPER('" & origen & "')"
    Set rs = conexion.execute(SentenciaSQL)

    idVueloRecieved=request.form("idVuelo")
    idVuelo = CInt(idVueloRecieved)
    dni = request.form("dni")
    response.cookies("dni_vuelta") = dni
    apellidos = request.form("apellidos")
    nombre = request.form("nombre")
    nasientos = request.form("nasientos")

    Set idReserva = conexion.execute("SELECT MAX(IDRESERVA) as LastID FROM RESERVA")
    idReserva = (idReserva("LastID") + 1)

    SentenciaSQLInsertar = "INSERT INTO RESERVA (IDRESERVA, APELLIDOS, NOMBRE, NIF, IDVUELO, N_ASIENTOS, CANCELADO)"
    SentenciaSQLInsertar = SentenciaSQLInsertar + "VALUES (" & idReserva & ", '" & apellidos & "', '" & nombre & "', '" & dni & "', " & idVuelo & ", " & nasientos & ", 0)"
    conexion.execute(SentenciaSQLInsertar)
    

    ' Si ya hemos reservado la ida, y no existe vuelta, terminamos la reserva
    reserva_sin_vuelta = request.cookies("reserva_sin_vuelta")
    if reserva_sin_vuelta = "true" then
        response.cookies("reserva_sin_vuelta") = "false"
        response.cookies("vuelta") = "true"
        response.redirect("../reservado_correctamente.asp?idVuelo=" & idVuelo & "&idReserva=" & idReserva & "")
    end if

    ' Si no hay vuelos de vuelta, redirigimos a la página de con el id del vuelo de vuelta
    if rs.eof then
        response.cookies("reserva_sin_vuelta") = "true"
        response.cookies("vuelta") = "false"
        response.redirect("../reservado_correctamente.asp?idVuelo=" & idVuelo & "&idReserva=" & idReserva & "")
    else 
        ' Si hay vuelos de vuelta, redirigimos a la página de con el id del vuelo de vuelta
        idVuelo = rs("IDVUELO")
        response.cookies("reserva_sin_vuelta") = "false"
        response.cookies("vuelta") = "true"
        response.redirect("../reserva.asp?idVuelo=" & idVuelo & " ")
    end if
    conexion.close
    set conexion = nothing
%>