use ppm::Image;

mod ppm;
mod ray;
mod vector;

fn main() {
    let mut img: Image = Default::default();
    img.write();
    let test = img.colors();
    let mut img2 = Image::new(256, 256, test, "output/img2.ppm".to_string()).unwrap();
    img2.write();
}
