require("bubble_text")

function love.load()
    bubble_text = BubbleText:new("Bubble Text", 100, 20, {200, 100, 150}, {50, 50, 150})
end

function love.update()
    bubble_text:update()
end

function love.draw()
    bubble_text:draw()
end

function love.keypressed(k)
    if k == "escape" then
       love.event.quit()
    end
end

BubbleText = {
    _x = 0,
    _y = 0,
    _velX = 2,
    _velY = 2
}

function BubbleText:new(text, text_size, radius, outer_color, center_color)
    local t = {}
    setmetatable(t, { __index = self })

    -- Load and set font.
    local font = love.graphics.newFont("fonts/freesansbold.ttf", text_size)
    love.graphics.setFont(font)

    t._image = love.graphics.newCanvas(font:getWidth(text) + radius * 2, font:getHeight() +  radius *2)
    t._width, t._height = t._image:getDimensions()

    -- Rectangle is drawn to the canvas with the regular/default alpha blend mode ("alphamultiply").
    love.graphics.setCanvas(t._image)
    love.graphics.clear(0, 0, 0, 0)
    love.graphics.setBlendMode("alpha")

    love.graphics.setColor(love.math.colorFromBytes(outer_color))

    -- Using the outer_color text to stamp in a circle.
     
    -- Polar Coordinates trigonometry Algorithm
    for index = 0, (math.pi * radius) do
        x = (math.cos(index / (radius / 2)) * radius) + radius
        y = (math.sin(index / (radius / 2)) * radius) + radius
        love.graphics.print(text, x, y)
    end

    -- Bresenham's Circle Drawing Algorithm
    -- https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
    -- local x = 0
    -- local y = radius
    -- local d = 3 - 2 * radius
    -- self:blit_symmetric_points(text, radius, x, y)
    -- while (y >= x) do
    --     x = x + 1
    --     if d > 0 then
    --         y = y - 1
    --         d = d + 4 * (x - y) + 10
    --     else
    --         d = d + 4 * x + 6
    --     end
    --     self:blit_symmetric_points(text, radius, x, y)

    -- end

    love.graphics.setColor(love.math.colorFromBytes(center_color))

    love.graphics.print(text, radius, radius)

    love.graphics.setColor(1, 1, 1, 1)
    love.graphics.setCanvas()

    return t
end

function BubbleText:blit_symmetric_points(text, radius, x, y)
    love.graphics.print(text, radius + x, radius + y)
    love.graphics.print(text, radius + x, radius - y)
    love.graphics.print(text, radius - x, radius + y)
    love.graphics.print(text, radius - x, radius - y)
    love.graphics.print(text, radius + y, radius + x)
    love.graphics.print(text, radius + y, radius - x)
    love.graphics.print(text, radius - y, radius + x)
    love.graphics.print(text, radius - y, radius - x)
end

function BubbleText:left()
    return self._x
end

function BubbleText:right()
    return self._x + self._width
end

function BubbleText:top()
    return self._y
end

function BubbleText:bottom()
    return self._y + self._height
end

function BubbleText:update()
    -- Move the text by adding x and y velocity, bouce off walls.
    self._x = self._x + self._velX
    if self._velX > 0 and self:right() > love.graphics.getWidth() then
        self._x = love.graphics.getWidth() - self._width
        self._velX = -2
    elseif self._velX < 0 and self:left() < 0 then
        self._x = 0
        self._velX = 2
    end

    self._y = self._y + self._velY
    if self._velY > 0 and self:bottom() > love.graphics.getHeight() then
        self._y = love.graphics.getHeight() - self._height
        self._velY = -2
    elseif self._velY < 0 and self:top() < 0 then
        self._y = 0
        self._velY = 2
    end
end

function BubbleText:draw()
    love.graphics.draw(self._image, self._x, self._y)
end