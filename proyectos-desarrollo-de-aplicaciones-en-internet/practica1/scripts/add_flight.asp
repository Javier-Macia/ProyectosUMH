<!--#include file=./includes/auth.inc-->
<!--#include file=./includes/conexion.inc-->
<%
    conexion.close
    set conexion = nothing
    response.redirect("../admin.asp")
%>  
