#!/bin/python3
#Data for moons from: https://github.com/devstronomy/nasa-data-scraper/blob/master/data/csv/satellites.csv
#Data for planets distance from: https://www.nationalgeographic.org/activity/planetary-size-and-distance-comparison/ 
#Data for comet: https://www.math.ubc.ca/~cass/courses/m308-05b/projects/lim/index.html
import os
import sys
import random
import csv
from math import sqrt, tan, pi

os.chdir(sys.path[0])
SCALE = 3
AU = 2.5
EMISSIVIDADE="#202020"

def str_to_bool(s):
    return s.lower() in ("yes", "true", "t", "1")

def colour_variant(hex_colour, brightness_offset=1):
    rgb_hex = [hex_colour[x:x+2] for x in [1, 3, 5]]
    new_rgb_int = [int(hex_value, 16) + brightness_offset for hex_value in rgb_hex]
    new_rgb_int = [min([255, max([0, i])]) for i in new_rgb_int] # make sure new values are between 0 and 255
    return "#" + "".join([hex(i)[2:] for i in new_rgb_int])

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

def generate_moons(planet_name, planet_radius, rotation_time, colour):
    moons = []
    with open('satellites.csv','rt')as f:
        data = csv.DictReader(f)
        for row in data:
            if(row["planet"] == planet_name):
                dist = random.uniform(planet_radius*1.5, planet_radius*2.5)
                n_orbit = random.uniform(rotation_time * 10, rotation_time * 20)
                moons.append(Astro(row["name"], dist, colour, orbit_time=n_orbit))
    return moons

class Astro:
    def __init__(self, name, distance, colour, radius=None, has_ring=False, orbit_time=0, rotation_time=0, texture=None, y_offset=None):
        self.name = name
        if not radius:
            self.radius = astro_radius(name)
        else:
            self.radius = radius
        self.texture = texture
        self.orbit_time=orbit_time
        self.rotation_time=rotation_time
        self.distance = distance
        self.colour = colour
        self.has_ring = has_ring
        self.moons = generate_moons(name, self.radius, rotation_time, "#808080")
        self.y_offset = y_offset

    def print_ring(self, curr_radius=SCALE, indent=7):
        print(' ' * indent, f'<!-- {self.name}\'s ring -->')
        print(' ' * indent, f'<group>')
        print(' ' * indent, f'    <scale X="1.5" Z="1.5" Y="0.01" />')
        print(' ' * indent, '     <models>')
        print(' ' * indent, f'         <model colour="#FFFFFF" emis="#AAAAAA" diff="#FFFFFFAA" file="models/torus.3d" texture="textures/8k_saturn_ring_alpha.jpg"/>')
        print(' ' * indent, '     </models>')
        print(' ' * indent, '</group>')


    def print_astro(self, curr_radius=SCALE, indent=7):
        rng_r = random.uniform(0,360)
        curr_translate = self.distance / curr_radius
        curr_scale = self.radius / curr_radius
        print(' ' * indent, f'<!-- {self.name} -->')
        print(' ' * indent, f'<group>')
        print(' ' * indent, f'    <rotate axisY="1" angle="{rng_r}" time={self.orbit_time} />')
        if self.y_offset:
            print(' ' * indent, f'    <translate Z="{curr_translate}" Y="{self.y_offset}"/>')
        else:
            print(' ' * indent, f'    <translate Z="{curr_translate}" />')
        print(' ' * indent, f'    <group>')
        print(' ' * indent, f'        <rotate axisY="1"  time={self.rotation_time} />')
        print(' ' * indent, f'        <scale X="{curr_scale}" Y="{curr_scale}" Z="{curr_scale}" />')
        if self.texture:
            print(' ' * indent, f'        <model file="models/sphere.3d" emis={EMISSIVIDADE} colour="#FFFFFF" texture="{self.texture}" />')
        else:
            print(' ' * indent, f'        <model file="models/sphere.3d" emis={EMISSIVIDADE} colour="{self.colour}" />')
        for m in self.moons:
            m.print_astro(self.radius, indent+8)
        if self.has_ring:
            self.print_ring(self.radius, indent+8)
        print(' ' * indent, f'    </group>')
        print(' ' * indent, '</group>')

def draw_asteroid_belt(name, number, min_dist, max_dist, size, colour):
    print('        <!-- {} -->'.format(name))
    print('        <group>')
    for i in range(number - 1):
        dist = random.uniform(min_dist, max_dist)
        otime = random.uniform(dist * 1.7,dist * 2.5)
        y_off = random.uniform(-0.1, 0.1)
        Astro(f'asteroid {i}', dist, colour, radius=size, orbit_time=otime, y_offset=y_off).print_astro(indent=11)
    print('        </group>')


def draw_comet(name, size, period, eccentricity, perihelion, aphelion, colour):
    a = (aphelion + perihelion) / (1 + eccentricity)
    b = a * sqrt(1 - eccentricity*eccentricity)
    print(f'        <!-- {name} -->')
    print(f'        <group colour="{colour}">')
    print(f'            <translate X="{aphelion * a / (perihelion + aphelion)}"/>')
    print(f'            <translate time={period}>')
    for i in range(361, 1, -1):
        ang = i * (pi / 180)
        bottom = sqrt(b*b + a*a*tan(ang)*tan(ang))
        x = a * b / bottom
        y = a * b * tan(ang) / bottom
        if i > 90 and i <= 270:
            x = -x
            y = -y
        print(f'                <point X="{x}" Z="{y}"/>')

    print('            </translate>')
    print('            <rotate angle="-90" axisX="1" />')
    print(f'            <scale X="{size/SCALE}" Y="{size/SCALE}" Z="{size/SCALE}" />')
    print(f'            <translate Y="{-size/2}"/>')
    print(f'            <model emis={EMISSIVIDADE} file="models/teapot.3d"/>')
    print('        </group>')

def get_planets():
    with open('planets.csv','rt')as f:
        data = csv.DictReader(f)
        for row in data:
            yield Astro(
                    row["planet"],
                    SCALE + float(row["relative distance"]),
                    row["color"],
                    has_ring=str_to_bool(row["has ring"]),
                    orbit_time=float(row["orbit time (days)"])/40,
                    rotation_time=float(row["rotation time (minutes)"])/80,
                    texture=row["texture file"])

print('<scene>')
print('    <!--Skybox-->')
print('    <group>')
print(f'        <scale X="{-200*SCALE}" Y="{-200*SCALE}" Z="{-200*SCALE}"/>')
print('        <model texture="textures/8k_stars_milky_way.jpg" file="models/sphere.3d" emis="#999999" />')
print('    </group>')
print('    <!--Sun-->')
print('    <group>')
print(f'        <scale X="{SCALE}" Y="{SCALE}" Z="{SCALE}" />')
print('        <light type="POINT" />')
print('        <group>')
print(f'           <rotate axisY="1"  time=100 />')
print('             <model file="models/sphere.3d" texture="textures/8k_sun.jpg" emis="#FFFFFF" />')
print('        </group>')

draw_asteroid_belt("Asteroid belt", 200 , SCALE + 7.5 , SCALE + 8.5  , 0.015, "#808080")
draw_asteroid_belt("Kuiper Belt"  , 1000, SCALE + 100, SCALE + 103, 0.04, "#808080")

draw_comet("Halley's commet", 0.01, 76, 0.976, 0.587 * (SCALE + 2.5), 35.3 * (SCALE + 2.5), "#808080")

for p in get_planets():
    p.print_astro()

print('    </group>')
print('</scene>')
