<% 
    If Session("login_ok") = false Then 
        Response.Redirect("./login.asp")
    End If
%>