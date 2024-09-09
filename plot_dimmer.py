#!/usr/bin/env python3

import numpy as np
# import matplotlib
# matplotlib.use("Qt5Cairo")
import matplotlib.pyplot as plt

maxw = 2250

data = np.array([
  [  0,   0],
  [ 20,  12],
  [ 36,  53],
  [ 38,  57],
  [ 60, 195],
  [ 70, 300],
  [ 80, 467],
  [ 90, 580],
  [100, 749],
  [110, 950],
  [113, 958],
  [150, 1600],
  [170, 1860],
  [180, 1970],
  [200, 2220],
  [220, 2250],
  [255, maxw]])

v = data[:, 0]
w = data[:, 1]
p = v / 255
t = p * maxw

# plt.plot(v, w, v, t, v, (1 / 2 - np.cos(p * np.pi) / 2) * maxw)(1 / 2 - np.cos(p * np.pi) / 2) * 255, w)
# plt.plot(p, (1 / 2 + np.arcsin(p * 2 - 1) / np.pi) * maxw)
plt.plot(p, w, p, t, (1 / 2 - np.cos(p * np.pi) / 2), w)
plt.axhline(y=0, linestyle=":", c="0.5")
plt.axhline(y=maxw, linestyle=":", c="0.5")
plt.xlabel("%")
plt.ylabel("W")
plt.legend(["mesuré", "théorique", "corrigé"])
plt.show()
