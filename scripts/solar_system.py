#!/bin/python3
#Data for moons from: https://github.com/devstronomy/nasa-data-scraper/blob/master/data/csv/satellites.csv
#Data for planets distance from: https://www.nationalgeographic.org/activity/planetary-size-and-distance-comparison/ 
import os
import sys
import random
import csv

os.chdir(sys.path[0])
SCALE = 3

def astro_radius(name):
    with open('planets.csv','rt')as f:
        data = csv.DictReader(f)
        for row in data:
            if(row["planet"] == name):
                return float(row["radius"])/60000

    with open('satellites.csv','rt')as f:
        data = csv.DictReader(f)
        for row in data:
            if(row["name"] == name):
                size = float(row["radius"])
                scale = size / 60000
                if size < 1000:
                    scale *= (0.01/scale)
                return scale

def generate_moons(planet_name, planet_radius, colour):
    moons = []
    with open('satellites.csv','rt')as f:
        data = csv.DictReader(f)
        for row in data:
            if(row["planet"] == planet_name):
                dist = random.uniform(planet_radius*1.5, planet_radius*2.5)
                moons.append(Astro(row["name"], dist, colour))
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
        self.moons = generate_moons(name, self.radius, "#808080")

    def print_astro(self, curr_radius=SCALE, indent=7):
        rng_r = random.uniform(0,360)
        curr_translate = self.distance / curr_radius
        curr_scale = self.radius / curr_radius
        print(' ' * indent, f'<!-- {self.name} -->')
        print(' ' * indent, f'<group colour="{self.colour}">')
        print(' ' * indent, f'    <rotate axisX="0" axisY="1" axisZ="0" angle="{rng_r}"/>')
        print(' ' * indent, f'    <translate X="0" Y="0" Z="{curr_translate}" />')
        print(' ' * indent, f'    <scale X="{curr_scale}" Y="{curr_scale}" Z="{curr_scale}" />')
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
        Astro(f'asteroid {i}', dist, colour, radius=size).print_astro(indent=11)
    print('        </group>')


def get_planets():
    with open('planets.csv','rt')as f:
        data = csv.DictReader(f)
        for row in data:
            yield Astro(row["planet"], SCALE + float(row["relative distance"]), row["color"])

print('<scene>')
print('    <!--Sun-->')
print('    <group colour="#FFFF00">')
print(f'        <scale X="{SCALE}" Y="{SCALE}" Z="{SCALE}" />')
print('        <models>')
print('            <model file="models/sphere.3d"/>')
print('        </models>')
for p in get_planets():
    p.print_astro()

draw_asteroid_belt("Asteroid belt", 200 , SCALE + 8  , SCALE + 9  , 0.02, "#808080")
draw_asteroid_belt("Kuiper Belt"  , 1000, SCALE + 100, SCALE + 103, 0.05, "#808080")

print('    </group>')
print('</scene>')
