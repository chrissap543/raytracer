use crate::vector::*;
use std::fs::File;
use std::io::Write;

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
        colors: Vec<Vec<Vec3>>,
        filename: String,
    ) -> Result<Self, &'static str> {
        if colors.len() != width.try_into().unwrap() || colors[0].len() != height.try_into().unwrap() {
            return Err("Invalid colors length")
        }

        Ok(Self {
            width: width,
            height: height,
            colors: colors,
            fd: File::create(filename).unwrap(),
        })
    }

    pub fn write(&mut self) {
        self.fd
            .write_all(format!("P3\n{} {}\n255\n", self.width, self.height).as_bytes())
            .expect("Unable to write data");

        for vec in self.colors.iter() {
            for c in vec.iter() {
                let str = format!(
                    "{} {} {}\n",
                    (255.999 * c.x()) as i16,
                    (255.999 * c.y()) as i16,
                    (255.999 * c.z()) as i16
                );
                self.fd
                    .write_all(str.as_bytes())
                    .expect("Unable to write data")
            }
        }
    }

    pub fn colors(&self) -> Vec<Vec<Vec3>> {
        self.colors.clone()
    }
}

impl Default for Image {
    fn default() -> Self {
        let mut colors: Vec<Vec<Vec3>> = vec![];
        for y in (0..256).rev() {
            let mut tmp_vec: Vec<Vec3> = vec![];
            for x in 0..256 {
                let c = Color::new(x as f64 / 255.0, y as f64 / 255.0, 0.25);
                tmp_vec.push(c);
            }
            colors.push(tmp_vec);
        }
        Self {
            width: 256,
            height: 256,
            colors: colors,
            fd: File::create("output/test.ppm").unwrap(),
        }
    }
}
