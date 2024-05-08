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
puede que haya que buscar otro acercamiento (es bastante lenta ademas) \

Nueva implementacion que genera los frames como imagenes \
es mucho mas lenta y engorrosa \

(plot) xiri@debian:~/planetas1/plot$ time python plot.py \

real	1m44.417s \
user	1m43.775s \
sys	0m0.844s \

(plot) xiri@debian:~/planetas1/plot$ time python plot.py \

real	0m47.371s \
user	0m46.745s \
sys	0m2.380s \

Repartir entre distintos procesos es una idea, generar varios videos o varios procesos generando images???? \
INCLUSO PASAR DE PYTHON E INTENTARLO CON GNUPLOT Y C

HAY UNA PRIMERA VERSION DEL CALCULO DEL VALOR DEL CAMPO GRAVITATORIO, \
-NO SE TIENEN EN CUENTA LOS PUNTOS DENTRO DE LOS PLANETAS
-ES TOTALMENTE SECUENCIAL Y PROBABLEMENTE NO MUY OPTIMIZADA
