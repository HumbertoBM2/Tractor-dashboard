# Library imports

import pygame as pg
import os
import types

# Code imports

from plot_settings import PLOT_SIZE

# Start pygame module

pg.init()

# Definitions of global constants

# Screen

FPS = 60

SCREEN_WIDTH = 1280
SCREEN_HEIGHT = 720
CAPTION = 'Tractor Dashboard'

SCREEN = pg.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))

# Assets


# Game

GAME_SPEED = 10


# Menu






# Background 


# Player




# Plot

GAME_PLOT_SIZE = tuple(element * 100 for element in PLOT_SIZE)
PLOT_POS = (SCREEN_WIDTH // 2 , SCREEN_HEIGHT // 2 - GAME_PLOT_SIZE[1] // 2)

# Game instances


