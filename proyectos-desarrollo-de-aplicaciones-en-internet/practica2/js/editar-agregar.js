var textStart = document.querySelectorAll('.text-start');
var textEnd = document.querySelectorAll('.text-end');

function reescalar(){
    // Si el ancho de la pantalla es menor a 768px
    if (window.innerWidth < 768) {
        textStart.forEach(element => {
            element.classList.remove('text-start');
            element.classList.add('text-center');
        }
        );
        textEnd.forEach(element => {
            element.classList.remove('text-end');
            element.classList.add('text-center');
        }
        );
    }
    // Si el ancho de la pantalla es mayor a 768px
    else {
        textStart.forEach(element => {
            element.classList.remove('text-center');
            element.classList.add('text-start');
        }
        );
        textEnd.forEach(element => {
            element.classList.remove('text-center');
            element.classList.add('text-end');
        }
        );
    }
}

addEventListener('resize', function () {
    reescalar();
});

addEventListener('load', function () {
    reescalar();
});
