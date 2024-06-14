import matplotlib.pyplot as plt

# Nombre del archivo (cambia esto según sea necesario)
file_name = '../hola'

# Leer los valores del archivo
with open(file_name, 'r') as file:
    valores = [float(line.strip()) for line in file]

# Representar los valores con Matplotlib
plt.plot(valores, marker='o', linestyle='-', color='b', label='Valores', markersize = 0.25)
plt.axhline(0, color='gray', linewidth=0.5)
plt.axvline(0, color='gray', linewidth=0.5)
plt.grid(True)

# Guardar la gráfica como imagen (opcional)
# plt.savefig('grafica_valores.png')

# Mostrar la gráfica
plt.show()
plt.savefig('desmos.png')

