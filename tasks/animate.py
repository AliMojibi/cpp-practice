"""Animate the billiard ball from results.csv (written by ball_on_table.cpp).

Usage:
    python animate.py                  # show the animation
    python animate.py --save ball.gif  # also save it as a GIF
"""
import argparse

import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.animation import FuncAnimation, PillowWriter
from matplotlib.patches import Rectangle

# Must match Table my_table(100, 50) in ball_on_table.cpp
TABLE_W, TABLE_H = 100.0, 50.0

parser = argparse.ArgumentParser()
parser.add_argument("--csv", default="results.csv")
parser.add_argument("--stride", type=int, default=10, help="draw every Nth sample (higher = faster)")
parser.add_argument("--trail", type=int, default=60, help="trail length in frames")
parser.add_argument("--save", help="save animation to this file (e.g. ball.gif)")
args = parser.parse_args()

df = pd.read_csv(args.csv).iloc[:: args.stride].reset_index(drop=True)

fig, ax = plt.subplots(figsize=(10, 5.5))
ax.add_patch(Rectangle((0, 0), TABLE_W, TABLE_H, facecolor="#1b6e3a", edgecolor="#5a3a1a", linewidth=8))
ax.set_xlim(-3, TABLE_W + 3)
ax.set_ylim(-3, TABLE_H + 3)
ax.set_aspect("equal")
ax.set_xlabel("x")
ax.set_ylabel("y")

(trail,) = ax.plot([], [], "-", color="white", alpha=0.4, lw=1.5)
(ball,) = ax.plot([], [], "o", color="white", markersize=12, markeredgecolor="black")
info = ax.text(0.01, 1.02, "", transform=ax.transAxes, family="monospace")


def update(i):
    start = max(0, i - args.trail)
    trail.set_data(df.x[start : i + 1], df.y[start : i + 1])
    ball.set_data([df.x[i]], [df.y[i]])
    info.set_text(f"t = {df.t[i]:7.1f}   v = ({df.vx[i]:+.2f}, {df.vy[i]:+.2f})")
    return trail, ball, info


anim = FuncAnimation(fig, update, frames=len(df), interval=20, blit=True)

if args.save:
    anim.save(args.save, writer=PillowWriter(fps=30))
    print(f"saved {args.save}")
plt.show()
