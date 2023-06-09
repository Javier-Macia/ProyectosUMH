<% If Session("login_ok") = false Then 
    Response.Redirect("./login.asp")
End If %>
<!-- #include file=./scripts/includes/conexion.inc -->
<!-- #include file=./scripts/includes/auth.inc -->
<!-- #include file=./scripts/includes/session.inc -->
<!-- #include file=./scripts/includes/pagebuilder.inc -->
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <link rel="icon" href="./favicon/favicon.ico" type="image/x-icon" />
        <title>Menú</title>
    
        <!-- Estilos -->
        <link rel="stylesheet" href="./style/general.css" />
        <link rel="stylesheet" href="./style/admin.css" />
    
        <!-- Fuentes-->
        <link rel="preconnect" href="https://fonts.googleapis.com" />
        <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
        <link href="https://fonts.googleapis.com/css2?family=Lato&family=Work+Sans&display=swap" rel="stylesheet" />
        
        <!--Conexion a fontawesome-->
        <script src="https://kit.fontawesome.com/2c36e9b7b1.js" crossorigin="anonymous"></script>
    </head>

    <body>
        <% build_header() %>

        <main id="admin-screen-wrapper">
            <div id="admin-menu-wrapper">
                <div id="admin-menu">
                    <ul id="admin_nav">
                        <li id="menu-title-wrapper">
                            <h1> Admin </h1>
                        </li>
                        <li id="menu-content-wrapper">
                            <div class="dropdown menu-action card-style" id="add-city-form-wrapper">
                                <button class="dropdown-button" id="btn-add" onclick="show_block('add-city-form','btn-add')">
                                    <i class="fa fa-chevron-down" ></i>
                                    <span class="btn-text"> Añadir una ciudad </span>
                                    <i class="fa fa-chevron-down"></i>
                                </button>
                                <form  class="hidden dropdown-content" id="add-city-form" action="./scripts/add_city.asp" method="POST">
                                    <label for="city"> Ciudad: </label>
                                    <input type="text" name="city" id="city_name" placeholder="Ciudad" required />
                                    <label for="airport_tax"> Tasa de aeropuerto: </label>
                                    <input type="number" name="airport_tax" id="airport_tax" placeholder="Tasa de aeropuerto" required />
                                    <input type="submit" value="Añadir" />
                                </form>
                            </div>

                            <button class="menu-action btn-primary" id="add-company-wrapper" onclick="visitar_insertar_reserva_jsp()">
                                <span class="btn-text"> Añadir una compañía con JSP </span>
                            </button>

                            <!--
                            <div class="dropdown menu-action card-style" id="add-flight-form-wrapper">
                                <button class="dropdown-button" id="btn-add-flight" onclick="show_block('add-flight-form','btn-add-flight')">
                                    <i class="fa fa-chevron-down" ></i>
                                    <span class="btn-text"> Añadir un vuelo </span>
                                    <i class="fa fa-chevron-down"></i>
                                </button>
                                <form  class="hidden dropdown-content" id="add-flight-form" action="./scripts/add_flight" method="POST">
                                    <label for="city_origin"> Ciudad de origen: </label>
                                    <input type="text" name="city_origin" id="city_origin" placeholder="Ciudad de origen" required />
                                    <label for="city_destination"> Ciudad de destino: </label>
                                    <input type="text" name="city_destination" id="city_destination" placeholder="Ciudad de destino" required />
                                    <label for="date"> Fecha: </label>
                                    <input type="date" name="date" id="date" placeholder="Fecha" required />
                                    <label for="company"> Compañía: </label>
                                    <input type="text" name="company" id="company" placeholder="Compañía" required />
                                    <label for="plane"> Avión: </label>
                                    <input type="text" name="plane" id="plane" placeholder="Avión" required />
                                    <label for="duration"> Duración: </label>
                                    <input type="number" name="duration" id="duration" placeholder="Duración" required />
                                    <label for="available_seats"> Plazas disponibles: </label>
                                    <input type="number" name="available_seats" id="available_seats" placeholder="Plazas disponibles" required />

                                    <input type="submit" value="Añadir" />
                                </form>
                            </div>
                            -->

                            <!--
                            <div class="dropdown menu-action card-style" id="add-company-form-wrapper">
                                <button class="dropdown-button" id="btn-add-company" onclick="show_block('add-company-form','btn-add-company')">
                                    <i class="fa fa-chevron-down" ></i>
                                    <span class="btn-text"> Añadir una compañía </span>
                                    <i class="fa fa-chevron-down"></i>
                                </button>
                                <form  class="hidden dropdown-content" id="add-company-form" action="./scripts/add_plane.asp" method="POST">
                                    <label for="company_code"> Código de avión: </label>
                                    <input type="number" name="company_code" id="company_code" placeholder="Código de vuelo" required min="1" max="999999999"/>

                                    <input type="submit" value="Añadir" />
                                </form>
                            </div>
                            -->

                        </li>
                        
                        <li id="logout_wrapper"> 
                            <button class="card-style" id="logout_btn" onclick="logout()"> Cerrar sesión </button>
                        </li>
                    </ul>
                </div>
            </div>


            <div id="admin-content">
                <div class="resultados-wrapper" id="resultados-ciudades-wrapper">
                    <div class="card-style">
                        <h1> Listado de ciudades </h1>
                        <table class="javifly-table" id="resultados-ciudades-content">
                            <thead>
                                <tr>
                                    <th> Código de ciudad </th>
                                    <th> Ciudad </th>
                                    <th> Tasa de aeropuerto </th>
                                    <th> Acciones </th>
                                </tr>
                            </thead>

                            <tbody>
                            <%
                            Set ciudades = Conexion.Execute("select IDCIUDAD,CIUDAD, TASA_AEROPUERTO from CIUDAD ORDER BY IDCIUDAD ASC")

                            do while not ciudades.EOF
                                Response.Write("<tr class='row_"& ciudades("IDCIUDAD") &"'>")
                                    Response.Write("<form id=""edit-form-"& ciudades("IDCIUDAD") &"""action='./scripts/edit_city.asp' method='POST'>")
                                        Response.Write("<td>")
                                            Response.Write("<span id='city-code-'" & ciudades("IDCIUDAD") & " >" & ciudades("IDCIUDAD") & "</span>")
                                            Response.Write("<input class='hidden' name='city_code' value='" & ciudades("IDCIUDAD") & "'/>")
                                        Response.Write("</td>")
                                        Response.Write("<td class='input_cell'>")
                                            Response.Write("<span id='city-code-'" & ciudades("IDCIUDAD") & " >" & ciudades("CIUDAD") & "</span>")
                                            Response.Write("<input class='hidden' name='city' value='" & ciudades("CIUDAD") & "' />")
                                        Response.Write("</td>")
                                        Response.Write("<td class='input_cell'>")
                                            Response.Write("<span id='city-code-'" & ciudades("IDCIUDAD") & " >" & ciudades("TASA_AEROPUERTO") & "</span>")
                                            Response.Write("<input class='hidden' name='airport_tax' value='" & ciudades("TASA_AEROPUERTO") & "' />")
                                        Response.Write("</td>")
                                    Response.Write("</form>")

                                    Response.Write("<td class='action-cell edit-cell'>")
                                        Response.Write("<button class='form-action btn-edit' onclick='toggle_edit_mode(""resultados-ciudades-content"", " & ciudades("IDCIUDAD") & ")' >")
                                        Response.Write("✏️Editar")
                                        Response.Write("</button>")
                                    Response.Write("</td>")

                                    Response.Write("<td class='actions-wrapper hidden'>")
                                        Response.Write("<button class='action-cell confirm-cell' onclick='confirm_edit(""resultados-ciudades-content""," & ciudades("IDCIUDAD") & ")' >")
                                            Response.Write("✅")
                                        Response.Write("</button>")

                                        Response.Write("<button class='action-cell cancel-cell' onclick='toggle_edit_mode(""resultados-ciudades-content"","& ciudades("IDCIUDAD") & ")' >")
                                            Response.Write("❌")
                                        Response.Write("</button>")

                                        Response.Write("<form class='action-cell delete-cell form-action' action='./scripts/rmv_city.asp' method='POST'>")
                                            Response.Write("<input class='hidden' type='number' name='city_code' value='" & ciudades("IDCIUDAD") & "'/>")
                                            Response.Write("<button class='btn-del'/>")
                                                Response.Write("🗑️")
                                            Response.Write("</button>")
                                        Response.Write("</form>")
                                    Response.Write("</td>")

                                Response.Write("</tr>")
                                ciudades.MoveNext
                            loop
                            %>
                            </tbody>
                        </table>
                    </div>
                </div>


                <div class="resultados-wrapper" id="resultados-vuelos-wrapper">
                    <div class="card-style">
                        <h1> Listado de vuelos </h1>
                        <table class="javifly-table" id="resultados-vuelos-content">
                            <thead>
                                <tr>
                                    <th> Código de vuelo </th>
                                    <th> ID origen </th>
                                    <th> ID destino </th>
                                    <th> Fecha </th>
                                    <th> ID compañía </th>
                                    <th> ID avión </th>
                                    <th> Duración </th>
                                    <th> Plazas disponibles </th>
                                    <th> Acciones </th>
                                </tr>
                            </thead>

                            <tbody>
                            <%
                            Set vuelos = Conexion.Execute("select IDVUELO, IDCIUDADORIGEN, IDCIUDADDESTINO, FECHA, IDCOMPANIA, IDAVION, DURACION, N_PLAZAS_DISPONIBLES from VUELO ORDER BY IDVUELO ASC")

                            do while not vuelos.EOF
                                Response.Write("<tr class='row_"& vuelos("IDVUELO") &"'>")
                                    Response.Write("<form id=""edit-form-"& vuelos("IDVUELO") &"""action='./scripts/edit_flight.asp' method='POST'>")
                                        Response.Write("<td>")
                                            Response.Write("<span id='flight-code-'" & vuelos("IDVUELO") & " >" & vuelos("IDVUELO") & "</span>")
                                            Response.Write("<input class='hidden' name='flight_code' value='" & vuelos("IDVUELO") & "'/>")
                                        Response.Write("</td>")
                                        Response.Write("<td class='input_cell'>")
                                            Response.Write("<span id='flight-code-'" & vuelos("IDVUELO") & " >" & vuelos("IDCIUDADORIGEN") & "</span>")
                                            Response.Write("<input class='hidden' name='city_origin' value='" & vuelos("IDCIUDADORIGEN") & "' />")
                                        Response.Write("</td>")
                                        Response.Write("<td class='input_cell'>")
                                            Response.Write("<span id='flight-code-'" & vuelos("IDVUELO") & " >" & vuelos("IDCIUDADDESTINO") & "</span>")
                                            Response.Write("<input class='hidden' name='city_destination' value='" & vuelos("IDCIUDADDESTINO") & "' />")
                                        Response.Write("</td>")
                                        Response.Write("<td class='input_cell'>")
                                            Response.Write("<span id='flight-code-'" & vuelos("IDVUELO") & " >" & vuelos("FECHA") & "</span>")
                                            Response.Write("<input class='hidden' name='date' value='" & vuelos("FECHA") & "' />")
                                        Response.Write("</td>")
                                        Response.Write("<td class='input_cell'>")
                                            Response.Write("<span id='flight-code-'" & vuelos("IDVUELO") & " >" & vuelos("IDCOMPANIA") & "</span>")
                                            Response.Write("<input class='hidden' name='company' value='" & vuelos("IDCOMPANIA") & "' />")
                                        Response.Write("</td>")
                                        Response.Write("<td class='input_cell'>")
                                            Response.Write("<span id='flight-code-'" & vuelos("IDVUELO") & " >" & vuelos("IDAVION") & "</span>")
                                            Response.Write("<input class='hidden' name='plane' value='" & vuelos("IDAVION") & "' />")
                                        Response.Write("</td>")
                                        Response.Write("<td class='input_cell'>")
                                            Response.Write("<span id='flight-code-'" & vuelos("IDVUELO") & " >" & vuelos("DURACION") & "</span>")
                                            Response.Write("<input class='hidden' name='duration' value='" & vuelos("DURACION") & "' />")
                                        Response.Write("</td>")
                                        Response.Write("<td class='input_cell'>")
                                            Response.Write("<span id='flight-code-'" & vuelos("IDVUELO") & " >" & vuelos("N_PLAZAS_DISPONIBLES") & "</span>")
                                            Response.Write("<input class='hidden' name='available_seats' value='" & vuelos("N_PLAZAS_DISPONIBLES") & "' />")
                                        Response.Write("</td>")
                                    Response.Write("</form>")
                                    Response.Write("<td class='action-cell edit-cell'>")
                                        Response.Write("<button class='form-action btn-edit' onclick='toggle_edit_mode(""resultados-vuelos-content""," & vuelos("IDVUELO") & ")' >")
                                        Response.Write("✏️Editar")
                                        Response.Write("</button>")
                                    Response.Write("</td>")
                                    Response.Write("<td class='actions-wrapper hidden'>")
                                        Response.Write("<button class='action-cell confirm-cell' onclick='confirm_edit(""resultados-vuelos-content""," & vuelos("IDVUELO") & ")' >")
                                            Response.Write("✅")
                                        Response.Write("</button>")
                                        Response.Write("<button class='action-cell cancel-cell' onclick='toggle_edit_mode(""resultados-vuelos-content"","& vuelos("IDVUELO") & ")' >")
                                            Response.Write("❌")
                                        Response.Write("</button>")
                                        Response.Write("<form class='action-cell delete-cell form-action' action='./scripts/rmv_flight.asp' method='POST'>")
                                            Response.Write("<input class='hidden' type='number' name='flight_code' value='" & vuelos("IDVUELO") & "'/>")
                                            Response.Write("<button class='btn-del'/>")
                                                Response.Write("🗑️")
                                            Response.Write("</button>")
                                        Response.Write("</form>")
                                    Response.Write("</td>")
                                Response.Write("</tr>")
                                vuelos.MoveNext
                            loop
                            %>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </main>

        <% build_footer() %>
        
    </body>
</html>

<script src="./js/headerAnimations.js"></script>
<script src="./js/adminUI.js" ></script>
<script src="./js/session.js"></script>

<script>
function visitar_insertar_reserva_jsp(){
    location.href = "http://localhost:8080/practica1-jsp/insertar-compania.html";
}
</script>
