- A la larga quiero crear los archivos en el setup y usar fopen con "a" en lugar
de "w" para escribir los archivos sin borrarlos

- Los calculos que para saber como es el grid son parte del setup,
porque sim_info necesita saber cunatos puntos son simulados

- Añadir dt a la simulacion de los puntos (Tiempo entre frames)

- Colosiones entre planetas

- Distancia maxima que voy a dejar que se alejen los planetas?

- Se me proponen comprobaciones de conservacion del momento angular / energia
Puedo calcular la energia por cada franme y ver que tal

- Comprobar que tal vectoriza, hace uso de SIMD GCC el codigo de la primera implementacion
la gravedad para los cuerpos y ver que puedo hacer para mejorarlo (No solo en caso necesario,
si no siempre, quiero aprender a hacerlo en el codigo y no depender tanto de los mecanismos
automaticos del compilador) (tiene buena pinta XD)

- Hecharle un vistazo a la integracion de Verlet, en teoria es un metodo para mantener las
integraciones numericas de ecuaciones diferenciales ordinarias de segundo orden con valores
conocidos mas estables/precisas. Suponer que la implementacion actual esta bien y implementar
este algoritmo
    https://www.physics.udel.edu/~bnikolic/teaching/phys660/numerical_ode/node5.html

- Kahan Summation Algorithm y Normalizar las variables alrededor de 0 para mayor precision (idealmente -1,1)
