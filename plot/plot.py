import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.colors import Normalize
from matplotlib.colorbar import ColorbarBase
import numpy as np
import os
from math import log, ceil

def leer_archivo(nombre_archivo):
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

def plot(frames_vectores, frames_planetas, nombre):
    fig, ax = plt.subplots()
    ax.set_title('Quiver Plot')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_facecolor('black')

    # Concatenar todas las coordenadas x e y de todos los vectores
    all_vectores_x = np.concatenate([frame[:, 0] for frame in frames_vectores])
    all_vectores_y = np.concatenate([frame[:, 1] for frame in frames_vectores])

    # Obtener los límites de los vectores
    min_vectores_x, max_vectores_x = all_vectores_x.min(), all_vectores_x.max()
    min_vectores_y, max_vectores_y = all_vectores_y.min(), all_vectores_y.max()

    # Calcular el rango de los vectores
    x_vectores_range = max_vectores_x - min_vectores_x
    y_vectores_range = max_vectores_y - min_vectores_y

    all_planetas_x = np.concatenate([frame[:, 0] for frame in frames_planetas])
    all_planetas_y = np.concatenate([frame[:, 1] for frame in frames_planetas])

    min_planetas_x, max_planetas_x = all_planetas_x.min(), all_planetas_x.max()
    min_planetas_y, max_planetas_y = all_planetas_y.min(), all_planetas_y.max()

    x_planetas_range = max_planetas_x - min_planetas_x
    y_planetas_range = max_planetas_y - min_planetas_y

    min_x, max_x = min(min_vectores_x, min_planetas_x), max(max_vectores_x, max_planetas_x)
    min_y, max_y = min(min_vectores_y, min_planetas_y), max(max_vectores_y, max_planetas_y)
    x_range, y_range = max(x_vectores_range, x_planetas_range), max(y_vectores_range, y_planetas_range) 

    ax.set_xlim(min_x - 0.1 * x_range, max_x + 0.1 * x_range)
    ax.set_ylim(min_y - 0.1 * y_range, max_y + 0.1 * y_range)

    # Obtener los módulos de los vectores de todos los frames
    modulos = np.concatenate([frame_vector[:, 2] for frame_vector in frames_vectores])

    # Normalizar los módulos para utilizarlos como colores
    print(modulos.min(), np.percentile(modulos, 95))
    norm = Normalize(vmin=modulos.min(), vmax=np.percentile(modulos, 95))
    #cmap = plt.get_cmap('RdBu')
    cmap = plt.get_cmap('coolwarm')

    ax.set_aspect('equal', 'box')
    cb = fig.colorbar(plt.cm.ScalarMappable(cmap=cmap, norm=norm), ax=ax)
    cb.set_label('Modulo del vector')

    for i, frame_vector in enumerate(frames_vectores):
        ax.clear()
        ax.set_xlim(min_x - 0.1 * x_range, max_x + 0.1 * x_range)
        ax.set_ylim(min_y - 0.1 * y_range, max_y + 0.1 * y_range)

        ratio_radio = 0.02

        ax.quiver(frame_vector[:, 0], frame_vector[:, 1], frame_vector[:, 3], frame_vector[:, 4], color=cmap(norm(frame_vector[:, 2])), scale_units='xy', angles='xy', scale=1, linewidth=0.3, headwidth=0.5, headlength=0.7)
        ax.scatter(frame_vector[:, 0], frame_vector[:, 1], color='white', s=1)
        for planeta in frames_planetas[i]:
            ax.add_patch(plt.Circle((planeta[0], planeta[1]), planeta[2] * ratio_radio, color='yellow'))
        plt.savefig(f'images/{nombre}{i}.png', dpi=300)

frames_puntos = leer_archivo("../resultados_puntos.txt")
frames_planetas = leer_archivo("../resultados_cuerpos.txt")

plot(frames_puntos, frames_planetas, "campo")
