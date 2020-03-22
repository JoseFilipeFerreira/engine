#!/bin/python3
#Data for moons from: https://github.com/devstronomy/nasa-data-scraper/blob/master/data/csv/satellites.csv

import json
import os
import sys
import random
import csv

os.chdir(sys.path[0])

class Astro:
    def __init__(self, name, radius, distance, red, green, blue, moons):
        self.name = name
        self.radius = radius
        self.distance = distance
        self.red = red
        self.green = green
        self.blue = blue
        self.moons = moons

    def print_astro(self, indent=7):
        print(' ' * indent, '<!-- {} -->'.format(self.name))
        print(' ' * indent, '<group R="{0} G="{1}" B="{2}" A="{3}">'.format(self.red, self.green, self.blue, 1))
        print(' ' * indent, '    <rotate axisX="0" axisY="1" axisZ="0" angle="{0}"/>'.format(random.uniform(0,360)))
        print(' ' * indent, '    <translate X="0" Y="0" Z="{0}" />'.format(self.distance))
        print(' ' * indent, '    <scale X="{0}" Y="{0}" Z="{0}" />'.format(self.radius))
        print(' ' * indent, '    <models>')
        print(' ' * indent, '        <model file="models/sphere.3d"/>')
        print(' ' * indent, '    </models>')
        for m in self.moons:
            m.print_astro(indent+4)
        print(' ' * indent, '</group>')

def draw_asteroid_belt(number, min_dist, max_dist, size):
    print('        <!-- Astteroid belt -->')
    print('        <group R="1" G="0.5" B="0.5">')
    for i in range(number - 1):
        Astro(f'asteroid {i}', size, random.uniform(min_dist, max_dist), 0, 1, 0, []).print_astro(11)
    print('        </group>')

def generate_moons(planet_name, min_dist, max_dist):
    moons = []
    with open('satellites.csv','rt')as f:
        data = csv.reader(f)
        for row in data:
            if(row[0] == planet_name):
                moons.append(Astro(row[1], 0.1, random.uniform(min_dist, max_dist), 1, 0, 0, []))
    return moons

Mercury = Astro('Mercury', 0.1, 6 , 0, 1, 0, [])
Venus   = Astro('Venus'  , 0.1, 7 , 0, 1, 0, [])
Earth   = Astro('Earth'  , 0.1, 8 , 0, 1, 0, generate_moons('Earth', 5, 10))
Mars    = Astro('Mars'   , 0.1, 9 , 0, 1, 0, generate_moons('Mars', 5, 10))
Jupiter = Astro('Jupiter', 0.1, 11, 0, 1, 0, generate_moons('Jupiter', 5, 10))
Saturn  = Astro('Saturn' , 0.1, 12, 0, 1, 0, generate_moons('Saturn', 5, 10))
Uranus  = Astro('Uranus' , 0.1, 13, 0, 1, 0, generate_moons('Uranus', 5, 10))
Neptune = Astro('Neptune', 0.1, 14, 0, 1, 0, generate_moons('Neptune', 5, 10))
Pluto   = Astro('Pluto'  , 0.1, 15, 0, 1, 0, generate_moons('Pluto', 5, 10))

planets = [Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto]

print('<scene>')
print('    <!--Sun-->')
print('    <group R="1" G="1" B="1">')
print('        <scale X="1" Y="1" Z="1" />')
print('        <models>')
print('            <model file="models/sphere.3d"/>')
print('        </models>')
for p in planets:
    p.print_astro()

draw_asteroid_belt(100, 10, 11, 0.01)

print('    </group>')
print('</scene>')
