<!--#include file=./includes/auth.inc-->
<!--#include file=./includes/conexion.inc-->
<%
    city_code = request.form("city_code")
    city = request.form("city")
    upper_city = UCase(city)
    airport_tax = request.form("airport_tax")
    
    sql = "UPDATE CIUDAD SET CIUDAD = '" & upper_city & "', TASA_AEROPUERTO = " & airport_tax & " WHERE IDCIUDAD = '" & city_code & "'"
    Set rs = conexion.execute(sql)
    conexion.close
    set conexion = nothing
    response.redirect("../admin.asp")
%>