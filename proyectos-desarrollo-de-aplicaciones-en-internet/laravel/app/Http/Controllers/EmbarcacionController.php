<?php
namespace App\Http\Controllers;
use App\Http\Controllers\Controller;
use DB;

class EmbarcacionController extends Controller{
    public function actualizarAnyo($id, $anyo){
        DB::table('embarcaciones')
            ->where('Matricula', $id)
            ->update(['Año' => $anyo]);
            
        // Obtenemos la embarcación actualizada
        $embarcaciones = DB::table('embarcaciones')
            ->where('Matricula', $id)
            ->get();
        $actualizado = (count($embarcaciones) == 0)? false : true;
        return view('embarcaciones.actualizar', ['id' => $id, 'anyo' => $anyo, 'actualizado' => $actualizado]);
        
    }

}?>