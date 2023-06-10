use crate::vector::*;
#[derive(Default)]
pub struct Ray {
    orig: Point3,
    dir: Vec3,
}

impl Ray {
    pub fn new(origin: &Point3, direction: &Vec3) -> Self {
        Self {
            orig: origin.clone(),
            dir: direction.clone(),
        }
    }

    pub const fn origin(&self) -> Point3 {
        self.orig
    }
    pub const fn direction(&self) -> Vec3 {
        self.dir
    }

    pub fn at(&self, t: f64) -> Point3 {
        self.orig + t * self.dir
    }
}
