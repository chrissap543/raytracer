use std::ops::*;

pub struct Vec3 {
    vals: [f64; 3],
}

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

    fn add(self, other: Self) -> Self::Output {
        Self {
            vals: [
                self.vals[0] + other.vals[0],
                self.vals[1] + other.vals[1],
                self.vals[2] + other.vals[2],
            ],
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

#[cfg(test)]
mod test {
    use super::*; 

    #[test]
    fn test_new1() {
        let x = Vec3::new(1.0, 2.0, 3.0); 
        assert_eq!(x.vals, [1.0, 2.0, 3.0]); 
    }
}