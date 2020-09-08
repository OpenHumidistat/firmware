#!/usr/bin/env python3
"""
Connect to Humidistat Arduino over serial and plot data in real-time.
"""
import signal
import sys

import numpy as np
import matplotlib.pyplot as plt

from SerialReader import SerialReader


def sigint_handler(signal, frame):
	print("KeyboardInterrupt caught.")
	plt.close('all')

	# Write data to text file
	print("Saving to file...")
	np.savetxt('data.csv', np.array(data).transpose(), header=' '.join(sr.header))

	sys.exit(0)


signal.signal(signal.SIGINT, sigint_handler)

num_axes = 3
ax_dist = [0, 0, 2, 1]

plt.ion()
fig, axs = plt.subplots(num_axes, sharex=True)
axs[-1].set_xlabel('Time (s)')
fig.show()

with SerialReader('/dev/ttyACM0', 19200) as sr:
	# Setup list of lists using number of columns deduced from header
	data = [[] for column in sr.header]

	# Setup empty plot
	lines = [axs[ax_dist[i]].plot([], label=sr.header[i])[0] for i, column in enumerate(data)]

	for ax in axs:
		ax.legend()
		ax.grid()

	# Synchronous loop consisting of reading lines from serial and subsequently plotting them
	while True:
		# Read all lines in the receive buffer and append them to the data list
		while sr.available():
			row = sr.readline()
			for i, column in enumerate(row):
				data[i].append(column)

		# Update the data associated with the lines
		for i, line in enumerate(lines):
			line.set_xdata(range(len(data[i])))
			line.set_ydata(data[i])

		# We need both statements for the axes to auto-scale
		for ax in axs:
			ax.relim()
			ax.autoscale_view()

		# Draw the plot
		fig.canvas.draw()
		fig.canvas.flush_events()
