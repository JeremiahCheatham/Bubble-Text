{-# LANGUAGE OverloadedStrings #-}

-- Load required modules.
import qualified SDL
import qualified SDL.Font
import Foreign.C.Types
import Control.Monad.State
import qualified Data.Text
import Data.Word (Word32)


-- Define a new GameData record.
data GameData = GameData
    { gameRenderer :: SDL.Renderer
    , gameBubbleText :: SDL.Texture
    , gameBubbleTextDim :: SDL.V2 CInt
    , gameFps :: Float }


-- Define a new GameState record.
data GameState = GameState
    { gameBubbleTextRect :: SDL.Rectangle CInt
    , gameVelX :: CInt
    , gameVelY :: CInt
    , gameOver :: Bool
    , gameLastTime :: Word32
    , gameCarryDelay :: Float
    , gameDeltaTime :: Float
    , gameFpsTime :: Word32
    , gameFpsCount :: Word32
    , gameFpsEnable :: Bool }


defaultWindow :: SDL.WindowConfig
defaultWindow = SDL.WindowConfig
    { SDL.windowBorder          = True
    , SDL.windowHighDPI         = False
    , SDL.windowInputGrabbed    = False
    , SDL.windowMode            = SDL.Windowed
    , SDL.windowGraphicsContext = SDL.NoGraphicsContext
    , SDL.windowPosition        = SDL.Wherever
    , SDL.windowResizable       = False
    , SDL.windowInitialSize     = SDL.V2 1280 720
    , SDL.windowVisible         = True
    }


-- Load all images and sounds assets and return GameData record.
createGameData :: SDL.Renderer -> IO GameData
createGameData renderer = do
    let outerColor = (SDL.V4 200 100 150 255)
        centerColor = (SDL.V4 50 50 150 255)
    (bubbleText, bubbleTextDim) <- createBubbleText renderer "Bubble Text" 100 20 outerColor centerColor
    return GameData 
        { gameRenderer = renderer
        , gameBubbleText = bubbleText
        , gameBubbleTextDim = bubbleTextDim
        , gameFps = 1000 / 60 }


-- Load all state assets and return GameState record.
createGameState :: GameData -> IO GameState
createGameState gameData = do
    let bubbleTextDim = gameBubbleTextDim gameData
    let bubbleTextRect = SDL.Rectangle (SDL.P (SDL.V2 0 0)) bubbleTextDim
    return GameState 
        { gameBubbleTextRect = bubbleTextRect
        , gameVelX = 120
        , gameVelY = 120
        , gameOver = False
        , gameLastTime = 0
        , gameCarryDelay = 0
        , gameDeltaTime = 0
        , gameFpsTime = 0
        , gameFpsCount = 0
        , gameFpsEnable = False }


blit :: SDL.Surface -> SDL.Surface -> CInt -> CInt -> IO ()
blit textSurf targetSurf x y = do
    void $ SDL.surfaceBlit textSurf Nothing targetSurf $ Just $ SDL.P (SDL.V2 x y)


-- Polar Coordinates trigonometry Algorithm
-- polarCoordinates :: SDL.Surface -> SDL.Surface -> Float -> IO ()
-- polarCoordinates textSurf targetSurf radius = do
--     let numPoints = round (pi * radius) :: Int
--     forM_ [0..numPoints] $ \index -> do
--         let x = (cos (fromIntegral index / (radius / 2)) * radius) + radius
--             y = (sin (fromIntegral index / (radius / 2)) * radius) + radius
--         blit textSurf targetSurf (round x) (round y)


-- Bresenham's Circle Drawing Algorithm
-- https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
bresenhamsAlgorithm :: SDL.Surface -> SDL.Surface -> CInt -> IO ()
bresenhamsAlgorithm textSurf targetSurf radius = do
    let x = 0
        y = radius
        d = 3 - 2 * radius
    drawSymmetricPoints textSurf targetSurf radius x y
    loop x y d
    where loop x y d
            | y < x     = return ()
            | otherwise = do
                let x' = x + 1
                let (y', d') = if d > 0
                    then (y - 1, d + 4 * (x' - y) + 10)
                    else (y, d + 4 * x' + 6)
                drawSymmetricPoints textSurf targetSurf radius x y
                loop x' y' d'


drawSymmetricPoints :: SDL.Surface -> SDL.Surface -> CInt -> CInt -> CInt -> IO ()
drawSymmetricPoints textSurf targetSurf radius x y = do
    blit textSurf targetSurf (radius + x) (radius + y)
    blit textSurf targetSurf (radius + x) (radius - y)
    blit textSurf targetSurf (radius - x) (radius + y)
    blit textSurf targetSurf (radius - x) (radius - y)
    blit textSurf targetSurf (radius + y) (radius + x)
    blit textSurf targetSurf (radius + y) (radius - x)
    blit textSurf targetSurf (radius - y) (radius + x)
    blit textSurf targetSurf (radius - y) (radius - x)


createBubbleText :: SDL.Renderer -> String -> Int -> CInt -> SDL.Font.Color -> SDL.Font.Color -> IO (SDL.Texture, SDL.V2 CInt)
createBubbleText renderer msg size radius outerColor centerColor = do
    font <- SDL.Font.load "fonts/freesansbold.ttf" size
    let text = Data.Text.pack msg
    outerSurf <- SDL.Font.blended font outerColor text
    (SDL.V2 width height) <- SDL.surfaceDimensions outerSurf
    let padding = radius * 2
    targetSurf <- SDL.createRGBSurface (SDL.V2(width + padding) (height + padding)) SDL.RGBA8888
    -- Using the outerColor text to stamp in a circle.
    bresenhamsAlgorithm outerSurf targetSurf radius
    -- polarCoordinates outerSurf targetSurf (fromIntegral radius)
    centerSurf <- SDL.Font.blended font centerColor text
    _ <- SDL.surfaceBlit centerSurf Nothing targetSurf $ Just $ SDL.P (SDL.V2 radius radius)
    dim <- SDL.surfaceDimensions targetSurf
    tex <- SDL.createTextureFromSurface renderer targetSurf
    SDL.Font.free font
    SDL.freeSurface outerSurf
    SDL.freeSurface centerSurf
    SDL.freeSurface targetSurf
    return (tex, dim)


-- Check all the events and return whether escape pressed or quit was requested.
checkKeysPressed :: [SDL.Event] -> Bool
checkKeysPressed events = foldr checkKey False events
    where checkKey event quit =
            case SDL.eventPayload event of
                SDL.KeyboardEvent keyboardEvent ->
                    if SDL.keyboardEventKeyMotion keyboardEvent == SDL.Pressed then
                        case SDL.keysymKeycode (SDL.keyboardEventKeysym keyboardEvent) of
                            SDL.KeycodeEscape -> True
                            _ -> quit
                    else quit
                SDL.QuitEvent -> True
                _ -> quit


updateBubbleText :: StateT GameState IO ()
updateBubbleText = do
    gameState <- get
    let SDL.Rectangle (SDL.P (SDL.V2 x y)) (SDL.V2 w h) = gameBubbleTextRect gameState
        velX = gameVelX gameState
        velY = gameVelY gameState
        deltaTime = gameDeltaTime gameState
    let x' = x + ( round ( fromIntegral velX * deltaTime ))
    let velX' = if velX > 0 then
                    if x' + w > 1280 then -120
                    else 120
                else
                    if x' < 0 then 120
                    else -120
    let y' = y + ( round ( fromIntegral velY * deltaTime ))
    let velY' = if velY > 0 then
                    if y' + h > 720 then -120
                    else 120
                else
                    if y' < 0 then 120
                    else -120
    put gameState { gameBubbleTextRect = SDL.Rectangle (SDL.P (SDL.V2 x' y')) (SDL.V2 w h)
                  , gameVelX = velX', gameVelY = velY' }


drawBubbleText :: GameData -> StateT GameState IO ()
drawBubbleText gameData = do
    gameState <- get
    let bubbleTextRect = gameBubbleTextRect gameState
    let renderer = gameRenderer gameData
        bubbleText = gameBubbleText gameData

    -- Draw bubble text.
    SDL.copy renderer bubbleText Nothing (Just bubbleTextRect)


fpsPrint :: StateT GameState IO ()
fpsPrint = do
    gameState <- get
    when (gameFpsEnable gameState) $ do
        let fpsTime = gameFpsTime gameState
            fpsCount = gameFpsCount gameState
        currentTime <- SDL.ticks
        (newTime, newCount) <-
            if (currentTime >= fpsTime ) then do
                if ((currentTime - 1000) > fpsTime) then do
                    liftIO $ putStrLn $ show fpsCount
                    return (fpsTime + 1000, 1)
                else 
                    return (fpsTime , fpsCount + 1)
            else return (fpsTime, fpsCount + 1)
        put gameState { gameFpsTime = newTime, gameFpsCount = newCount }


fpsDelay :: Float -> StateT GameState IO ()
fpsDelay frameDelay = do
    gameState <- get
    let lastTime = gameLastTime gameState
        carryDelay = gameCarryDelay gameState
    
    firstTime <- SDL.ticks
    (newCarryDelay, newTime, newElapsedTime) <-
        if (firstTime >= lastTime) then do
            let elapsedTime = fromIntegral (firstTime - lastTime)
            if ((frameDelay + carryDelay) > elapsedTime) then do
                let delay = frameDelay - elapsedTime + carryDelay
                SDL.delay $ round delay
                secondTime <- SDL.ticks
                if (secondTime >= lastTime) then do
                    let secondElapsedTime = fromIntegral (secondTime - lastTime)
                    return ((frameDelay - secondElapsedTime + carryDelay), secondTime, secondElapsedTime)
                else return (0, secondTime, elapsedTime)
            else return ((frameDelay - elapsedTime + carryDelay), firstTime, elapsedTime)
        else return (0, firstTime, 0)

    let newCarryDelay' =
            if (newCarryDelay > frameDelay) then frameDelay
            else if (newCarryDelay < (-frameDelay)) then (-frameDelay)
            else newCarryDelay
    
    let deltaTime = newElapsedTime / 1000

    put gameState { gameLastTime = newTime
                  , gameCarryDelay = newCarryDelay'
                  , gameDeltaTime = deltaTime }


-- Main game loop using gameState.
gameLoop :: GameData -> StateT GameState IO ()
gameLoop gameData = do
    -- Get current events and check for key pressed events.
    events <- SDL.pollEvents
    let quitRequest = ( checkKeysPressed events )

    updateBubbleText

    -- Clear the renderer
    SDL.clear $ gameRenderer gameData

    -- Draw the bubble text.
    drawBubbleText gameData

    -- Present the renderer/flip back and front buffers.
    SDL.present $ gameRenderer gameData

    fpsPrint
    fpsDelay $ gameFps gameData

    -- Loop unless escaped pressed.
    unless quitRequest (gameLoop gameData)


-- Free all asset memory and shutdown SDL.
cleanup :: GameData -> SDL.Window -> IO ()
cleanup gameData window = do
    let renderer = gameRenderer gameData
        bubbleText = gameBubbleText gameData
    SDL.destroyTexture bubbleText
    SDL.destroyRenderer renderer
    SDL.destroyWindow window
    SDL.Font.quit


main :: IO ()
main = do
    -- Initialize SDL and SDL.TTF for use.
    SDL.initializeAll
    SDL.Font.initialize

    -- Create the window and renderer.
    window <- SDL.createWindow "Bubble Text" defaultWindow
    renderer <- SDL.createRenderer window (-1) SDL.defaultRenderer

    -- Load all images and sounds assets and return GameData record.
    gameData <- createGameData renderer

    -- Load all state assets and return GameState record.
    gameState <- createGameState gameData

    -- Start the main game loop using the gameState.
    evalStateT (gameLoop gameData) gameState

    -- Free all asset memory and shutdown SDL.
    cleanup gameData window