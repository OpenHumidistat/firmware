#!/usr/bin/env python3
"""
Connect to Humidistat Arduino over serial and plot data in real-time.
"""
import signal
import sys
import argparse

import numpy as np
import matplotlib.pyplot as plt

from SerialReader import SerialReader


def saveto_sigint_handler(filename: str):
	def sigint_handler(signal, frame):
		print("KeyboardInterrupt caught.")
		plt.close('all')

		# Write data to text file
		print(f"Saving to {filename}...")
		np.savetxt(filename, np.array(data).transpose(), header=' '.join(sr.header), comments='')

		sys.exit(0)
	return sigint_handler


# Parse CLI arguments
parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-p", "--port", default='/dev/ttyUSB0', help="The serial port device to which the Arduino is "
                                                                 "connected.")
parser.add_argument("-b", "--baud", type=int, default=115200, help="The symbol rate of the connection.")
parser.add_argument("-o", "--output", default='data.csv.gz', help="Filename to save the data to. Will be automatically "
                                                                  "gzipped if it ends in .gz.")
args = parser.parse_args()

signal.signal(signal.SIGINT, saveto_sigint_handler(args.output))

ax_dist = [0, 0, 2, 1, 2, 2, 2, 2, 1, 1, 1]

plt.ion()
fig, axs = plt.subplots(max(ax_dist) + 1, sharex=True)
axs[-1].set_xlabel('Time (s)')
fig.show()

with SerialReader(args.port, args.baud) as sr:
	# Setup list of lists using number of columns deduced from header
	data = [[] for column in sr.header]

	# Setup empty plot
	lines = [axs[ax_dist[i]].plot([], label=column)[0] for i, column in enumerate(sr.header[1:])]

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
			line.set_xdata(np.array(data[0])/1000)
			line.set_ydata(data[i+1])

		# We need both statements for the axes to auto-scale
		for ax in axs:
			ax.relim()
			ax.autoscale_view()

		# Draw the plot
		fig.canvas.draw()
		fig.canvas.flush_events()
