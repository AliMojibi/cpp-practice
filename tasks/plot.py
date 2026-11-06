import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv("joint_log.csv")

fig, ax = plt.subplots(3, 1, sharex=True, figsize=(8, 7))
ax[0].plot(df.t, np.rad2deg(df.q)); ax[0].axhline(45, ls="--", c="k")
ax[0].set_ylabel("q [deg]")
ax[1].plot(df.t, df.dq); ax[1].set_ylabel("dq [rad/s]")
ax[2].plot(df.t, df.u);  ax[2].set_ylabel("tau [Nm]")
ax[2].set_xlabel("t [s]")
for a in ax: a.grid(True)
plt.tight_layout()
plt.savefig("joint_sim.png", dpi=150)
plt.show()