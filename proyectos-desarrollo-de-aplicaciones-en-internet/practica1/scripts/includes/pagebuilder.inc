<%
Function build_footer()
    Response.Write("<footer id='footer'>")
    Response.Write("    <div class='footer-content'>")
    Response.Write("        <div class='footer-left'>")
    Response.Write("            <h3>Políticas</h3>")
    Response.Write("            <p>© 2023 <a href='./index.asp'>Javifly</a> Todos los derechos reservados.</p>")
    Response.Write("        </div>")
    Response.Write("        <div class='footer-center'>")
    Response.Write("            <h3>Síguenos en nuestras redes sociales</h3>")
    Response.Write("            <ul class='social'>")
    Response.Write("                <li><a target='_blank' href='https://www.facebook.com//'><i class='fa fa-facebook'></i></a></li>")
    Response.Write("                <li><a target='_blank' href='https://twitter.com/'><i class='fa fa-twitter'></i></a></li>")
    Response.Write("                <li><a target='_blank' href='https://www.linkedin.com/in/javiermaciasempere/'><i class='fa fa-linkedin'></i></a></li>")
    Response.Write("                <li><a target='_blank' href='https://www.youtube.com/watch?v=dQw4w9WgXcQ&ab_channel=RickAstley'><i class='fa fa-youtube'></i></a></li>")
    Response.Write("                <li><a target='_blank' href='https://www.instagram.com/javierms_/'><i class='fa fa-instagram'></i></a></li>")
    Response.Write("            </ul>")
    Response.Write("        </div>")

    Response.Write("        <div class='footer-right'>")
    Response.Write("            <h3>Contáctanos</h3>")
    Response.Write("            <a href='mailto:javier.macia05@goumh.umh.es'>javier.macia05@goumh.umh.es</a>")
    Response.Write("        </div>")
    Response.Write("    </div>")
    Response.Write("</footer>")
End Function
%>

<%
Function build_header()
    Response.Write("<header id='main-header'>")
    Response.Write("    <a href='index.asp' id='logo-wrapper'>")
    Response.Write("        <div id='logo'>")
    Response.Write("            <img src='./favicon/favicon_128x128.png' alt='logo'>")
    Response.Write("        </div>")
    Response.Write("        <h1> Javifly </h1>")
    Response.Write("    </a>")
    Response.Write("    <nav id='navbar'>")
    Response.Write("        <label id='icon_menu' for='menu_bar'>☰</label> ")
    Response.Write("        <input type='checkbox' id='menu_bar'>")
    Response.Write("        <ul id='menu_nav'>")
    Response.Write("            <li><a href='index.asp'>Inicio</a></li>")
    'Response.Write("            <li><a href='contact.asp'>Contacto</a></li>")
    Response.Write("            <li>")
    If Session("login_ok") = true Then
        Response.Write("            <a href='admin.asp'><i class ='fa fa-menu'></i>Administrador</a>")
    Else
        Response.Write("            <a href='login.asp'><i class='fa fa-user'></i> Identifícate</a>")
    End If
    Response.Write("            </li>")
    Response.Write("        </ul>")
    Response.Write("    </nav>")
    Response.Write("</header>")
End Function
%>