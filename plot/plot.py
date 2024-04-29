import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import os
from math import log, ceil

def leer_archivo(archivo, length):
    """Convierte el resultado en un array de arrays, con los valores de cada frame"""
    with open(str(archivo), "r", encoding="utf-8") as resultados:
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

def animate_frames(frames):
    """Crear una animacion y la guarda, necesita FFMPEG instalado en el sistema"""
    fig, ax = plt.subplots()

    # Extraer el número de cuerpos y el número de coordenadas por cuerpo
    num_bodies = len(frames[0]) // 2
    coordinates_per_body = 2

    # Crear una lista de puntos para cada cuerpo
    points = [ax.plot([], [], marker='o')[0] for _ in range(num_bodies)]

    def update(frame):
        for i in range(num_bodies):
            # Obtener las coordenadas x e y del cuerpo i en el frame actual
            x = frames[frame][i * coordinates_per_body]
            y = frames[frame][i * coordinates_per_body + 1]
            # Convertir las coordenadas a listas o arrays
            x = [x]
            y = [y]
            # Actualizar la posición del punto correspondiente al cuerpo i
            points[i].set_data(x, y)

        ax.set_title(f'Frame {frame}')
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_xlim(-20, 20)  # Ajusta los límites x para que se ajusten a tus datos
        ax.set_ylim(-20, 20)  # Ajusta los límites y para que se ajusten a tus datos
        ax.set_aspect('equal')  # Establecer la escala de ejes igualada

    ani = animation.FuncAnimation(fig, update, frames=len(frames), interval=200)

    # Guardar la animación como un archivo de video
    ani.save('animacion.mp4', writer='ffmpeg', fps=10)
    plt.close(fig)  # Cerrar la figura para liberar recursos

def generate_frames(frames, output_dir):
    """Genera frames de la animación y los guarda como archivos de imagen"""
    fig, ax = plt.subplots()

    # Extraer el número de cuerpos y el número de coordenadas por cuerpo
    num_bodies = len(frames[0]) // 2
    coordinates_per_body = 2

    # Crear el directorio de salida si no existe
    os.makedirs(output_dir, exist_ok=True)

    def update(frame):
        ax.clear()
        for i in range(num_bodies):
            # Obtener las coordenadas x e y del cuerpo i en el frame actual
            x = frames[frame][i * coordinates_per_body]
            y = frames[frame][i * coordinates_per_body + 1]
            ax.plot(x, y, marker='o')  # Dibuja el cuerpo i en el frame actual
        ax.set_title(f'Frame {frame}')
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_xlim(-20, 20)  # Ajusta los límites x para que se ajusten a tus datos
        ax.set_ylim(-20, 20)  # Ajusta los límites y para que se ajusten a tus datos
        ax.set_aspect('equal')  # Establecer la escala de ejes igualada

        # Guardar el frame actual como archivo de imagen
        digitos = ceil(log(len(frames)))
        frame_path = os.path.join(output_dir, f'frame_{frame:0{digitos}d}.png')
        #print(frame_path)
        fig.savefig(frame_path, bbox_inches='tight', pad_inches=0)

    for frame in range(len(frames)):
        update(frame)

    plt.close(fig)  # Cerrar la figura para liberar recursos

frames = leer_archivo("../resultados.txt", 1000)
if len(frames) > 0:
    animate_frames(frames)
    #generate_frames(frames, "animacion_frames")
else:
    print("algo falla")
