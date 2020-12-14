# TSP-Solver
## Table of Contents
* [Introduction](introduction)
* [Technologies](technologies)
* [Setup](setup)
* [Features](features)

## Introduction
The initial motivation for this project was to attempt to implement the Simulated Annealing algorithm to a
problem of my choice. The problem I chose was the Travelling Salesman Problem: given a collection of points 
in a space and the distance between them, what is the shortest path that crosses all of them exactly once.

## Technologies
The application is written in C++17 and uses SDL 2.0 along with the extension SDL_image.

## Setup
The build this project, simply open the project file in Visual Studio 2019 and build it in the architecture of your choice (x86 or x64).

## Features
* Choice of multiple distance metrics
* Can change the individual lengths between two chosen vertices
* Can select a number of vertices to move and delete

### Todo:

* Use of wxWidgets to allow for more solid GUI and easier implementation of new features
* Undo/Redo actions
* Loading and Saving graphs
* Copying selecting vertices
* Addition of L-p metrics
* Load images to overlay onto the graph
