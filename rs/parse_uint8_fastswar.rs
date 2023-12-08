
fn parse_uint8_fastswar(b: &[u8]) -> Option<u8> {
  if b.len() == 0 || b.len() > 3 { return None; }
  let p = b.as_ptr() as *const u32;
  let mut digits = unsafe { p.read_unaligned() };
  digits ^= 0x30303030;
  digits <<= (4 - b.len()) * 8;
  let num = ((digits.wrapping_mul(0x640a01)) >> 24) as u8;
  let all_digits = ((digits | (digits.wrapping_add(0x06060606))) & 0xF0F0F0F0) == 0;
  (all_digits && digits.swap_bytes() <= 0x020505).then_some(num)
}

struct Test { str: [u8; 4], len: usize, want: i32 }

macro_rules! error {
  ($self:ident, $fmt:literal, $($args:expr),*) => {
    print!("{:X?}, len {}: ", &$self.str, $self.len);
    println!($fmt, $($args),*);
  };
}

impl Test {

  fn new() -> Self {
    Self { str: [0; 4], len: 0, want: 0 }
  }

  fn test_rec(&mut self) {
    let i = self.len;
    self.len += 1;
    let want = self.want * 10;
    for b in 0..=255 {
      self.str[i] = b;
      self.want =
        if b < b'0' || b > b'9' { -1 }
        else { want + (b - b'0') as i32 };
      self.check();
      if i < 2 { self.test_rec(); }
    }
    self.len -= 1;
  }

  fn check(&self) {
    let want = self.want;
    let got = parse_uint8_fastswar(&self.str[..self.len]).map_or(-1, i32::from);
    if want < 0 || want > 255 {
      if got >= 0 { error!(self, "expected error, got {}", got); }
    }
    else {
      if got < 0 { error!(self, "expected {}, got error", want); }
      else if got != want { error!(self, "expected {}, got {}", want, got); }
    }
  }

}

fn main() {
  let mut t = Test::new();
  t.test_rec();
}
