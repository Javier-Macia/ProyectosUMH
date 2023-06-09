
function AJAXCrearObjeto(){
    var obj;
    if (window.XMLHttpRequest) {
        obj = new XMLHttpRequest();
    } else {
        // Es IE
        try{
            obj = new ActiveXObject("Msxml2.XMLHTTP");
            //Es IE
        }catch(e){
            // El navegador utilizado no está soportado
        }
    }
    return obj;
}

function leerDatos(){
    if (oXML.readyState == 4) {
        if (oXML.status == 200) {
            var xml;
            try {
                xml = oXML.responseXML.documentElement
            } catch(e) {
                xml = null;
            }

            var tabla = document.getElementById("resultados-vuelos-contenido");
            var definicion_tabla = new String("");
            var v;
            var item;
            if (xml == null){
                definicion_tabla += '<td class="not-found-message" colspan="8">No hay vuelos disponibles</td>';
            }else{
                definicion_tabla += '<th>Ciudad Origen</th><th>Ciudad Destino</th><th>Fecha</th><th>Compa&ntilde;&iacute;a</th><th>Plazas Disponibles</th><th>Duraci&oacute;n</th><th>ID Avi&oacute;n</th><th></th>';
                for (var i = 0; i < xml.getElementsByTagName('vuelo').length; i++) {
                    definicion_tabla += '<tr>';
                    item = xml.getElementsByTagName('vuelo')[i];
                    /*
                    v = item.getElementsByTagName('idVuelo')[0].firstChild.data;
                    definicion_tabla += '<td>' + v + '</td>';
                    */
                    v = item.getElementsByTagName('ciudad_origen')[0].firstChild.data;
                    definicion_tabla += '<td>' + v + '</td>';
                    //
                    v = item.getElementsByTagName('ciudad_destino')[0].firstChild.data;
                    definicion_tabla += '<td>' + v + '</td>';                
                    //
                    v = item.getElementsByTagName('fecha')[0].firstChild.data;
                    definicion_tabla += '<td>' + v + '</td>';
                    //
                    v = item.getElementsByTagName('n_plazas_disponibles')[0].firstChild.data;
                    definicion_tabla += '<td>' + v + '</td>';
                    definicion_tabla= definicion_tabla+'<td>' + v + '</td>';
                    // 
                    v = item.getElementsByTagName('duracion')[0].firstChild.data;
                    definicion_tabla= definicion_tabla+'<td>' + v + '</td>';
                    // 
                    /*
                    v = item.getElementsByTagName('avion')[0].firstChild.data;
                    definicion_tabla= definicion_tabla+'<td>' + v + '</td>';
                    */

                    // Cogemos el id del vuelo para poder reservarlo
                    v = item.getElementsByTagName('idVuelo')[0].firstChild.data;
                    definicion_tabla += '<td><button class="btn btn-primary" onclick="reservar_vuelo('+v+')">Reservar</button></td>';
                    definicion_tabla += '</tr>';
                }
            }
            tabla.innerHTML = definicion_tabla;
        }
    }
}

function reservar_vuelo(idVuelo){
    window.location.href = "reserva.asp?idVuelo="+idVuelo;
}

function mostrar_vuelos() {
    event.preventDefault();
    var valorOrigen = document.getElementById("origen").options[document.getElementById("origen").selectedIndex].text;
    var valorDestino = document.getElementById("destino").options[document.getElementById("destino").selectedIndex].text;
    var resultados = document.getElementById("resultados-vuelos");

    if (resultados.classList.contains("hidden")) {
        resultados.classList.remove("hidden");
    }

    if ((valorOrigen == document.getElementById("origen").options[0].text) || (valorOrigen == document.getElementById("origen").options[1].text)) {
        valorOrigen = "";
    }
    if ((valorDestino == document.getElementById("destino").options[0].text) || (valorDestino == document.getElementById("destino").options[1].text)) {
        valorDestino = "";
    }

    console.log("Mostrar vuelos");
    console.log(valorOrigen);
    console.log(valorDestino);

    oXML = AJAXCrearObjeto();
    oXML.open('POST', './scripts/consulta_vuelos.asp');
    oXML.onreadystatechange = leerDatos;
    oXML.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    oXML.send('origen=' + valorOrigen + '&destino=' + valorDestino);
}

function intercambiar_vuelos(){
    var origen = document.getElementById("origen");
    var destino = document.getElementById("destino");
    var aux = origen.selectedIndex;
    origen.selectedIndex = destino.selectedIndex;
    destino.selectedIndex = aux;
}

function muestra_opciones(context){
    var origen = document.getElementById("origen-wrapper")
    var destino = document.getElementById("destino-wrapper")

    var valorOrigen = document.getElementById("origen").options[document.getElementById("origen").selectedIndex].text;
    var valorDestino = document.getElementById("destino").options[document.getElementById("destino").selectedIndex].text;
    
    var select_pulsado = document.getElementById(context);
    var optionsOrigen = origen.getElementsByTagName("option");
    var optionsDestino = destino.getElementsByTagName("option");


    if (select_pulsado.id == "origen") {
        for (var i = 0; i < optionsDestino.length; i++) {
            optionsDestino[i].classList.remove("hidden");
            if (optionsDestino[i].text == valorOrigen) {
                optionsDestino[i].classList.add("hidden");
            }
        }
    } else {
        for (var i = 0; i < optionsOrigen.length; i++) {
            optionsOrigen[i].classList.remove("hidden");
            if (optionsOrigen[i].text == valorDestino) {
                optionsOrigen[i].classList.add("hidden");
            }
        }
    }

}