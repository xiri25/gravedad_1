import matplotlib.pyplot as plt

# Nombre del archivo (cambia esto según sea necesario)
file_name = '../hola'

# Leer los valores del archivo
posiciones_planeta1 = []
posiciones_planeta2 = []

with open(file_name, 'r') as file:
    lines = file.readlines()
    for i in range(0, len(lines), 3):
        if i + 1 < len(lines):
            x1, y1 = map(float, lines[i].strip().split())
            x2, y2 = map(float, lines[i + 1].strip().split())
            posiciones_planeta1.append((x1, y1))
            posiciones_planeta2.append((x2, y2))

# Separar las posiciones en listas de coordenadas x e y
x1, y1 = zip(*posiciones_planeta1)
x2, y2 = zip(*posiciones_planeta2)

# Representar las trayectorias de los planetas con Matplotlib
plt.figure(figsize=(10, 10))
plt.plot(x1, y1, marker='o', linestyle='-', color='b', markersize=4, label='Planeta 1')
plt.plot(x2, y2, marker='o', linestyle='-', color='r', markersize=4, label='Planeta 2')
plt.axhline(0, color='gray', linewidth=0.5)
plt.axvline(0, color='gray', linewidth=0.5)
plt.title('Trayectorias de los planetas')
plt.xlabel('Posición X')
plt.ylabel('Posición Y')
plt.legend()
plt.grid(True)
plt.axis('equal')

# Guardar la gráfica como imagen (opcional)
# plt.savefig('trayectorias_planetas.png')

# Mostrar la gráfica
plt.savefig("desmos2.png")

