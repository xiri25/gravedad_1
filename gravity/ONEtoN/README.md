# README
Simulación en la que no se tienen en cuenta las interacciones entre los distintos cuerpos
y en su lugar cada cuerpo orbita un foco siguiendo la solucion analitica de una orbita según
la ley de la gravitación universal. (En el futuro se podria añadir la rotación de la propia
órbita respecto al foco para poder "simular" precesión)

# APUNTES SOBRE LA IMPLEMENTACIÓN
-Por ahora estoy pensando en hacer la transformación de coordenadas, simular, y hacer la transformación
inversa para volver al espacio absoluto que estoy usando. No parece la forma más eficiente. Se me ocurre
que debería hacer la transformación de la los resultados de la iteración ("suiimulación")
-Intentar una implementación en la que se fuerce al vector momento angunlar a ser cte y se use verlet o euler
para actualizar el vector velocidad
