<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class Cliente extends Model
{
    //
    protected $table = 'Embarcaciones';

    public $timestamps = false; // Nosotros no tenemos los campos created_at y updated_at en las tablas, y por eso ponemos esto en false
}
