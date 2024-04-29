import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

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


frames = leer_archivo("../resultados.txt", 1000)
if len(frames) > 1:
    animate_frames(frames)
else:
    print("algo falla")
