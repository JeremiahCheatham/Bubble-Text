![Screenshot](../screenshot.png)

# Simple Bubble Text Demo (Rust - SDL2)
Simple Demo showing how to make Bubble Text using both Polar Coordinates trigonometry Algorithm and Bresenham's Circle Drawing Algorithm. https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/

# Build
To build the yellow-snow binary you will need all required build tools needed to compile via cargo from rust. You will also need SDL2 with image, ttf and mixer both the libraries and headerfiles.

## ArchLinux instructions.

    sudo pacman -S --needed base-devel rust
    sudo pacman -S --needed sdl2 sdl2_ttf
    cargo build
    cargo run
