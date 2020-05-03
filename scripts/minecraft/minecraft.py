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

        print(' ' * indent, '<group>')
        print(' ' * indent, f'    <model colour="#8B4513" file="models/box.3d"/>')
        print(' ' * indent, f'    <translate Y="{height}" />')
        print(' ' * indent, '</group>')
        
        if height > 0:
            self.draw_trunk(height -1)

    def draw_tree(self, indent=3):
        print(' ' * indent, '<group>')
        print(' ' * indent, f'    <!-- {self.name} -->')
        print(' ' * indent, f'    <translate X="{self.x}" Y="{self.y}" Z="{self.z}" />')
        self.draw_trunk(4)
        print(' ' * indent, '</group>')

class Cube:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
        r = lambda: random.randint(0,255)
        self.colour = '#%02X%02X%02X' % (r(),r(),r())
        self.name = f"cube {x} {y} {z}"

    def draw_cube(self, indent=3):
        print(' ' * indent, '<group>')
        print(' ' * indent, f'    <!-- {self.name} -->')
        print(' ' * indent, f'    <translate X="{self.x}" Y="{self.y}" Z="{self.z}" />')
        print(' ' * indent, f'    <model colour="{self.colour}" file="models/box.3d"/>')
        print(' ' * indent, '</group>')


print("<scene>")
print("    <group>")
print(f'        <scale X="-{SIZE_X + SIZE_Z}" Y="-{SIZE_X + SIZE_Z}" Z="-{SIZE_X + SIZE_Z}"/>')
print('        <model file="models/sphere.3d" colour="#45b3e0"/>')
print("    </group>")
floor_factory = PerlinNoiseFactory(2, octaves=4, unbias=True)
tree_factory = PerlinNoiseFactory(2, octaves=4)

for x in range(SIZE_X):
    for z in range(SIZE_Z):
        xa = x - SIZE_X/2
        za = z - SIZE_Z/2
        base_floor = floor(floor_factory(x/SIZE_X, z/SIZE_Z) * 10)
        Cube(xa, base_floor, za).draw_cube()

        #if tree_factory(x/SIZE_X, z/SIZE_Z) * 10 > 5:
        #    Tree(xa, base_floor + 1, za).draw_tree()


print("</scene>")
