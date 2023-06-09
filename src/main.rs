mod vector;
mod ppm; 

fn main() {
    println!("Hello, world!");
    let x = vector::Vec3::new(1.0, 2.0, 3.0);
    println!("{}", x.x())
}
