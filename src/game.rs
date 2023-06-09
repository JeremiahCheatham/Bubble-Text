use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;

use crate::fps::Fps;
use crate::bubble_text::BubbleText;

pub struct Game<'a> {
    canvas: sdl2::render::WindowCanvas,
    event_pump: sdl2::EventPump,
    fps: Fps<'a>,
    bubble_text: BubbleText<'a>,
}

impl<'a> Game<'a> {
    pub fn new(
        event_pump: sdl2::EventPump,
        canvas: sdl2::render::WindowCanvas,
        timer_subsystem: &'a sdl2::TimerSubsystem,
        texture_creator: &'a sdl2::render::TextureCreator<sdl2::video::WindowContext>,
        ttf_context: &'a sdl2::ttf::Sdl2TtfContext,
    ) -> Result<Self, String> {

        let fps = Fps::new(60, &timer_subsystem)?;

        // The Outer and Inter Bubble colors.
        let outer_color = Color::RGBA(200, 100, 150, 0);
        let center_color = Color::RGBA(50, 50, 160, 0);

        // Create score instance.
        let bubble_text = BubbleText::new(
            &texture_creator,
            &ttf_context,
            String::from("Bubble Text"),
            100,
            20.0,
            outer_color,
            center_color
        )?;

    Ok(Self {
        canvas: canvas,
        event_pump: event_pump,
        fps: fps,
        bubble_text: bubble_text,
    })}


    pub fn run(&mut self) -> Result<(), String> {
        // Start the game loop.
        'running: loop {
            // Handle events.
            for event in self.event_pump.poll_iter() {
                match event {
                    Event::Quit {..} |
                    Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                        break 'running;
                    },
                    _ => {},
                }
            }
    
            // Update the game state.
            self.bubble_text.update();
    
            // Clear the canvas and draw the everything.
            self.canvas.clear();
            self.bubble_text.draw(&mut self.canvas)?;
    
            // Flip the buffers.
            self.canvas.present();
    
            // Delay the game if needed and save the delta_time.
            self.fps.update();
        }
        Ok(())
    }
}