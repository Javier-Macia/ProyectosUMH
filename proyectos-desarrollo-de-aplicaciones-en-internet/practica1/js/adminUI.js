function switch_class(id, visibility){
    let e = document.getElementById(id);
    let b = e.classList.contains(visibility);

    if (b) {
        e.classList.remove(visibility);
        e.classList.add("hidden");
    }
    else {
        e.classList.remove("hidden");
        e.classList.add(visibility);
    }
}

function permute_arrows(button) {
    var e = document.getElementById(button).getElementsByClassName("fa");
  
    for (var i = 0; i < e.length; i++) {
        if (e[i].classList.contains("fa-chevron-up")) {
            e[i].classList.remove("fa-chevron-up");
            e[i].classList.add("fa-chevron-down");
            e[i].classList.add("fa-rotate");
            e[i].addEventListener("animationend", function() {
              this.classList.remove("fa-rotate");
            });
        } else if (e[i].classList.contains("fa-chevron-down")) {
            e[i].classList.remove("fa-chevron-down");
            e[i].classList.add("fa-chevron-up");
            e[i].classList.add("fa-rotate");
            e[i].addEventListener("animationend", function() {
              this.classList.remove("fa-rotate");
            });
        }
    }
}

function hide_other_blocks(id) {
    var dropdowns = document.getElementById("menu-content-wrapper").getElementsByClassName("dropdown");
    for (var i = 0; i < dropdowns.length; i++) {
        var content = dropdowns[i].getElementsByClassName("dropdown-content");
        var button = dropdowns[i].getElementsByClassName("dropdown-button");
        var content_id = content[0].id;
        var button_id = button[0].id;
        
        var e = document.getElementById(content_id);

        if (e.classList.contains("block") && content_id != id){
            switch_class(content_id, "block");
            permute_arrows(button_id);
        }
    }
}
  

function show_block(id,button) {
    hide_other_blocks(id);
    switch_class(id, "block");
    permute_arrows(button);
}

function confirm_edit(tableId, id){
    let table = document.getElementById(tableId).getElementsByTagName("tbody")[0];
    let row = table.getElementsByClassName("row_"+id)[0];
    let form = row.querySelector("#edit-form-"+id);
    form.submit();
}

function hide_other_edit_cells(table, id){
    let rows = table.getElementsByTagName("tr");
    let row_id = "row_"+id;
    for (let i = 0; i < rows.length; i++){
        let row = rows[i];
        if (!row.classList.contains(row_id)){
            let edit_cell = row.getElementsByClassName("edit-cell")[0];
            let actions_cell = row.getElementsByClassName("actions-wrapper")[0];
            /**Ocultamos la casilla de editar si esta está mostrada, y viceversa */
            if (edit_cell.classList.contains("hidden")){
                edit_cell.classList.remove("hidden");
                actions_cell.classList.add("hidden");
            }
            let input_cells = row.getElementsByClassName("input_cell");
            /**Ocultamos los span y mostramos los inputs */
            for (let j = 0; j < input_cells.length; j++) {
                let text_input = input_cells[j].getElementsByTagName("input")[0];
                let text_span = input_cells[j].getElementsByTagName("span")[0];
                if (text_span.classList.contains("hidden")){
                    text_input.classList.add("hidden");
                    text_span.classList.remove("hidden");
                }
            }
        }
    }
}

function toggle_edit_mode(tableId, id){
    
    let t = document.getElementById(tableId).getElementsByTagName("tbody")[0];
    let row = t.getElementsByClassName("row_"+id)[0];
    let edit_cell = row.getElementsByClassName("edit-cell")[0];
    let actions_cell = row.getElementsByClassName("actions-wrapper")[0];
    hide_other_edit_cells(t, id);
    /**Ocultamos los span y mostramos los inputs */
    let input_cells = row.getElementsByClassName("input_cell");
    for (let i = 0; i < input_cells.length; i++) {
        let text_input = input_cells[i].getElementsByTagName("input")[0];
        let text_span = input_cells[i].getElementsByTagName("span")[0];
        if (text_input.classList.contains("hidden")){
            text_input.classList.remove("hidden");
            text_span.classList.add("hidden");
        }else{
            text_input.classList.add("hidden");
            text_span.classList.remove("hidden");
        }
    }
    /**Ocultamos la casilla de editar si esta está mostrada, y viceversa */
    if (edit_cell.classList.contains("hidden")){
        edit_cell.classList.remove("hidden");
        actions_cell.classList.add("hidden");
    }else{
        edit_cell.classList.add("hidden");
        actions_cell.classList.remove("hidden");
    }
}


function addFlight(){
    event.preventDefault();
    let table = document.getElementById("resultados-vuelos-content");
    let flight_code = table.getElementsByTagName("tr").length;
    let origin = document.getElementById("origin").value;
    let destination = document.getElementById("destination").value;
    let departure_time = document.getElementById("departure_time").value;
    let arrival_time = document.getElementById("arrival_time").value;
    let flight_duration = document.getElementById("flight_duration").value;
    let flight_price = document.getElementById("flight_price").value;

    let form = document.getElementById("add-flight-form");

    form.method = "POST";
    form.action = "./scripts/add_flight.asp";
    let params = {flight_code, origin, destination, departure_time, arrival_time, flight_duration, flight_price};
    
    for (const key in params) {
        if (params.hasOwnProperty(key)) {
            const hiddenField = document.createElement('input');
            hiddenField.type = 'hidden';
            hiddenField.name = key;
            hiddenField.value = params[key];

            form.appendChild(hiddenField);
        }
    }

    form.submit();
}