En la práctica 1, lo que hemos hecho 
ha sido convertir un archivo con extensión .c a .cpp

De todas maneras, un programa escrito en C, debe funcionar
también en un compilador de C++ (porque C++ tiene librerías
de C). Ahora bien, las funciones de C, no funcionan en las
librerías de C++

También hemos aprendido cómo se estructura el pseudocódigo.
Como apunte importante, si tenemos un vector:
V:real[10]:

En c++ sería:
float V[11]

Para reservar memoria dinámica, en pseudocódigo:
V:real[n]

En C:
int v;
v = (float*)malloc(sizeof(float)*(n+1));

En C++:
v = new float [n+1];