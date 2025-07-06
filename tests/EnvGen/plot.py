#!/usr/bin/env python3
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Read the CSV file
data = pd.read_csv("build/envelope_output.csv")

# Create the plot
plt.figure(figsize=(12, 6))
plt.plot(data["sample"], data["value"], "b-", linewidth=2, label="Envelope")
plt.grid(True, alpha=0.3)
plt.xlabel("Sample")
plt.ylabel("Amplitude")
plt.title("EnvGen Envelope Output (with curve formula)")
plt.legend()

# Add some annotations
plt.axhline(y=0, color="k", linestyle="-", alpha=0.3)
plt.axhline(y=1, color="r", linestyle="--", alpha=0.5, label="Peak level")

# Save the plot
plt.savefig("envelope_plot.png", dpi=300, bbox_inches="tight")
plt.show()

print("Plot saved as envelope_plot.png")
