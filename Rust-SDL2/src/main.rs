pub const SCREEN_WIDTH: u32 = 1280;
pub const SCREEN_HEIGHT: u32 = 720;

mod sdl;
mod game;
mod fps;
mod bubble_text;

fn main() -> Result<(), String> {
    // Initialize SDL
    let sdl = sdl::Sdl::new()?;

    // Create game struct
    let mut game = game::Game::new(
        sdl.event_pump,
        sdl.canvas,
        &sdl.timer_subsystem,
        &sdl.texture_creator,
        &sdl.ttf_context,
    )?;

    game.run()?;

    Ok(())
}
