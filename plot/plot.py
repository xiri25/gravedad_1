import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.colors import Normalize
from matplotlib.colorbar import ColorbarBase
import numpy as np
import os
from math import log, ceil

def leer_archivo_puntos(nombre_archivo):
    """Leer los puntos"""
    frames = []
    frame_actual = []

    with open(nombre_archivo, 'r', encoding="utf-8") as archivo:
        for linea in archivo:
            # Si la línea está en blanco, el frame actual ha terminado
            if linea.strip() == '':
                if frame_actual:
                    frames.append(frame_actual)
                    frame_actual = []
            else:
                # Dividir la línea en sus componentes
                valores = linea.strip().split()
                # Convertir los valores a flotantes (o int si es necesario)
                valores = [float(valor) for valor in valores]
                frame_actual.append(valores)

    # Asegurarse de agregar el último frame si hay alguno
    if frame_actual:
        frames.append(frame_actual)

    return np.array(frames)


def leer_archivo_planetas(archivo, length):
    """Convierte el resultado en un array de arrays, con los valores de cada frame"""
    with open(archivo, "r", encoding="utf-8") as resultados:
        frames_num = len(resultados.readlines())

        if (frames_num != length):
            #Error
            return np.array([])

        frames = []

        # Reset file pointer to the beginning of the file
        resultados.seek(0)

        for _, line in enumerate(resultados):
            frame = [float(value) for value in line.split()]
            frames.append(np.array(frame))

    return np.array(frames)

def plot(frames_vectores, nombre):
    fig, ax = plt.subplots()
    ax.set_title('Quiver Plot')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_facecolor('black')

    # Concatenar todas las coordenadas x e y de todos los frames
    all_x = np.concatenate([frame[:, 0] for frame in frames_vectores])
    all_y = np.concatenate([frame[:, 1] for frame in frames_vectores])

    # Obtener los límites de los ejes
    min_x, max_x = all_x.min(), all_x.max()
    min_y, max_y = all_y.min(), all_y.max()

    # Calcular el rango de los ejes
    x_range = max_x - min_x
    y_range = max_y - min_y

    # Establecer los límites de los ejes con un margen PARECE QUE SE LA PELA
    ax.set_xlim(min_x - 1 * x_range, max_x + 1 * x_range)
    ax.set_ylim(min_y - 1 * y_range, max_y + 1 * y_range)

    #print("Límites de los ejes X:", ax.get_xlim())
    #print("Límites de los ejes Y:", ax.get_ylim())


    # Obtener los módulos de los vectores de todos los frames
    modulos = np.concatenate([frame[:, 2] for frame in frames_vectores])

    # Normalizar los módulos para utilizarlos como colores
    #print(modulos.min(), modulos.max())
    norm = Normalize(vmin=modulos.min(), vmax=modulos.max())
    #cmap = plt.get_cmap('RdBu')
    cmap = plt.get_cmap('coolwarm')

    for i, frame in enumerate(frames_vectores):
        ax.clear()
     #   print("Frame:", i)
   #     for vector in frame:
      #      print("Vector:", vector)
        ax.quiver(frame[:, 0], frame[:, 1], frame[:, 3], frame[:, 4], color=cmap(norm(frame[:, 2])), scale_units='xy', angles='xy', scale=2)
        ax.scatter(frame[:, 0], frame[:, 1], color='white', s=10)
        ax.set_aspect('equal', 'box')
        cb = fig.colorbar(plt.cm.ScalarMappable(cmap=cmap, norm=norm), ax=ax)
        cb.set_label('Modulo del vector')
        plt.savefig(f'images/{nombre}{i}.png', dpi=300)
        plt.show()

frames_puntos = leer_archivo_puntos("../resultados_puntos.txt")
#print(frames_puntos[1][11])
plot(frames_puntos, "campo")
