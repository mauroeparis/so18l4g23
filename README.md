# Sistemas Operativos Laboratorio 4

## Parte 1: Censura en la lectura de archivos.

Al montar nuestro Filesystem con **cfuse** e intentar leer el manifiesto comunista, el sistema debe reemplazar nuestras _palabras prohibidas_ con XXXX.

### Día 1

Empezamos a leer la documentación de fuse. Creamos nuestro repositorio con las utils (`censorship.h` con `forbidden_words[]`) y resources.

La idea ahora es cambiar en `fat_fuse_ops` el modo en que el Filesystem nos muestra el manifiesto. para eso modificaremos `fat_fuse_read()`.

### Día 2

No podíamos debuggear bien porque no entendíamos cómo usar la macro `DEBUG` y al final era sacando una flag.

**IMPORTANTE:** Desde mac no se pudo instalar fuse.

### Día 3

Intentamos leer todos los `read` y `wait` para enteder como llegar a manipular el buffer.

Fuimos probando distintas formas de debuggear. Logramos acceder al buffer, de esta manera intentaremos acceder al manifesto y editar lo que vemos.

Otras dificultades para entender el formato que `DEBUG` pide.

Vemos como el buffer se va llenando y limpiando para mostrar todo el manifesto.

Decidimos empezar a leer todo el manifesto almacenando `char` by `char` de signo de puntuación a signo de puntuación y comparar con el `censorship`, si existe, cambia el buffer en esa dirección por la palabra censurada.

Intentamos implementar según la siguiente lógica:

```
void censorship(char *buf, size_t size){
  /*Function that change*/

  for x in forbidden_words]:
    manifiesto.replace(x, "x"*len(x))

}
```

_Mezclando un poco de C con Python._

Finalmente quedó así implementado:

```
void censorship(char *buf, size_t size){
  /*Function that change*/

  int start_w = 0;

  for(int i = 0; i < size; i++) {
    if(is_punt(buf[i])){
      int len = i - start_w;
      char *word = malloc(len * sizeof(char));
      for(int j = 0; j < len; j++) word[j] = buf[j + start_w];

      for(int j = 0; j < fw_size; j++){
        if(len != n[j])
          continue;
        if(memcmp(word, forbidden_words[j], len) == 0){
          memset(buf + start_w, 'x', len);
          break;
        }
      }
      free(word);
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

Esto causó confusión, ya que creíamos que para hacer el `mknod`, teníamos que usar `create` de `fuse_operation` y eso nos demoró bastante. Hasta que, leyendo la documentación, nos dimos cuenta que efectivamente teníamos que usar `mkdir`, y ahí cuando agregamos las funciones que faltaban, incluyendo ésta, hacer `mknod` fue fácil.

Por eventos personales de todos los miembros del grupo, nos costó ponernos a trabajar el fin de semana el tiempo suficiente.

El problema actual es en la implementación de `write`, entender cómo funciona el seteo de clusters.
