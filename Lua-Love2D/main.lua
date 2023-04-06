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