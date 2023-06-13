use std::ops::*;
#[derive(Clone, Copy)]
pub struct Vec3 {
    vals: [f64; 3],
}

pub type Point3 = Vec3;
pub type Color = Vec3;

impl Vec3 {
    pub fn new(a: f64, b: f64, c: f64) -> Vec3 {
        Vec3 { vals: [a, b, c] }
    }

    pub fn x(&self) -> f64 {
        self.vals[0]
    }
    pub fn y(&self) -> f64 {
        self.vals[1]
    }
    pub fn z(&self) -> f64 {
        self.vals[2]
    }

    pub fn length(&self) -> f64 {
        self.length_squared().sqrt()
    }
    pub fn length_squared(&self) -> f64 {
        self.vals[0] * self.vals[0] + self.vals[1] * self.vals[1] + self.vals[2] * self.vals[2]
    }
}

pub fn dot(u: &Vec3, v: &Vec3) -> f64 {
    u.x() * v.x() + u.y() * v.y() + u.z() * v.z()
}

pub fn cross(u: &Vec3, v: &Vec3) -> Vec3 {
    Vec3 {
        vals: [
            u.vals[1] * v.vals[2] - u.vals[2] * v.vals[1],
            u.vals[2] * v.vals[0] - u.vals[0] * v.vals[2],
            u.vals[0] * v.vals[1] - u.vals[1] * v.vals[0],
        ],
    }
}

pub fn unit_vector(u: &Vec3) -> Vec3 {
    *u / u.length()
}

impl Default for Vec3 {
    fn default() -> Self {
        Self {
            vals: [0.0, 0.0, 0.0],
        }
    }
}

// unary operators
impl Neg for Vec3 {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Self {
            vals: [-self.vals[0], -self.vals[1], -self.vals[2]],
        }
    }
}

impl Index<usize> for Vec3 {
    type Output = f64;

    fn index(&self, index: usize) -> &Self::Output {
        match index {
            0 => &self.vals[0],
            1 => &self.vals[1],
            2 => &self.vals[2],
            _ => panic!("index out of bounds"),
        }
    }
}

impl IndexMut<usize> for Vec3 {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        match index {
            0 => &mut self.vals[0],
            1 => &mut self.vals[1],
            2 => &mut self.vals[2],
            _ => panic!("index out of bounds"),
        }
    }
}

// binary operators
impl Add for Vec3 {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        Self {
            vals: [
                self.vals[0] + rhs.vals[0],
                self.vals[1] + rhs.vals[1],
                self.vals[2] + rhs.vals[2],
            ],
        }
    }
}

impl Sub for Vec3 {
    type Output = Self;

    fn sub(self, rhs: Self) -> Self::Output {
        Self {
            vals: [
                self.vals[0] - rhs.vals[0],
                self.vals[1] - rhs.vals[1],
                self.vals[2] - rhs.vals[2],
            ],
        }
    }
}

impl Mul for Vec3 {
    type Output = Self;

    fn mul(self, rhs: Self) -> Self::Output {
        Self {
            vals: [
                self.vals[0] * rhs.vals[0],
                self.vals[1] * rhs.vals[1],
                self.vals[2] * rhs.vals[2],
            ],
        }
    }
}

impl Mul<f64> for Vec3 {
    type Output = Self;
    fn mul(self, rhs: f64) -> Self::Output {
        Self {
            vals: [self.vals[0] * rhs, self.vals[1] * rhs, self.vals[2] * rhs],
        }
    }
}

impl Mul<Vec3> for f64 {
    type Output = Vec3;
    fn mul(self, rhs: Vec3) -> Self::Output {
        Self::Output {
            vals: [self * rhs.vals[0], self * rhs.vals[1], self * rhs.vals[2]],
        }
    }
}

impl Div<f64> for Vec3 {
    type Output = Self;
    fn div(self, rhs: f64) -> Self::Output {
        Self {
            vals: [self.vals[0] / rhs, self.vals[1] / rhs, self.vals[2] / rhs],
        }
    }
}

impl AddAssign for Vec3 {
    fn add_assign(&mut self, rhs: Self) {
        self.vals[0] += rhs.vals[0];
        self.vals[1] += rhs.vals[1];
        self.vals[2] += rhs.vals[2];
    }
}

impl SubAssign for Vec3 {
    fn sub_assign(&mut self, rhs: Self) {
        self.vals[0] -= rhs.vals[0];
        self.vals[1] -= rhs.vals[1];
        self.vals[2] -= rhs.vals[2];
    }
}

impl MulAssign<f64> for Vec3 {
    fn mul_assign(&mut self, rhs: f64) {
        self.vals[0] *= rhs;
        self.vals[1] *= rhs;
        self.vals[2] *= rhs;
    }
}

impl DivAssign<f64> for Vec3 {
    fn div_assign(&mut self, rhs: f64) {
        self.vals[0] /= rhs;
        self.vals[1] /= rhs;
        self.vals[2] /= rhs;
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_new1() {
        let x = Vec3::new(1.0, 2.0, 3.0);
        assert_eq!(x.vals, [1.0, 2.0, 3.0]);
    }
}
