import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import Normalize

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

    return np.array(frames, dtype=object)

def plot(frames_planetas, nombre):
    fig, ax = plt.subplots()
    ax.set_title('Quiver Plot')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_facecolor('black')

    all_planetas_x = np.concatenate([frame[:, 0] for frame in frames_planetas])
    all_planetas_y = np.concatenate([frame[:, 1] for frame in frames_planetas])

    min_planetas_x, max_planetas_x = all_planetas_x.min(), all_planetas_x.max()
    min_planetas_y, max_planetas_y = all_planetas_y.min(), all_planetas_y.max()

    x_planetas_range = max_planetas_x - min_planetas_x
    y_planetas_range = max_planetas_y - min_planetas_y

    min_x, max_x = min_planetas_x, max_planetas_x
    min_y, max_y = min_planetas_y, max_planetas_y
    x_range, y_range = x_planetas_range, y_planetas_range

    ax.set_xlim(min_x - 0.1 * x_range, max_x + 0.1 * x_range)
    ax.set_ylim(min_y - 0.1 * y_range, max_y + 0.1 * y_range)

    ax.set_aspect('equal', 'box')

    for i, frame_planeta in enumerate(frames_planetas):
        ax.clear()
        ax.set_xlim(min_x - 0.1 * x_range, max_x + 0.1 * x_range)
        ax.set_ylim(min_y - 0.1 * y_range, max_y + 0.1 * y_range)

        # Asumiendo que frame_planeta tiene al menos 2 columnas (x, y)
        if frame_planeta.shape[1] > 2:
            ratio_radio = 0.02
            for planeta in frame_planeta:
                ax.add_patch(plt.Circle((planeta[0], planeta[1]), planeta[2] * ratio_radio, color='yellow'))
        else:
            for planeta in frame_planeta:
                ax.scatter(planeta[0], planeta[1], color='yellow')

        plt.savefig(f'images2/{nombre}{i}.png', dpi=300)

frames_planetas = leer_archivo("../3body")

plot(frames_planetas, "campo")

