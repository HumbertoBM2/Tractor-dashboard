# Library imports

import sys
import matplotlib.pyplot as plt
from matplotlib.backends.backend_agg import FigureCanvasAgg

# Code imports

import game_settings
from plot import run_plot


# Game loop

def game() :

    run = True
    clock = game_settings.pg.time.Clock()

    # Fetch fig from the plot module

    fig = run_plot()

    # Transform fig into canvas and get its raw data

    canvas = FigureCanvasAgg(fig)
    size = game_settings.GAME_PLOT_SIZE
    renderer = canvas.get_renderer()
    raw_data = renderer.buffer_rgba()



    while run :
        
        # Limit game FPS

        clock.tick(game_settings.FPS)

        # If close window button is pressed, execution stops

        for event in game_settings.pg.event.get() :
            if event.type == game_settings.pg.QUIT :
                sys.exit()

        # Draw background and tractor


        # Fetch a new fig from the plot module

        fig = run_plot()

        # Redraw canvas

        canvas.draw()

        # Transform canvas' raw data into surface

        surf = game_settings.pg.image.frombuffer(raw_data.tobytes(), size, "RGBA")

        # Draw plot
        
        game_settings.SCREEN.blit(surf, game_settings.PLOT_POS)
    
        # Update display

        game_settings.pg.display.update()

def run_game() :

    # Set game window

    game_settings.pg.display.set_caption(game_settings.CAPTION)

    # Call menu method

    game()

    # Quit game

    game_settings.pg.quit()

if __name__ == "__main__":
    run_game()
