#!/bin/python3
#Data for moons from: https://github.com/devstronomy/nasa-data-scraper/blob/master/data/csv/satellites.csv
#Data for planets distance from: https://www.nationalgeographic.org/activity/planetary-size-and-distance-comparison/ 
import json
import os
import sys
import random
import csv

os.chdir(sys.path[0])
SCALE = 3

def astro_radius(name):
    with open('planets.csv','rt')as f:
        data = csv.reader(f)
        for row in data:
            if(row[0] == name):
                return float(row[1])/60000

    with open('satellites.csv','rt')as f:
        data = csv.reader(f)
        for row in data:
            if(row[1] == name):
                size = float(row[3])
                scale = size / 60000
                if size < 1000:
                    scale *= (0.01/scale)
                return scale

def generate_moons(planet_name, min_dist, max_dist):
    moons = []
    with open('satellites.csv','rt')as f:
        data = csv.reader(f)
        for row in data:
            if(row[0] == planet_name):
                dist = random.uniform(min_dist, max_dist)
                moons.append(Astro(row[1], dist, "#FF0000"))
    return moons

class Astro:
    def __init__(self, name, distance, colour, radius=None):
        self.name = name
        if not radius:
            self.radius = astro_radius(name)
        else:
            self.radius = radius
        self.distance = distance
        self.colour = colour
        self.moons = generate_moons(name, self.radius*1.5, self.radius*2.5)

    def print_astro(self, curr_radius=1, indent=7):
        print(' ' * indent, '<!-- {} -->'.format(self.name))
        print(' ' * indent, '<group colour="{0}">'.format(self.colour))
        print(' ' * indent, '    <rotate axisX="0" axisY="1" axisZ="0" angle="{0}"/>'.format(random.uniform(0,360)))
        print(' ' * indent, '    <translate X="0" Y="0" Z="{0}" />'.format(self.distance / curr_radius))
        print(' ' * indent, '    <scale X="{0}" Y="{0}" Z="{0}" />'.format(self.radius / curr_radius))
        print(' ' * indent, '    <models>')
        print(' ' * indent, '        <model file="models/sphere.3d"/>')
        print(' ' * indent, '    </models>')
        for m in self.moons:
            m.print_astro(self.radius, indent+4)
        print(' ' * indent, '</group>')

def draw_asteroid_belt(name, number, min_dist, max_dist, size, colour):
    print('        <!-- {} -->'.format(name))
    print('        <group>')
    for i in range(number - 1):
        dist = random.uniform(min_dist, max_dist)
        Astro(f'asteroid {i}', dist, colour, radius=size).print_astro(SCALE, 11)
    print('        </group>')

Mercury = Astro('Mercury',SCALE + 1  , "#FF6347")
Venus   = Astro('Venus'  ,SCALE + 2  , "#FFA500")
Earth   = Astro('Earth'  ,SCALE + 2.5, "#4169E1")
Mars    = Astro('Mars'   ,SCALE + 4  , "#B22222")
Jupiter = Astro('Jupiter',SCALE + 13 , "#8B4513")
Saturn  = Astro('Saturn' ,SCALE + 24 , "#A0522D")
Uranus  = Astro('Uranus' ,SCALE + 49 , "#5F9EA0")
Neptune = Astro('Neptune',SCALE + 76 , "#0000CD")
Pluto   = Astro('Pluto'  ,SCALE + 90 , "#FFE4E1")

planets = [Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto]

print('<scene>')
print('    <!--Sun-->')
print('    <group colour="#FFFF00">')
print(f'        <scale X="{SCALE}" Y="{SCALE}" Z="{SCALE}" />')
print('        <models>')
print('            <model file="models/sphere.3d"/>')
print('        </models>')
for p in planets:
    p.print_astro(SCALE)

draw_asteroid_belt("Asteroid belt", 200 , SCALE + 8  , SCALE + 9  , 0.02, "#FF00AA")
draw_asteroid_belt("Kuiper Belt"  , 1000, SCALE + 100, SCALE + 103, 0.05, "#FF00AA")

print('    </group>')
print('</scene>')
