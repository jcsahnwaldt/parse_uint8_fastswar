
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

fn main() {
  let b = "0\0\0\0".as_bytes();
  println!("{:?}", parse_uint8_fastswar(&b[..1]));
  let b = "255\0".as_bytes();
  println!("{:?}", parse_uint8_fastswar(&b[..3]));
}
