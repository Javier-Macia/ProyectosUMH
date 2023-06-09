<%
function logout()
    If Session("login_ok") = true Then 
            Session("login_ok") = false
            Session.abandon
    End If 
End Function
%>

<%
function login()
    If Session("login_ok") = true Then 
            Response.Redirect("../admin.asp")
    End If 
End Function
%>

<%
function validate_login()
    if request.form("usuario") = "admin" and request.form("pass") = "1234" then
        session("login_ok") = true
    Else
        response.cookies("error_login") = "true"
        response.redirect("../login.asp")
    End If
End function
%>