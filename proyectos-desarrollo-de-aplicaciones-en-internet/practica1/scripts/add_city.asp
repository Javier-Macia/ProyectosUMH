<!--#include file=./includes/auth.inc-->
<!--#include file=./includes/conexion.inc-->
<%
    'city_code = request.form("city_code")
    city = UCase(request.form("city"))
    airport_tax = request.form("airport_tax")
    
    Set city_code = conexion.execute("SELECT MAX(IDCIUDAD) as LastID FROM CIUDAD")
    new_city_code = (city_code("LastID") + 1)
    
    sql = "insert into CIUDAD values ('" & new_city_code & "' , '" & city & "', '" & airport_tax & "')"
    Set rs = conexion.execute(sql)
    conexion.close
    set conexion = nothing
    response.redirect("../admin.asp")

%>  
