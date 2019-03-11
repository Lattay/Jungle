#!/usr/bin/env python
from sketcher import sketch, Sketch
from sketcher.common import Color
import struct
import sys

if len(sys.argv) >= 2:
    filename = sys.argv[1]
else:
    filename = './out.bin'
w, h = 800, 800


class Indiv:
    def __init__(self, sp, fm, x, y):
        self.pos = (w*x, h*y)
        self.spec = sp
        self.fam = fm


@sketch
class Sk(Sketch):
    # you do not need to inherit from sketch but it help auto-completion
    def setup(self):
        spset = set()
        fmset = set()
        self.frame(0.1)
        self.size(w, h)
        self.step = 0
        tab = []
        atop = 0
        with open(filename, 'rb') as f:
            res = f.read(12)
            while len(res) == 12:
                if res[:8] == b'\x00'*8:
                    # header
                    (atop,) = struct.unpack('i', res[8:])
                    tab.append([])
                else:
                    sp, fm, x, y = struct.unpack('hhff', res)
                    spset.add(sp)
                    fmset.add(fm)
                    if not (0 <= x <= 1) or not (0 <= y <= 1):
                        print("Bug", x, y)
                    tab[-1].append(Indiv(sp, fm, x, y))
                res = f.read(12)
            if atop > len(tab[-1]):
                tab.pop()
        self.tab = tab
        self.background('black')
        self.stroke_color('red')
        self.spcolor = {}
        for sp in spset:
            self.spcolor[sp] = Color(self.rd.randint(50, 255) for _ in range(3))
        self.no_stroke()

    def loop(self):
        self.clear()
        if self.step < len(self.tab):
            print(self.step)
            for indiv in self.tab[self.step]:
                self.fill_color(self.spcolor[indiv.spec])
                self.ellipse(*indiv.pos, 2)
            self.step += 1
        else:
            self.step = 0
