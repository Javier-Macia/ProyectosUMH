<?php
namespace App\Http\Controllers;
use App\Http\Controllers\Controller;
use DB;

class ClienteController extends Controller{
    public function verTodos(){
        $clientes = DB::select('select * from clientes');
        return view('clientes.ver', ['clientes' => $clientes]);
    }

    public function verUno($id){
        $clientes = DB::select('select * from clientes where Id_Cliente = ?', [$id]);
        return view('clientes.ver', ['clientes' => $clientes]);
    }
}?>