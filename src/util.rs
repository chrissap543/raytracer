pub static INFINITY: f64 = f64::INFINITY;
pub static PI: f64 = 3.14159265358979323846264338327950288f64;

pub fn degrees_to_radian(degrees: f64) -> f64 {
    degrees * PI / 180.0
}
