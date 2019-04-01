# MicrofluidicPlatform
Automated Organ-On-Chip Microfluidic Platform

Found Bugs:
- During calibration, after GUI has been running for a while, channel 3 will not move at all when given the command to calibrate. Likewise, when given the command to pump, channel 3 will not move under the same circumstance occasion. Lowering the flowrate significantly remedies the problem.

When all three channels are set to recirculate, the stage should not be moving.

In recipe creator, "s/Sample" should be "sec/Sample".

- Must still add documentation to GUI.py
