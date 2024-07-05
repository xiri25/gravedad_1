
import matplotlib.pyplot as plt

# Nombre del archivo (cambia esto según sea necesario)
file_name = '../3body'

# Leer los valores del archivo
posiciones_planeta1 = []
posiciones_planeta2 = []
posiciones_planeta3 = []

with open(file_name, 'r') as file:
    lines = file.readlines()
    for i in range(0, len(lines), 3):
        if i + 2 < len(lines):
            line1 = lines[i].strip()
            line2 = lines[i + 1].strip()
            line3 = lines[i + 2].strip()
            
            if line1 and line2 and line3:  # Ensure lines are not empty
                x1, y1 = map(float, line1.split())
                x2, y2 = map(float, line2.split())
                x3, y3 = map(float, line3.split())
                posiciones_planeta1.append((x1, y1))
                posiciones_planeta2.append((x2, y2))
                posiciones_planeta3.append((x3, y3))

# Separar las posiciones en listas de coordenadas x e y
x1, y1 = zip(*posiciones_planeta1)
x2, y2 = zip(*posiciones_planeta2)
x3, y3 = zip(*posiciones_planeta3)

# Representar las trayectorias de los planetas con Matplotlib
plt.figure(figsize=(15, 15))
plt.plot(x1, y1, marker='o', linestyle='-', color='b', markersize=4, label='Planeta 1')
plt.plot(x2, y2, marker='o', linestyle='-', color='r', markersize=4, label='Planeta 2')
plt.plot(x3, y3, marker='o', linestyle='-', color='b', markersize=4, label='Planeta 3')
plt.axhline(0, color='gray', linewidth=0.5)
plt.axvline(0, color='gray', linewidth=0.5)
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
plt.savefig("3body.png")

