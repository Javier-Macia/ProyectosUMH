<?php
namespace App\Http\Controllers;
use App\Http\Controllers\Controller;

class ArticulosController extends Controller{
    public function ver($id){
        return view('articulos/ver', ['id' => $id]);
        //return view('articulos.ver', ['id' => $id]);
    }
}?>