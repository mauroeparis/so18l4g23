# Sistemas Operativos Laboratorio 4

## Parte 1: Censura en la lectura de archivos.

Al montar nuestro Filesystem con **cfuse** e intentar leer el manifiesto comunista, el sistema debe reemplazar nuestras _palabras prohibidas_ con XXXX.

### Día 1

Empezamos a leer la documentación de fuse. Creamos nuestro repositorio con las utils (`censorship.h` con `forbidden_words[]`) y resources.

La idea ahora es cambiar en `fat_fuse_ops` el modo en que el Filesystem nos muestra el manifiesto. para eso modificaremos `fat_fuse_read()`.
