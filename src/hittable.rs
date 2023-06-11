use crate::{vector::{Point3, Vec3, dot}, ray::Ray};

mod sphere;

pub struct HitRecord {
    p: Point3, 
    normal: Vec3, 
    t: f64,
    front_face: bool, 
}

pub trait Hittable {
    fn hit(r: &Ray, t_min: f64, t_max: f64, rec: &HitRecord) -> bool; 
}

impl HitRecord {
    pub fn set_face_normal(&self, r: &Ray, outward_normal: &Vec3) {
        self.front_face = dot(&r.direction(), outward_normal) < 0.0; 
        if self.front_face {
            self.normal = outward_normal.clone(); 
        } else {
            self.normal = -outward_normal.clone(); 
        }
    }
}