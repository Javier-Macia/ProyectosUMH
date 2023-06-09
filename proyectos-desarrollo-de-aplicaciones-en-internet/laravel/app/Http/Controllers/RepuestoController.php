<?php
namespace App\Http\Controllers;
use App\Http\Controllers\Controller;
use DB;

class RepuestoController extends Controller{
    public function ver($referencia = 1){
        $repuestos = DB::select("select * from Repuestos where referencia = $referencia");
        return view('repuestos.ver', ['repuestos' => $repuestos]);
    }
}?>