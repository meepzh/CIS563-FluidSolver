#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import sys

spread = np.random.rand(50) * 100
center = np.ones(25) * 50
flier_high = np.random.rand(10) * 100 + 100
flier_low = np.random.rand(10) * -100
data = np.concatenate((spread, center, flier_high, flier_low), 0)

pressure_data = np.loadtxt("../build/particlestats/psr-" + sys.argv[1] + ".txt")
pressure_force_data = np.loadtxt("../build/particlestats/pfm-" + sys.argv[1] + ".txt")
velocity_data = np.loadtxt("../build/particlestats/vel-" + sys.argv[1] + ".txt")

fig, axes = plt.subplots(nrows=1, ncols=3, figsize=(18, 12))

axes[0].boxplot(pressure_data)
axes[0].set_title('Pressure')

axes[1].boxplot(pressure_force_data)
axes[1].set_title('Pressure Force')

axes[2].boxplot(velocity_data)
axes[2].set_title('Velocity')

plt.tight_layout()
plt.show()