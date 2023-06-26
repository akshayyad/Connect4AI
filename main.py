import pygame
from connect4.constants import*

pygame.init()

window = pygame.display.set_mode((HEIGHT,WIDTH))
pygame.display.set_caption('A Bit Racey')

running = True

while running:
    clock = pygame.time.Clock()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.MOUSEBUTTONDOWN:
            pass

    window.fill(white)

    pygame.draw.circle(window, blue, (400, 400), 75)

    pygame.display.flip()

    

