use crate::vector::*;

use super::Hittable;

pub struct Sphere {
    center: Point3,
    radius: f64,
}

impl Sphere {
    pub fn new(cen: &Point3, rad: f64) -> Self {
        Self {
            center: cen.clone(),
            radius: rad,
        }
    }
}

impl Hittable for Sphere {
    fn hit(r: &crate::ray::Ray, t_min: f64, t_max: f64, rec: &super::HitRecord) -> bool {}
}
