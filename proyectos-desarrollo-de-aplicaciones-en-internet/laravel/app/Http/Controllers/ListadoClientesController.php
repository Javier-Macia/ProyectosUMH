<?php
namespace App\Http\Controllers;

use App\Http\Controllers\Controller;

use App;

class ListadoClientesController extends Controller{
    public function ver(){
        $clientes = App\Cliente::all();
        return view('listadoclientes', ['clientes' => $clientes]);
    }
}