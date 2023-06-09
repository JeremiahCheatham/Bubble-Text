use sdl2::VideoSubsystem;
use sdl2::render::{TextureCreator, WindowCanvas};
use sdl2::video::{Window};

use crate::{SCREEN_WIDTH, SCREEN_HEIGHT};

pub struct Sdl {
    pub canvas: sdl2::render::WindowCanvas,
    pub event_pump: sdl2::EventPump,
    pub ttf_context: sdl2::ttf::Sdl2TtfContext,
    pub texture_creator: sdl2::render::TextureCreator<sdl2::video::WindowContext>,
    pub timer_subsystem: sdl2::TimerSubsystem,
}

impl Sdl {
    pub fn new() -> Result<Self, String> {
    // Initialize SDL2
    let sdl_context = sdl2::init()?;
    let video_subsystem: VideoSubsystem = sdl_context.video()?;

    // Create a window
    let window: Window = video_subsystem.window(
        "Bubble Text", SCREEN_WIDTH, SCREEN_HEIGHT)
        .position_centered()
        .build()
        .map_err(|e| e.to_string())?;

    // Get the window's canvas
    let canvas: WindowCanvas = window.into_canvas().build().map_err(|e| e.to_string())?;

    // Load the background image
    let texture_creator: TextureCreator<_> = canvas.texture_creator();

    // Create a TTF context
    let ttf_context = sdl2::ttf::init().map_err(|e| e.to_string())?;

    // Create a timer subsystem
    let timer_subsystem = sdl_context.timer()?;

    // Create event pump.
    let event_pump = sdl_context.event_pump()?;

    Ok(Self {
        canvas,
        event_pump,
        ttf_context,
        texture_creator,
        timer_subsystem,
    })}
}