use sdl2::pixels::{Color, PixelFormatEnum};
use sdl2::rect::Rect;
use sdl2::render::{Texture, TextureCreator, WindowCanvas};
use sdl2::surface::Surface;
use sdl2::ttf::Sdl2TtfContext;
use sdl2::video::WindowContext;
// use std::f64::consts::PI;

use crate::{SCREEN_WIDTH, SCREEN_HEIGHT};

pub struct BubbleText<'a> {
    texture: Texture<'a>,
    rect: Rect,
    vel_x: i32,
    vel_y: i32
}

impl<'a> BubbleText<'a> {
    pub fn new(
        texture_creator: &'a TextureCreator<WindowContext>,
        ttf_context: &'a Sdl2TtfContext,
        text: String,
        size: u16,
        radius: f64,
        outer_color: Color,
        center_color: Color
    ) -> Result<BubbleText<'a>, String> {

        let font = ttf_context.load_font("fonts/freesansbold.ttf", size)?;

        let outer_surf = font.render(&text).solid(outer_color).map_err(|e| e.to_string())?;
        let center_surf = font.render(&text).blended(center_color).map_err(|e| e.to_string())?;
        let mut dest_rect = Rect::new(
            radius as i32,
            radius as i32,
            outer_surf.width(),
            outer_surf.height()
        );

        let mut target_surf = Surface::new(
            outer_surf.width() + (radius as u32) * 2,
            outer_surf.height() + (radius as u32) * 2,
            PixelFormatEnum::ARGB8888
        ).map_err(|e| e.to_string())?;

        // Using the outer_color text to stamp in a circle.

        // Polar Coordinates trigonometry Algorithm
        // for index in 0..(PI * radius) as i32 {
        //     let x = (f64::cos(index as f64 / (radius / 2.0)) * radius) + radius;
        //     let y = (f64::sin(index as f64 / (radius / 2.0)) * radius) + radius;
        //     dest_rect.x = x as i32;
        //     dest_rect.y = y as i32;
        //     outer_surf.blit(None, &mut target_surf, Some(dest_rect))?;
        // }

        // Bresenham's Circle Drawing Algorithm
        // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
        let mut x = 0;
        let mut y = radius as i32;
        let mut d = 3 - 2 * (radius as i32);
        blit(&outer_surf, &mut target_surf, &mut dest_rect, radius as i32, x, y)?;
        while y >= x {
            x += 1;
            if d > 0 {
                y -= 1;
                d = d + 4 * (x - y) + 10;
            } else {
                d = d + 4 * x + 6;
            }
            blit(&outer_surf, &mut target_surf, &mut dest_rect, radius as i32, x, y)?;
        }

        dest_rect.x = radius as i32;
        dest_rect.y = radius as i32;
        center_surf.blit(None, &mut target_surf, Some(dest_rect))?;

        let texture = texture_creator.create_texture_from_surface(&target_surf).map_err(|e| e.to_string())?;
        let rect = Rect::new(0, 0, target_surf.width(), target_surf.height());
        let vel_x = 2;
        let vel_y = 2;
        Ok(BubbleText { texture, rect, vel_x, vel_y})
    }

    pub fn draw(&self, canvas: &mut WindowCanvas) -> Result<(), String> {
        canvas.copy(&self.texture, None, self.rect)?;
        Ok(())
    }

    pub fn update(&mut self) {
        self.rect.x += self.vel_x;
        if self.vel_x > 0 {
            if self.rect.x + self.rect.w > SCREEN_WIDTH as i32 {
                self.vel_x = -2;
            }
        } else {
            if self.rect.x < 0 {
                self.vel_x = 2;
            }  
        }

        self.rect.y += self.vel_y;
        if self.vel_y > 0 {
            if self.rect.y + self.rect.h > SCREEN_HEIGHT as i32 {
                self.vel_y = -2;
            }
        } else {
            if self.rect.y < 0 {
                self.vel_y = 2;
            }  
        }
    }
}

fn blit(
    outer_surf: &Surface,
    target_surf: &mut Surface,
    dest_rect: &mut Rect,
    radius: i32,
    x: i32,
    y: i32
) -> Result<(), String> {
    dest_rect.x = radius + x; dest_rect.y = radius + y;
    outer_surf.blit(None, target_surf, Some(*dest_rect))?;
    dest_rect.x = radius - x; dest_rect.y = radius + y;
    outer_surf.blit(None, target_surf, Some(*dest_rect))?;
    dest_rect.x = radius + x; dest_rect.y = radius - y;
    outer_surf.blit(None, target_surf, Some(*dest_rect))?;
    dest_rect.x = radius - x; dest_rect.y = radius - y;
    outer_surf.blit(None, target_surf, Some(*dest_rect))?;
    dest_rect.x = radius + y; dest_rect.y = radius + x;
    outer_surf.blit(None, target_surf, Some(*dest_rect))?;
    dest_rect.x = radius - y; dest_rect.y = radius + x;
    outer_surf.blit(None, target_surf, Some(*dest_rect))?;
    dest_rect.x = radius + y; dest_rect.y = radius - x;
    outer_surf.blit(None, target_surf, Some(*dest_rect))?;
    dest_rect.x = radius - y; dest_rect.y = radius - x;
    outer_surf.blit(None, target_surf, Some(*dest_rect))?;
    Ok(())
}