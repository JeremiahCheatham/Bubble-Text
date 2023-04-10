"use strict";

// Setup the main canvas and context.
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// Set the default fill color.
ctx.fillStyle = '#000000';

// All assets to be loaded.
const resources = [
    { src: 'fonts/freesansbold.ttf', font: 'FreeSansBold' }
];

const promises = resources.map(({ src, font }) => {
    return new Promise((resolve) => {
        if (font) {
            const fontFace = new FontFace(font, `url(${src})`);
            fontFace.load().then(() => {
                document.fonts.add(fontFace);
                resolve();
            });
        } else {
            resolve();
        }
    });
});
  
const resourcesLoadedPromise = Promise.all(promises);

class BubbleText {
    constructor(text, text_size, radius, outer_color, center_color) {
        // Create canvas context and font.
        const canvas = document.createElement('canvas');
        const context = canvas.getContext('2d');
        const font = `${text_size}px FreeSansBold`;
        context.font = font;

        // Calculate canvas size and set dimensions
        const width = context.measureText(text).width + radius * 2;
        const height = text_size + radius * 2;
        canvas.width = width;
        canvas.height = height;

        // Draw bubble text onto canvas
        context.textBaseline = "top";
        context.textAlign = "left";
        context.fillStyle = outer_color;
        context.font = font;

        // Using the outer_color text to stamp in a circle.
     
        // Polar Coordinates trigonometry Algorithm
        // for (let index = 0; index < Math.PI * radius; index++) {
        //     const x = (Math.cos(index / (radius / 2)) * radius) + radius;
        //     const y = (Math.sin(index / (radius / 2)) * radius) + radius;
        //     context.fillText(text, x, y);
        // }

        // Bresenham's Circle Drawing Algorithm
        // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
        let x = 0
        let y = radius
        let d = 3 - 2 * radius
        this.blit_symmetric_points(context, text, radius, x, y);
        while (y >= x) {
            x = x + 1
            if (d > 0) {
                y = y - 1
                d = d + 4 * (x - y) + 10
            } else {
                d = d + 4 * x + 6
            }
            this.blit_symmetric_points(context, text, radius, x, y);
    
        }

        // Change to center color and stamp center.
        context.fillStyle = center_color;
        context.fillText(text, radius, radius);

        // Store canvas and dimensions in class instance
        this._canvas = canvas;
        this._width = width;
        this._height = height;
        this._velX = 2;
        this._velY = 2;
        this._posX = 0;
        this._posY = 0;
    }

    blit_symmetric_points(context, text, radius, x, y) {
        context.fillText(text, radius + x, radius + y);
        context.fillText(text, radius - x, radius + y);
        context.fillText(text, radius + x, radius - y);
        context.fillText(text, radius - x, radius - y);
        context.fillText(text, radius + y, radius + x);
        context.fillText(text, radius - y, radius + x);
        context.fillText(text, radius + y, radius - x);
        context.fillText(text, radius - y, radius - x);
    }

    get left() {
        return this._posX;
    }

    set left(value) {
        this._posX = value;
    }

    get right() {
        return this._posX + this._width;
    }

    set right(value) {
        this._posX = value - this._width;
    }

    get top() {
        return this._posY;
    }

    set top(value) {
        this._posY = value;
    }

    get bottom() {
        return this._posY + this._height;
    }

    set bottom(value) {
        this._posY = value - this._height;
    }

    update() {
        this._posX += this._velX;
        if (this._velX > 0) {
            if (this.right > canvas.width) {
                this.right = canvas.width;
                this._velX = -2;
            }
        } else {
            if (this.left < 0) {
                this.left = 0;
                this._velX = 2;
            }
        }

        this._posY += this._velY;
        if (this._velY > 0) {
            if (this.bottom > canvas.height) {
                this.bottom = canvas.height;
                this._velY = -2
            }
        } else {
            if (this.top < 0) {
                this.top = 0;
                this._velY = 2;
            }
        }
    }

    draw() {
        ctx.drawImage(this._canvas, this._posX, this._posY);
    }
}

  
resourcesLoadedPromise.then(() => {

    const myBubbleText = new BubbleText('Bubble Text', 100, 20, '#C86496', '#323296');

    function gameLoop(timeStamp) {

        // Update
        myBubbleText.update();
        
        // Clear canvas to black
        ctx.fillRect(0, 0, canvas.width, canvas.height);

        // Draw
        myBubbleText.draw();
    
        // Schedule the next animation frame
        requestAnimationFrame(gameLoop);
    }

    window.addEventListener('keydown', (event) => {
        if (event.key === 'Escape') {
            window.close();
        }
    });

    let lastTime = performance.now();
  
    // Start the game loop
    requestAnimationFrame(gameLoop);
}).catch((err) => {
    console.error('Error loading resources:', err);
});