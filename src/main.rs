use ppm::Image;
use ray::{ray_color, Ray};
use vector::{Point3, Vec3};

mod ppm;
mod ray;
mod vector;
mod hittable; 

fn main() {
    // default image
    let mut img: Image = Default::default();
    img.write();

    // setting up image
    let aspect_ratio = 16.0 / 9.0;
    let image_width = 400;
    let image_height = (image_width as f64 / aspect_ratio) as u32;

    // setting up camera
    let viewport_height = 2.0;
    let viewport_width = aspect_ratio * viewport_height;
    let focal_length = 1.0;

    let origin = Point3::new(0.0, 0.0, 0.0);
    let horizontal = Vec3::new(viewport_width, 0.0, 0.0);
    let vertical = Vec3::new(0.0, viewport_height, 0.0);
    let lower_left_corner =
        origin - horizontal / 2.0 - vertical / 2.0 - Vec3::new(0.0, 0.0, focal_length);

    let mut v: Vec<Vec<Vec3>> = vec![];
    for j in (0..image_height).rev() {
        let mut temp_vec: Vec<Vec3> = vec![];
        for i in 0..image_width {
            let u = i as f64 / (image_width - 1) as f64;
            let v = j as f64 / (image_height - 1) as f64;
            let r = Ray::new(
                &origin,
                &(lower_left_corner + u * horizontal + v * vertical - origin),
            );
            let c = ray_color(&r);
            temp_vec.push(c);
        }
        v.push(temp_vec);
    }
    let mut raytraced = Image::new(
        image_width,
        image_height,
        &v,
        "output/raytraced.ppm".to_string(),
    )
    .unwrap();
    raytraced.write();
}
