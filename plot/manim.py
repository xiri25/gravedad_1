from manim import *

class GravitationalField(Scene):
    def construct(self):
        # Array de frames con vectores

        # frame = [{posx, posy, normx, normy, mod},{}]

        frames = []

        for i in range(100):
            normx = [1,2,3,5,6,7,8,9,0,1,2]
            normy = [3,6,7,3,2,1,5,6,8,6,0]
            mod   = [5,7,4,7,3,2,8,9,1,0,4]
            frames.append([{"posx": 0,  "posy": 0, "normx": normx[0]*i,  "normy": normy[0]*i,  "mod": mod[0]*i},
                         {"posx": 1,  "posy": 0, "normx": normx[1]*i,  "normy": normy[1]*i,  "mod": mod[1]*i},
                         {"posx": 2,  "posy": 0, "normx": normx[2]*i,  "normy": normy[2]*i,  "mod": mod[2]*i},
                         {"posx": 3,  "posy": 0, "normx": normx[3]*i,  "normy": normy[3]*i,  "mod": mod[3]*i},
                         {"posx": 4,  "posy": 0, "normx": normx[4]*i,  "normy": normy[4]*i,  "mod": mod[4]*i},
                         {"posx": 5,  "posy": 0, "normx": normx[5]*i,  "normy": normy[5]*i,  "mod": mod[5]*i},
                         {"posx": 6,  "posy": 0, "normx": normx[6]*i,  "normy": normy[6]*i,  "mod": mod[6]*i},
                         {"posx": 7,  "posy": 0, "normx": normx[7]*i,  "normy": normy[7]*i,  "mod": mod[7]*i},
                         {"posx": 8,  "posy": 0, "normx": normx[8]*i,  "normy": normy[8]*i,  "mod": mod[8]*i},
                         {"posx": 9,  "posy": 0, "normx": normx[9]*i,  "normy": normy[9]*i,  "mod": mod[9]*i},
                         {"posx": 10, "posy": 0, "normx": normx[10]*i, "normy": normy[10]*i, "mod": mod[10]*i}])

        """
        frames = [
            [{"posx": 2, "posy": 2, "normx": 0.5, "normy": -0.5, "mod": 1.0},
             {"posx": 3, "posy": 2, "normx": -0.5, "normy": 0.5, "mod": 1.0}],
            # Añade más frames según sea necesario
        ]
        """

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
