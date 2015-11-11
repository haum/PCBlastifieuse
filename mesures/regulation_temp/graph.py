#!/bin/env python3

import matplotlib.pyplot as plt

# Liste de points
points = []

# Parsing des valeurs dans le fichier texte
with open('mesure.txt', 'r') as f:
    for line in f.readlines():

        linedata = {}
        for field in line.split('\t'):
            fieldname, value = field.strip().split('=')
            fieldname = fieldname.strip()
            value = float(value.strip())
            linedata[fieldname] = value

        points.append(linedata)


# Découpage des points qui nous intéressent
points = points[384:1024]

plt.figure(1)

# Plot de la température
plt.subplot(211)
plt.plot(
    [p['temps'] for p in points], # X
    [p['temperature'] for p in points], # Y
    'r'
)

plt.hlines(180, points[0]['temps'], points[-1]['temps'])

# Plot de la commande
plt.subplot(212)
plt.ylim((-1, 2))
plt.plot(
    [p['temps'] for p in points], # X
    [p['relai'] for p in points], # Y
    'b', drawstyle='steps'
)

# Affichage
plt.show()
