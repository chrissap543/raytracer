use ppm::Image;

mod ppm;
mod ray;
mod vector;

fn main() {
    // default image
    let mut img: Image = Default::default();
    img.write();

    // setting up image
    let aspect_ratio = 16.0 / 9.0;
    let image_width = 400;
    let image_height = (image_width as f64 / aspect_ratio) as i32;
}
