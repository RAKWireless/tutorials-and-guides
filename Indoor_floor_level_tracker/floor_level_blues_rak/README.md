# Indoor Floor-Level Tracker

## Introduction

This repository includes information about how to replicate [the first Blues Accelerator](https://dev.blues.io/accelerators/indoor-floor-level-tracker/) with WisBlock Hardware


## Some details

The only changes from the [original code](https://github.com/blues/app-accelerators/tree/main/01-indoor-floor-level-tracker) were that the `WB_pressure_sensor.h` file was added so that no more lines were added to the `main.cpp` file, and the lines for the sensor used by Blues were removed from the main file. The logic used by Blues Wireless is exactly the same.
