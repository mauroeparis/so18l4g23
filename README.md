# Sistemas Operativos Laboratorio 4

## Parte 1: Censura en la lectura de archivos.

Al montar nuestro Filesystem con **cfuse** e intentar leer el manifiesto comunista, el sistema debe reemplazar nuestras _palabras prohibidas_ con XXXX.

### Día 1

Empezamos a leer la documentación de fuse. Creamos nuestro repositorio con las utils (`censorship.h` con `forbidden_words[]`) y resources.

La idea ahora es cambiar en `fat_fuse_ops` el modo en que el Filesystem nos muestra el manifiesto. para eso modificaremos `fat_fuse_read()`.

### Día 2

No podíamos debuggear bien porque no entendíamos cómo usar la macro `DEBUG` y al final era sacando una flag.

**IMPORTANTE:** Desde mac no se pudo instalar fuse, tratamos usarlo a traves de ssh pero tampoco fue posible.

### Día 3

Intentamos leer todos los `read` y `write` para enteder como llegar a manipular el buffer.

Fuimos probando distintas formas de debuggear. Logramos acceder al buffer, de esta manera intentaremos acceder al manifesto y editar lo que vemos.

Otras dificultades para entender el formato que `DEBUG` pide.

Vemos como el buffer se va llenando y limpiando para mostrar todo el manifesto.

Decidimos empezar a leer todo el manifesto almacenando `char` by `char` de signo de puntuación a signo de puntuación y comparar con el `censorship`, si existe, cambia el buffer en esa dirección por la palabra censurada.

Intentamos implementar según la siguiente lógica:

```
void censorship(char *buf, size_t size){
  /*Function that change*/

  for x in forbidden_words:
    manifiesto.replace(x, "x"*len(x))

}
```

_Mezclando un poco de C con Python._

Finalmente quedó así implementado:

```
void censorship(char *buf, size_t size){
  /*Function that change*/

  int start_w = 0;

  for(int i = 0; i < size; i++){
    if(is_punt(buf[i])){
      int len = i - start_w;
      for(int j = 0; j < FW_SIZE; j++){
        char *fw = forbidden_words[j];
        if(len == strlen(fw)){
          if(memcmp(buf + start_w, fw, len) == 0){
            memset(buf + start_w, 'x', len);
            break;
          }
        }
      }

      start_w = i + 1;
    }
  }

}

bool is_punt(char b) {
  return b == ' ' || b == ',' || b == '.' || b == '\n';
}
```

Y finalmente, la parte 1 anda.

## Parte 2: Operaciones de escritura.

Implementar las funciones para crear archivos, y para escribirlos.

### Día 4

Nos dimos cuenta que para empezar con esta parte 2, nos faltaba la función `mkdir`, entre otras en nuestro repositorio.

Esto causó confusión, ya que creíamos que para hacer la creacion de archivos, teníamos que usar `create` de `fuse_operations` y eso nos demoró bastante. Hasta que, leyendo la documentación, nos dimos cuenta que teníamos que usar `mknod`, y cuando agregamos las funciones que faltaban, incluyendo `mkdir`, hacer funcionar la funcion fue fácil como explicaba la consigna.

Por eventos personales de todos los miembros del grupo, nos costó ponernos a trabajar el fin de semana el tiempo suficiente.

El problema actual es en la implementación de `write`, entender cómo funciona el seteo de clusters.

### Día 5

Luego de muchas pruebas y ejecutar el codigo de diferentes formas, la escritura de archivos seguía sin funcionar y no entendiamos porque. Al parecer haciendo pruebas, la imagen habia corrompido y por eso nuestras implementaciones no funcionaban.

Al cargar de nuevo la imagen, nos dimos cuenta que el archivo se creaba pero el contenido no se guardaba. Esto era porque la función `do_fat_file_pwrite` no estaba aún implementada.

`do_fat_file_pwrite` terminó siendo una copia de `do_fat_file_pread` pero agregando la función `fat_write_dir_entry`.

La escritura funciona pero solo para archivos que ocupen un solo cluster. Estamos investigando como hacer que archivos de 2 clusters o más puedan ser creados.
