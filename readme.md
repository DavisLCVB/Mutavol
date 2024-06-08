# Mutavol

## Descripción

**Mutavol** es un sencillo proyecto que combina un analizador léxico y un analizador
sintáctico para pseudocódigo.

El analizador léxico se encarga
de reconocer los tokens del lenguaje y el analizador sintáctico se encarga de verificar
que la secuencia de tokens sea correcta.

## Estructura

El proyecto está dividido en tres módulos o carpetas principales:

- `args`: Módulo encargado del procesamiento de los argumentos pasados al programa.
- `core`: Módulo encargado de la lógica principal del programa.
- `utilities`: Módulo encargado de funciones auxiliares así como estructuras de
  datos necesarias para el funcionamiento del programa.

## Funcionamiento

A continuación se detallarán los pasos que sigue el programa para analizar un archivo

### Fase 0: Procesamiento de argumentos

Mutavol recibe 4 parámetros de manera general, 2 de ellos corresponder a la versión y
a la ayuda mientas que los restantes corresponden al archivo de entrada y salida, este
último opcional.

La manera en que se implementó esta funcionalidad es mediante la creación del módulo
`args` que se encarga de procesar los argumentos pasados al programa mediante el uso
de banderas o *flags*.

Las banderas posibles del programa se detallan a continuación:

- `-h` o `--help`: Muestra la ayuda del programa.
- `-v` o `--version`: Muestra la versión del programa.
- `-i` o `--input`: Especifica el archivo de entrada.
- `-o` o `--output`: Especifica el archivo de salida.

\* Si no se especifica un archivo de salida, el programa imprimirá en la salida
estándar.

Si el programa recibe una bandera no reconocida, se mostrará un mensaje de error y se
cerrará. De igual forma, si el programa recibe una bandera que requiere un argumento o
si no se especifica una bandera que lo requiera, se finalizará la ejecución del
programa.

***Nota***: Más información en [Args](./docs/args.md).

#### Características del código:

- Se ha empleado el patrón de diseño *Singleton* para la clase `Args`.
- El código permite la especificación de archivos tanto para sistemas Windows como
  para sistemas Linux.

### Fase 1: Lectura de archivo

Una vez que el módulo `args` ha procesado los argumentos, se procede a leer el archivo
especificado en la bandera `--i`.

El archivo es leído y limpiado por un submódulo de `core` llamado `reader`. Este
módulo se complementa con una estructura de datos llamada `buffer` que se encuentra en
su propio submódulo dentro del `core`. La estructura `buffer` se encarga de almacenar
el contenido del archivo leído además de información sobre la posición del cáracter en
el archivo.

- Paso 1: Se verifica la existencia del archivo mediante la función `verify()` de reader.
- Paso 2: En la función `read_file()` se convierte el archivo a un
  `std::vector<wchar_t>` donde están almacenados los caracteres del archivo.
- Paso 3: Se crea un buffer con el contenido del archivo, agregando datos como la
  posición del cáracter en el archivo (estructura `Position`).
- Paso 4: Se realiza una limpieza en la que se eliminan comentarios de una línea (`//`)
  y comentarios de bloque (`/* */`), además de suprimir líneas vacías.

<div style="text-align: center;">
<img src="resources/file_to_buff.png" alt="Conversión de archivo a Buffer">
</div>

#### Características del código:

- La clase `Reader` implementa el patrón de diseño *Singleton*.
- La clase `Buffer` implementa el patrón de diseño *Singleton*, sin embargo, la clase
  hace uso de los templates por lo que cada tipo con la que se use, generará una nueva
  instancia *Singleton*.
- Se crearon estructuras de datos auxiliares como listas enlazadas (`LinkedList`) y
  `Position` para el manejo de la información del archivo.
- Para las listas enlazadas se implementó el patrón de diseño *Iterator*.
- Se implementó el patrón de diseño *Factory* para la creación de instancia de `Node`
  y `LinkedList`.

***Nota***: Más información en [Reader](./docs/reader.md) y [Buffer](./docs/buffer.md).