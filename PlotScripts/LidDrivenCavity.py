import csv
import numpy as np
import matplotlib.pyplot as plt

X = np.arange(0.0, 1.0, 0.05)
Y = np.arange(0.0, 1.0, 0.05)
U, V = np.meshgrid(X, Y)

fig, ax = plt.subplots()


with open("../FluidSolverCore/out/build/debug/validation/Results/LidDrivenCavity.csv", "r") as file:
    csv_reader = csv.reader(file, delimiter=",")
    firstLine = True
    width = 33.0
    max_speed = 0.1

    x = 0
    y = 0
    for row in csv_reader:
        # Ignore the line with the labels
        if firstLine:
            firstLine = False
            continue
        
        velX = float(row[0])
        velY = float(row[1])
        ax.quiver(1.0 / width * x, 1.0 / width * y, velX, velY)

        x += 1
        if x == width:
            x = 0
            y += 1
    file.close()

plt.show()
