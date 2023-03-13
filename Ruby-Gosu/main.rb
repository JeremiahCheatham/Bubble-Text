require 'gosu'

WIDTH = 1280
HEIGHT = 720
TITLE = "Bubble Text"
GAME_FONT = "fonts/freesansbold.ttf"

class Main < Gosu::Window
    def initialize
        super(WIDTH, HEIGHT)
        self.caption = TITLE

        # The Outer and Inter Bubble colors.
        @outer_color = Gosu::Color.new(255, 200, 100, 150)
        @center_color = Gosu::Color.new(255, 50, 50, 150)

        @bubble_text = Bubble_Text.new("Bubble Text", 100, 10, @outer_color, @center_color)
    end

    def update()
        @bubble_text.update()
    end

    def draw()
        # draw_rect(0, 0, WIDTH, HEIGHT, Gosu::Color::WHITE)
        @bubble_text.draw()
    end

    def button_down(id)
        close if id == Gosu::KbEscape
    end
end

class Bubble_Text
    # We need to create the bubble text white and then draw it as a color.
    # If we were to draw the color text directly will show slight text boarder cascade.
    def initialize(text, text_size, radius, outer_color, center_color)
        @x_vel = 2
        @y_vel = 2
        @x = 0
        @y = 0

        # Create a white color for creating the white bubble text.
        white_color = Gosu::Color.new(255, 255, 255, 255)

        # create font based on a true type font and size.
        font = Gosu::Font.new(text_size, name: GAME_FONT)

        # Create initial Text to calculate target surface size.
        text_width = font.text_width(text)
        text_height = font.height

        # The target image will need to include padding for the radius all the way around.
        padding = radius * 2

        # This will create a white background bubble by stamping the text in a circle.
        white_bubble = Gosu.render(padding + text_width.to_i, padding + text_height.to_i) do
            # Using the outer_color text stamp in a circle.
            # Polar Coordinates trigonometry Algorithm
            # for index in (-Math::PI..Math::PI).step(1.0 / radius)
            #     x = (Math.cos(index) * radius) + radius
            #     y = (Math.sin(index) * radius) + radius
            #     font.draw_text(text, x, y, 0, 1, 1, white_color, mode = :additive)
            # end

            # Bresenham's Circle Drawing Algorithm
            # https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
            x = 0
            y = radius
            d = radius
            while (y >= x)
                font.draw_text(text, radius + x, radius + y, 0, 1, 1, white_color, mode = :additive)
                font.draw_text(text, radius - x, radius + y, 0, 1, 1, white_color, mode = :additive)
                font.draw_text(text, radius + x, radius - y, 0, 1, 1, white_color, mode = :additive)
                font.draw_text(text, radius - x, radius - y, 0, 1, 1, white_color, mode = :additive)
                font.draw_text(text, radius + y, radius + x, 0, 1, 1, white_color, mode = :additive)
                font.draw_text(text, radius - y, radius + x, 0, 1, 1, white_color, mode = :additive)
                font.draw_text(text, radius + y, radius - x, 0, 1, 1, white_color, mode = :additive)
                font.draw_text(text, radius - y, radius - x, 0, 1, 1, white_color, mode = :additive)
                x += 1
                if d > 0
                    y -= 1
                    d = d + 4 * (x - y) + 10
                else
                    d = d + 4 * x + 6
                end
            end
        end

        # We will stamp the white bubble background as the target color and center stamp the center color.
        @image = Gosu.render(padding + text_width.to_i, padding + text_height.to_i) do
            white_bubble.draw(0, 0, 0, 1, 1, outer_color)
            font.draw_text(text, radius, radius, 0, 1, 1, center_color)
        end
    end

    def update()
        @x += @x_vel
        if @x_vel > 0 and @x + @image.width > WIDTH
            @x_vel = -2
        elsif @x_vel < 0 and @x < 0
            @x_vel = 2
        end

        @y += @y_vel
        if @y_vel > 0 and @y + @image.height > HEIGHT
            @y_vel = -2
        elsif @y_vel < 0 and @y < 0
            @y_vel = 2
        end
    end

    def draw()
        @image.draw(@x, @y, 0)
    end
end

Main.new.show