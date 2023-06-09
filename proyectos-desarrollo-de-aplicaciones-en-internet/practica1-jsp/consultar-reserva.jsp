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
    String ciudad = request.getParameter("ciudad");
    String dni = request.getParameter("dni");

    String sqlString = "SELECT * FROM LISTADO_RESERVAS, RESERVA WHERE CIUDAD_ORIGEN = UPPER(\'" + ciudad + "\') AND LISTADO_RESERVAS.NIF = \'" + dni + "\' AND LISTADO_RESERVAS.IDRESERVA = RESERVA.IDRESERVA";

%>

<a href="comprobacion-reservas.html">Volver</a>

<%
   PreparedStatement StatementRSFind = null;
   ResultSet RSFind = null;
   boolean resultException = false;
   boolean rsReady = false;

   try{
      StatementRSFind = connRSFind.prepareStatement(sqlString);
      RSFind = StatementRSFind.executeQuery();
      rsReady = RSFind.next();
   }catch(SQLException e1){
      resultException = true;

%>
<p>Error: <%=e1.getMessage()%></p>
<%
}

int i = 0;


if (rsReady){
   boolean done = false;

%>

<p>Reservas encontradas:</p>
<table class="javifly-table">
   <thead>
      <tr>
         <th>Id Reserva</th>
         <th>Id Vuelo</th>
         <th>Apellidos</th>
         <th>Nombre</th>
         <th>Ciudad Origen</th>
         <th>Ciudad Destino</th>
         <th>Fecha Vuelo</th>
         <th>Compania</th>
         <th>Cancelado(Si/No)</th>
      </tr>
   </thead>
   <tbody>
<%
   while (!done){
      i++;
      Integer idReserva_int = (int)RSFind.getObject("IDRESERVA");
      String idReserva = Integer.toString(idReserva_int);
      Integer idVuelo_int = (int) RSFind.getObject("IDVUELO");
      String idVuelo = Integer.toString(idVuelo_int);
      String apellidos = (String) RSFind.getObject("APELLIDOS");
      String nombre = (String) RSFind.getObject("NOMBRE");
      String ciudad_origen = (String)RSFind.getObject("CIUDAD_ORIGEN");
      String ciudad_destino = (String)RSFind.getObject("CIUDAD_DESTINO");
      Object fechaVuelo_obj = RSFind.getObject("FECHA_VUELO");
      String fechaVuelo = fechaVuelo_obj.toString();
      String compania = (String) RSFind.getObject("COMPANIA");
      Object cancelado_obj = RSFind.getObject("CANCELADO");
      String cancelado = cancelado_obj.toString(); 
%>

<tr>
   <td><%=idReserva%></td>
   <td><%=idVuelo%></td>
   <td><%=apellidos%></td>
   <td><%=nombre%></td>
   <td><%=ciudad_origen%></td>
   <td><%=ciudad_destino%></td>
   <td><%=fechaVuelo%></td>
   <td><%=compania%></td>
   <td><% 
   if (cancelado == "1"){
      out.print("Si");
   } else{
      out.print("No");
   }%></td>
</tr>

<%
      done = !RSFind.next();
   } //Cerramos el while
   RSFind.close();
%>

      </tbody>
   </table>

<%
} // Cerramos el if
else{
%>
<p>No se han encontrado reservas con los parametros especificados.</p>
<%
}
if (StatementRSFind != null)
StatementRSFind.close();

if (connRSFind != null)
connRSFind.close();

%>
</main>
</body>
</html>