<%
Set Conexion = Server.CreateObject("ADODB.Connection")

function connect_db()
    Conexion.ConnectionString = "Data Source=agencia; USER=SYSDBA; PASSWORD=masterkey"
    Conexion.Mode = 1
    Conexion.Open
end function

function close_conexion()
    Conexion.Close
    set Conexion = nothing
end function

%>

<%
    connect_db()
%>