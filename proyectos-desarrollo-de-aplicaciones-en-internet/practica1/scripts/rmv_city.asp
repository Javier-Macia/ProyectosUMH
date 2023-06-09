<!--#include file=./includes/auth.inc-->
<!--#include file=./includes/conexion.inc-->
<%
    city_code = request.form("city_code")
    
    sql = "DELETE FROM CIUDAD WHERE idciudad = '" & city_code & "'"
    Set rs = conexion.execute(sql)
    conexion.close
    set conexion = nothing
    response.redirect("../admin.asp")
%>  
