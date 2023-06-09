<!--#include file="./includes/session.inc"--> 
<%
response.cookies("login_error") = "false"
validate_login()
login()
%>
