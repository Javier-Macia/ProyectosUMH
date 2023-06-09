function agregarLinea(id_factura) {
    event.preventDefault();
    var url = "./agregar.php?tabla=detalle_factura&id_factura=" + id_factura;
    location.href = url;
}