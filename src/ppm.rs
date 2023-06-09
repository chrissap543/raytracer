use crate::vector::*;
use std::fs::File;
use std::io::{BufWriter, Write};

// ppm file specification
pub struct Image {
    width: u32,
    height: u32,
    colors: Vec<Vec<Vec3>>,
    fd: File,
}

impl Image {
    pub fn new(
        width: u32,
        height: u32,
        colors: &Vec<Vec<Vec3>>,
        filename: String,
    ) -> Result<Self, &'static str> {
        if colors.is_empty() {
            return Ok(Self {
                width,
                height,
                colors: Self::default_colors(width, height),
                fd: File::create(filename).unwrap(),
            });
        }
        if colors.len() != height.try_into().unwrap()
            || colors[0].len() != width.try_into().unwrap()
        {
            return Err("Invalid colors length");
        }

        Ok(Self {
            width,
            height,
            colors: colors.clone(),
            fd: File::create(filename).unwrap(),
        })
    }
    fn default_colors(width: u32, height: u32) -> Vec<Vec<Vec3>> {
        let mut colors: Vec<Vec<Vec3>> = vec![];
        for y in (0..height).rev() {
            let mut tmp_vec: Vec<Vec3> = vec![];
            for x in 0..width {
                let c = Color::new(x as f64 / 255.0, y as f64 / 255.0, 0.25);
                tmp_vec.push(c);
            }
            colors.push(tmp_vec);
        }
        colors
    }

    pub fn write(&mut self) {
        let mut file = self.fd.try_clone().unwrap();
        let mut buf = BufWriter::new(file);
        buf.write_all(format!("P3\n{} {}\n255\n", self.width, self.height).as_bytes())
            .expect("Unable to write data");

        for vec in self.colors.iter() {
            for c in vec.iter() {
                let str = format!(
                    "{} {} {}\n",
                    (255.999 * c.x()) as i16,
                    (255.999 * c.y()) as i16,
                    (255.999 * c.z()) as i16
                );
                buf.write_all(str.as_bytes()).expect("Unable to write data")
            }
        }
    }
}

impl Default for Image {
    fn default() -> Self {
        let width = 256;
        let height = 256;
        Self {
            width,
            height,
            colors: Self::default_colors(width, height),
            fd: File::create("output/test.ppm").unwrap(),
        }
    }
}
