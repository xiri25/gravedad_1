# gravedad_1

Tener en cuenta que todo se basa en las leyes de Newton por lo que fenómenos como la precesión
de mercurio no pueden ser simulados

A LA LARGA TENER LOS PUNTOS DEL CAMPO Y LOS CUERPOS DEFINIDOS POR LA MISMA STRUCT
Y POR LO TANTO EN EL MISMO ARRAY PUEDE RESULATAR ALTAMENTE BENEFICIOSO
no lo creo, se gasta mas memoria de la que me esperaba

Por el otro lado deveria considerar que hay variables que se mantienen constantes
y que por lo tanto no necesitan ser guardadas tantas veces (speed/memory tradeoff)

ES MEJOR USAR UNA CANTIDAD DE RAM FIJA Y ALTA DONDE SIMULAR UNAS CUANTAS
ITERACCIONES Y DESPUES ESCRIBIRLAS DE GOLPE QUE ESCRIBIR DESPUES DE CADA
ITERACCION (AL MENOS SIEMPRE Y CUANDO SE QUIERAN CONSERVAR LOS ESTADOS DE TODAS)

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

Si necesito mas precision, descomentar los valores del campo en la struct points2d

// Segmentation fault :)

ES BASTANTE URGENTE HACER UN setup.c
MALLOC NO LANZA NINGUN ERROR SI SE VA A ALOJAR MAS MEMORIA DE LA QUE SE DISPONE FISICAMENTE, PERO SI LA USAS
TE ENCUENTRAS CON SEG. FAULT




VERLET ES MAS PRECISO, CON EULER DE (20, 2, 1) A (200, 2, 0.1) conservo 1 decimal
con verlet 3 decimales. El caso es que estoy perdiendo mas altura cuando la orbita deberia ser circular
