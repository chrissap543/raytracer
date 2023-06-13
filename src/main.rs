#![allow(dead_code)]

use std::{
    sync::{Arc, Mutex},
    time::Instant,
};

use ppm::Image;
use ray::{ray_color, Ray};
use vector::{Point3, Vec3};

mod hittable;
mod ppm;
mod ray;
mod vector;

fn main() {
    // setting up image
    let aspect_ratio = 16.0 / 9.0;
    let image_width = 2000;
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

    // generate colors
    // let start = Instant::now();
    // let v = multithread(
    //     image_height,
    //     image_width,
    //     &origin,
    //     &horizontal,
    //     &vertical,
    //     &lower_left_corner,
    // ).unwrap();
    // let duration = start.elapsed();
    // println!("Time elapsed in multithread() is: {:?}", duration);

    // let start = Instant::now();
    // let _ = singlethread(
    //     image_height,
    //     image_width,
    //     &origin,
    //     &horizontal,
    //     &vertical,
    //     &lower_left_corner,
    // );
    // let duration = start.elapsed();
    // println!("Time elapsed in singlethread() is: {:?}", duration);

    let v = singlethread(
        image_height,
        image_width,
        &origin,
        &horizontal,
        &vertical,
        &lower_left_corner,
    );
    let mut raytraced = Image::new(
        image_width,
        image_height,
        &v,
        "output/multi.ppm".to_string(),
    )
    .unwrap();
    raytraced.write();
}

fn singlethread(
    image_height: u32,
    image_width: u32,
    origin: &Vec3,
    horizontal: &Vec3,
    vertical: &Vec3,
    lower_left_corner: &Vec3,
) -> Vec<Vec<Vec3>> {
    let mut v: Vec<Vec<Vec3>> = vec![];
    for j in (0..image_height).rev() {
        let mut temp_vec: Vec<Vec3> = vec![];
        for i in 0..image_width {
            let u = i as f64 / (image_width - 1) as f64;
            let v = j as f64 / (image_height - 1) as f64;
            let r = Ray::new(
                &origin,
                &(*lower_left_corner + u * *horizontal + v * *vertical - *origin),
            );
            let c = ray_color(&r);
            temp_vec.push(c);
        }
        v.push(temp_vec);
    }
    v
}

fn multithread(
    image_height: u32,
    image_width: u32,
    origin: &Vec3,
    horizontal: &Vec3,
    vertical: &Vec3,
    lower_left_corner: &Vec3,
) -> Option<Vec<Vec<Vec3>>> {
    let pool = rayon::ThreadPoolBuilder::new()
        .num_threads(16)
        .build()
        .unwrap();

    let empty: Vec<Vec<Vec3>> =
        vec![vec![Vec3::default(); image_width as usize]; image_height as usize];
    let ret = Arc::new(Mutex::new(empty));
    pool.scope(|s| {
        for j in (0..image_height).rev() {
            for i in 0..image_width {
                let local = ret.clone();
                s.spawn(move |_| {
                    //let local = local;
                    let u = i as f64 / (image_width - 1) as f64;
                    let v = j as f64 / (image_height - 1) as f64;
                    let r = Ray::new(
                        &origin,
                        &(*lower_left_corner + u * *horizontal + v * *vertical - *origin),
                    );
                    let c = ray_color(&r);

                    let mut data = local.lock().unwrap();
                    data[(image_height - j - 1) as usize][i as usize] = c;
                });
            }
        }
    });

    match Arc::try_unwrap(ret) {
        Ok(v) => Some(v.into_inner().unwrap()),
        Err(_) => {
            eprintln!("Failed to unwrap Arc");
            None
        }
    }
}
