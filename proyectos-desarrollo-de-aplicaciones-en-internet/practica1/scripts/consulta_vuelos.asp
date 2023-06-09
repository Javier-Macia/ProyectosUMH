<!--#include file="./includes/conexion.inc"-->
<%
origen = request.form("origen")
destino = request.form("destino")

response.ContentType = "text/xml"
response.CacheControl = "no-cache, must-revalidate"

if (origen <> "") then 
    SentenciaSQL = "SELECT * FROM LISTA_VUELOS_PRECIO WHERE UPPER(CIUDAD_ORIGEN) = UPPER('" & origen & "')"
    if (destino <> "") then
        SentenciaSQL = SentenciaSQL & " AND UPPER(CIUDAD_DESTINO) = UPPER('" & destino & "')"
    end if
else 
    SentenciaSQL = "SELECT * FROM LISTA_VUELOS_PRECIO"
    if (destino <> "") then
        SentenciaSQL = SentenciaSQL & " WHERE UPPER(CIUDAD_DESTINO) = UPPER('" & destino & "')"
    end if
end if

Set rs = Conexion.Execute(SentenciaSQL)

if (not(rs.Eof)) then

    %><?xml version="1.0" encoding="UTF-8"?><%
    response.write("<xml>")

    ' Recorremos el Recorset
    do until rs.Eof
        response.write("<vuelo>")

        response.write("<idVuelo>")
        response.write( rs("IDVUELO") )
        response.write("</idVuelo>")

        response.write("<ciudad_origen>")
        response.write( rs("CIUDAD_ORIGEN") )
        response.write("</ciudad_origen>")

        response.write("<ciudad_destino>")
        response.write( rs("CIUDAD_DESTINO") )
        response.write("</ciudad_destino>")

        response.write("<fecha>")
        response.write( rs("FECHA") )
        response.write("</fecha>")

        response.write("<n_plazas_disponibles>")
        response.write( rs("N_PLAZAS_DISPONIBLES") )
        response.write("</n_plazas_disponibles>")


        response.write("<duracion>")
        response.write( rs("DURACION") )
        response.write("</duracion>")

        response.write("<avion>")
        response.write( rs("IDAVION") )
        response.write("</avion>")

        response.write("</vuelo>")
        rs.MoveNext
    loop
    response.write("</xml>")

end if

rs.Close
set rs = nothing

close_conexion()

%>