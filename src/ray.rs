use crate::vector::*;
#[derive(Default)]
pub struct Ray {
    orig: Point3,
    dir: Vec3,
}

impl Ray {
    pub fn new(origin: &Point3, direction: &Vec3) -> Self {
        Self {
            orig: *origin,
            dir: *direction,
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

pub fn ray_color(r: &Ray) -> Color {
    // intercept sphere
    if let Some(t) = hit_sphere(&Point3::new(0.0, 0.0, -1.0), 0.5, r) {
        let normal = unit_vector(&(r.at(t) - Vec3::new(0.0, 0.0, -1.0)));
        return Color::new(normal.x() + 1.0, normal.y() + 1.0, normal.z() + 1.0);
    }
    let unit_dir = unit_vector(&r.direction());
    let t = 0.5 * (unit_dir.y() + 1.0);
    (1.0 - t) * Color::new(1.0, 1.0, 1.0) + t * Color::new(0.5, 0.7, 1.0)
}

pub fn hit_sphere(center: &Point3, radius: f64, r: &Ray) -> Option<f64> {
    let diff = r.origin() - *center;
    let a = r.direction().length_squared();
    let half_b = dot(&r.direction(), &diff);
    let c = diff.length_squared() - radius * radius;

    let discriminant = half_b * half_b - a * c;
    if discriminant < 0.0 {
        None
    } else {
        Some((-half_b - discriminant.sqrt()) / a)
    }
}
