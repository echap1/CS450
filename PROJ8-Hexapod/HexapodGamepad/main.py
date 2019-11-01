import pygame

pygame.init()
clock = pygame.time.Clock()
keepPlaying = True

if pygame.joystick.get_count() < 1:
    print("Couldn't find joystick")
    exit(-1)

joystick: pygame.joystick.JoystickType
joystick = pygame.joystick.Joystick(0)
joystick.init()

while keepPlaying:
    clock.tick(60)
    for event in pygame.event.get():
        print(event)