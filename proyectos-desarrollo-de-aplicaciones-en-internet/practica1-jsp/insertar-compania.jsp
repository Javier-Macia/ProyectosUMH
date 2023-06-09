<%@include file="conexion.jsp" %>

<!DOCTYPE html>
<html lang="es">
<head>
   <title>Javifly</title>
   <meta charset="UTF-8">
   <link rel="stylesheet" href="style/general.css">
</head>

<body>
<main>

<%
    String idString = request.getParameter("id");
    String nombre = request.getParameter("nombre");
    int id = Integer.parseInt(idString);
    PreparedStatement pstmt = null;
    boolean resultException = false;
    boolean rsReady = false;


    String sqlString = "INSERT INTO COMPANIA (idcompania,compania) VALUES("+id+",'"+nombre+"')";

    try{
        pstmt = connRSFind.prepareStatement(sqlString);
        pstmt.executeUpdate();
    }catch(SQLException e1){
        resultException = true;
        out.println("El id ya existe, no se ha podido introducir");
    }finally {
        if (pstmt != null) {
            try {
                pstmt.close();
            } catch (SQLException e) {
                out.println("No se ha podido cerrar la conexión a la base de datos");
            }
        }
    }

    if (!resultException){
        out.println("Compania agregada");
    }

%>
<a href="insertar-compania.html">Volver</a>

</main>
</body>
