import matplotlib.pyplot as plt

# Lista para almacenar los valores
valores = []

# Leer el archivo línea por línea y guardar los valores
with open('../hola', 'r') as archivo:
    for linea in archivo:
        # Convertir la línea a float y agregarlo a la lista de valores
        valores.append(float(linea.strip()))

# Crear una lista de índices para el eje x
indices = list(range(1, len(valores) + 1))

# Graficar los valores
plt.plot(indices, valores, marker='o', linestyle='-')
plt.xlabel('Línea')
plt.ylabel('Valor')
plt.title('Valores del Archivo')
plt.grid(True)
plt.savefig('grafico_valores.png')
plt.show()

