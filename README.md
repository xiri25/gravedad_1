# gravedad_1

A LA LARGA TENER LOS PUNTOS DEL CAMPO Y LOS CUERPOS DEFINIDOS POR LA MISMA STRUCT
Y POR LO TANTO EN EL MISMO ARRAY PUEDE RESULATAR ALTAMENTE BENEFICIOSO

### Primer intento
Vamos con 3 cuerpos celestes con orbitas circulares, cuyos planos orbitales son
paralelos al plano que forman los 3. Es decir, un sistema en 2D para empezar

por ahora los calculos de las posiciones de los planetas tienen buena pinta,
aunque agradecerian unos cambios XD

Voy a escribir los resultados en un archivo, porque aun no me aclaro con python
El archivo tiene como formato, un frame por linea y cada coordenada de cada cuerpo
separada por espacios

sol.pos_x sol.pos_y tierra.pos_x tierra.pos_y luna.pos_x luna.pos_y \
sol.pos_x sol.pos_y tierra.pos_x tierra.pos_y luna.pos_x luna.pos_y \
...

esto deberia facilitar la lectura en python, al menos por ahora \
matplotlib necesita ffmpeg para generar un video con la animacion (sugerencia de GePeTo) \

He subido el numero de frames para ver mas cosas y creo que veo algo raro (Creo que solo son imprecisiones del plot y de float) \
la animacion se procesa en un solo core y necesita de bastante ram,
puede que haya que buscar otro acercamiento (es bastante leta ademas)
