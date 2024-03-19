# -*- coding: utf-8 -*-
"""
Created on Mon Jan 29 12:32:00 2024

@author: Stage

Depth of mixing layer reflections.
Plots for DHS breifing, 2024-01-30.

"""

import os
import pandas as pd
import numpy as np
import math
import matplotlib.pyplot as plt

def gaus(x):
    return math.exp(-0.5*x*x)

zz = list(np.linspace(0, 3, 301))

cc = [gaus(z) for z in zz]

plt,ax = plt.subplots(figsize=(2.5,3))
ax.set_xlabel('Concentration')
ax.set_ylabel('z')

ax.plot(cc,zz, label='Original')

hml = 1.75
ax.plot([0,1.2], [hml, hml],'--')
ax.plot([0,1.2], [0, 0],'k-')

zz2 = [z for z in zz if z<=hml]

cc2 = [gaus(2 * hml -z) for z in zz2]
ax.plot(cc2,zz2, label='Reflection')

cc3 = [gaus(z) + gaus(2 * hml -z) for z in zz2]
ax.plot(cc3,zz2, label='Sum')
ax.legend()