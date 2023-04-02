import pygame

# For use with Polar Coordinates trigonometry Algorithm
# import math

pygame.init()

WIDTH = 1280
HEIGHT = 720
TITLE = "Bubble Text"
GAME_FONT = "fonts/freesansbold.ttf"
running = True

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption(TITLE)
clock = pygame.time.Clock()


class Bubble_Text:
    def __init__(self, text, text_size, radius, outer_color, center_color):
        self.x_vel = 2
        self.y_vel = 2

        # create font based on a true type font and size.
        font = pygame.font.Font(GAME_FONT, text_size)

        # Create initial Text to calculate target surface size.
        text_surf = font.render(text, True, outer_color)

        # Create target surface with increased size for padding. And convert it for gpu acceleration with alpha.
        padding = radius * 2
        self.surface = pygame.Surface((text_surf.get_width() + padding, text_surf.get_height() + padding)).convert_alpha()

        # get a rect from the surface.
        self.rect = self.surface.get_rect()
        self.rect.x, self.rect.y = 0, 0

        # Make sure the surface is blank and transparent.
        self.surface.fill((0, 0, 0, 0))

        # Using the outer_color text to stamp in a circle.

        # Polar Coordinates trigonometry Algorithm
        # for index in range(0, int(math.pi * radius) ):
        #     x = (math.cos(index / (radius / 2)) * radius) + radius
        #     y = (math.sin(index / (radius / 2)) * radius) + radius
        #     self.surface.blit(text_surf, (x, y))

        # Bresenham's Circle Drawing Algorithm
        # https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
        x = 0
        y = radius
        d = 3 - 2 * radius
        self.blit_symmetric_points(text_surf, radius, x, y)
        while (y >= x):
            x += 1
            if d > 0:
                y -= 1
                d = d + 4 * (x - y) + 10
            else:
                d = d + 4 * x + 6
            self.blit_symmetric_points(text_surf, radius, x, y)

        # Using the center_color stamp the text in the center.
        self.surface.blit(font.render(text, True, center_color), (radius, radius))

    def blit_symmetric_points(self, text_surf, radius, x, y):
        self.surface.blit(text_surf, (radius + x, radius + y))
        self.surface.blit(text_surf, (radius - x, radius + y))
        self.surface.blit(text_surf, (radius + x, radius - y))
        self.surface.blit(text_surf, (radius - x, radius - y))
        self.surface.blit(text_surf, (radius + y, radius + x))
        self.surface.blit(text_surf, (radius - y, radius + x))
        self.surface.blit(text_surf, (radius + y, radius - x))
        self.surface.blit(text_surf, (radius - y, radius - x))

    def update(self):
        # Move the text by adding x and y velocity, bouce off walls.
        self.rect.x += self.x_vel
        if self.x_vel > 0 and self.rect.right > screen.get_width():
            self.rect.right = screen.get_width()
            self.x_vel = -2
        elif self.x_vel < 0 and self.rect.left < 0:
            self.rect.left = 0
            self.x_vel = 2

        self.rect.y += self.y_vel
        if self.y_vel > 0 and self.rect.bottom > screen.get_height():
            self.rect.bottom = screen.get_height()
            self.y_vel = -2
        elif self.y_vel < 0 and self.rect.top < 0:
            self.rect.top = 0
            self.y_vel = 2

    def draw(self):
        screen.blit(self.surface, (self.rect.x, self.rect.y))


def input():
    global running
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False

def update():
    bubble_text.update()

def draw():
    screen.fill((0, 0, 0))
    bubble_text.draw()
    pygame.display.flip()

# Creat a bubble text with text size, thickness, outer color and inner color.
bubble_text = Bubble_Text("Bubble Text", 100, 20, (200, 100, 150), (50, 50, 150))


while running:
    input()
    update()
    draw()
    clock.tick(60)

pygame.quit()