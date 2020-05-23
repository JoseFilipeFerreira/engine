#!/bin/python3
from perlin import PerlinNoiseFactory
from math import floor
import random

SIZE_X = 200
SIZE_Z = 200
BASE = -10

class Tree:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
        self.name = f"tree {x} {y} {z}"

    def draw_trunk(self, height, indent=7):

        for i in range(height):
            print(' ' * indent, '<group>')
            print(' ' * indent, f'    <model colour="#8B4513" file="models/box.3d"/>')
            print(' ' * indent, f'    <translate Y="{height - i}" />')
            print(' ' * indent, '</group>')

    def draw_tree(self, indent=3):
        print(' ' * indent, '<group>')
        print(' ' * indent, f'    <!-- {self.name} -->')
        print(' ' * indent, f'    <translate X="{self.x}" Y="{self.y}" Z="{self.z}" />')
        self.draw_trunk(4)
        print(' ' * indent, '</group>')

class Cube:
    def __init__(self, x, y, z, depth=1):
        self.x = x
        self.y = y
        self.z = z
        self.name = f"cube {x} {y} {z}"
        self.depth = depth

    def draw_cube(self, indent=3):
        for i in range(self.depth):
            print(' ' * indent, '<group colour="#FFFFFF">')
            print(' ' * indent, f'    <!-- {self.name} -->')
            print(' ' * indent, f'    <translate X="{self.x}" Y="{self.y - i}" Z="{self.z}" />')
            print(' ' * indent, f'    <model texture="textures/dirt_block.jpg" file="models/box.3d" emis="#888888"/>')
            print(' ' * indent, '</group>')


print("<scene>")
print('    <light type="DIRECTIONAL" dirY="10" colour="#101010" />')
print("    <group>")
print(f'        <scale X="-{SIZE_X + SIZE_Z}" Y="-{SIZE_X + SIZE_Z}" Z="-{SIZE_X + SIZE_Z}"/>')
print('        <model file="models/sphere.3d" texture="textures/8k_stars_milky_way.jpg" emis="#FFFFFF"/>')
print("    </group>")

floor_factory = PerlinNoiseFactory(2, octaves=64)
def get_height(x, z):
        return floor(floor_factory(x/SIZE_X, z/SIZE_Z) * 50)

for x in range(SIZE_X):
    for z in range(SIZE_Z):
        xa = x - SIZE_X/2
        za = z - SIZE_Z/2

        p0 = get_height(x-1, z)
        p1 = get_height(x, z-1)
        p2 = get_height(x+1, z)
        p3 = get_height(x, z+1)

        curr = get_height(x,z)

        max_dif = max([curr - p0, curr -  p1, curr - p2, curr - p3])

        if max_dif < 1:
            max_dif = 1

        Cube(xa, curr, za, depth=max_dif).draw_cube()

        #if tree_factory(x/SIZE_X, z/SIZE_Z) * 10 > 5:
        #    Tree(xa, base_floor + 1, za).draw_tree()

print("</scene>")
