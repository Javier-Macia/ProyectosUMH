<%@page contentType="text/html; charset=iso-8859-1" language="java" import="java.sql.*,org.firebirdsql.management.*"%>

<%

   String DB_SERVER_URL = "localhost";
   int DB_SERVER_PORT = 3050;

   String DB_PATH = "C:/DAI";

   String DB_NAME = "AGENCIA.FDB";
   String DB_USER = "SYSDBA";
   String DB_PASSWORD = "masterkey";
   String DB_CONNECTION_STRING = "jdbc:firebirdsql:"+DB_SERVER_URL+"/"+DB_SERVER_PORT+":"+DB_PATH+"/"+DB_NAME;

   FBManager fbManager = new FBManager();

   fbManager.setServer(DB_SERVER_URL);
   fbManager.setPort(DB_SERVER_PORT);

   fbManager.start();

   Class.forName("org.firebirdsql.jdbc.FBDriver");
   
   Connection connRSFind = DriverManager.getConnection(DB_CONNECTION_STRING, DB_USER, DB_PASSWORD);

%>

