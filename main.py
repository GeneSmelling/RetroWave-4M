import pygame
import sys
import pyttsx3

# Initialize Pygame
pygame.init()

# Set up display resolutions
resolutions = [(1920, 1080), (960, 540), (640, 360)]
resolution = resolutions[0]  # default resolution

# Create the display
screen = pygame.display.set_mode(resolution)
pygame.display.set_caption('RetroWave-4M Splash Screen')

# Splash screen parameters
font = pygame.font.SysFont('Arial', 48)
version_text = font.render('Version 0.1.0-alpha', True, (255, 255, 255))  # white text

# Text-to-speech optional support
text_to_speech_enabled = True  # Set to False to disable
if text_to_speech_enabled:
    engine = pyttsx3.init()
    engine.say('Splash Screen Version Zero Point One Alpha')
    engine.runAndWait()

# Main loop
clock = pygame.time.Clock()
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    screen.fill((0, 0, 0))  # Fill the screen with black
    screen.blit(version_text, (resolution[0] // 2 - version_text.get_width() // 2, resolution[1] // 2 - version_text.get_height() // 2))
    pygame.display.flip()

    clock.tick(60)  # Control FPS at 60
