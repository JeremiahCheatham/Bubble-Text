![Screenshot](../screenshot.png)

# Simple Bubble Text Demo (Haskell - SDL2)
Simple Demo showing how to make Bubble Text using both Polar Coordinates trigonometry Algorithm and Bresenham's Circle Drawing Algorithm. https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/

# Build
To build the bubble-text binary you will need all required build tools needed to compile via ghc and stack. You will also need SDL2 with ttf both the libraries and headerfiles.

ArchLinux instructions.

    sudo pacman -S --needed base-devel ghc stack
    sudo pacman -S --needed sdl2 sdl2_ttf
    stack build
    stack run
