from manim import *

class GravitationalField(Scene):
    def construct(self):
        # Array de frames con vectores
        frames = [
            [{"posx": 1, "posy": 1, "normx": 0.5, "normy": 0.5, "mod": 1.0}, {"posx": 2, "posy": 2, "normx": -0.5, "normy": 0.5, "mod": 1.0}],
            [{"posx": 2, "posy": 2, "normx": -0.5, "normy": 0.5, "mod": 1.0}, {"posx": 3, "posy": 3, "normx": 0.5, "normy": -0.5, "mod": 1.0}],
            # Añade más frames según sea necesario
        ]

        for frame in frames:
            self.draw_frame(frame)
            self.wait()

    def draw_frame(self, vectors):
        for vector in vectors:
            pos = np.array([vector["posx"], vector["posy"], 0])
            norm = np.array([vector["normx"], vector["normy"], 0])
            mod = vector["mod"]
            color = self.get_color(mod)
            arrow = Arrow(start=pos, end=pos + norm, buff=0, color=color)
            self.add(arrow)

    def get_color(self, mod):
        # Define una función para asignar un color basado en el módulo del vector
        # Por ejemplo, podrías hacer que los vectores más largos sean más rojos, y los vectores más cortos sean más azules.
        # Aquí hay un ejemplo simple:
        return interpolate_color(RED, BLUE, mod)

# Ejecutar la animación
scene = GravitationalField()
scene.render()
