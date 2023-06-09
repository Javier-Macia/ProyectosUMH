<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| This file is where you may define all of the routes that are handled
| by your application. Just tell Laravel the URIs it should respond
| to using a Closure or controller method. Build something great!
|
*/

Route::get('/', function () {
    return view('index');
});

Route::get('datosclientes/', 'ClienteController@verTodos');
Route::get('datoscliente/{id}', 'ClienteController@verUno');

Route::get('listarclientes', function(){
    return view('listarclientes');
});

Route::get('listarclientes/poblacion/{poblacion}', function($poblacion){
    return view('clientes_por_poblacion', ['poblacion' => $poblacion]);
});

Route::get('actualizar_embarcacion/{id}/{anyo}', 'EmbarcacionController@actualizarAnyo');

Route::get('listadoclientes', 'ListadoClientesController@ver');