use std::ffi::c_void;
use std::ptr::addr_of;
use std::mem::size_of;

// according to godbolt.org, all of these functions generate
// mov eax, dword ptr [rdi]

fn foo(b: &[u8]) -> u32 {
    let p = b.as_ptr() as *const u32;
    unsafe { p.read_unaligned() }
}

extern "C" {
    fn memcpy(dst: *mut c_void, src: *const c_void, count: usize) -> *mut c_void;
}

fn bar(b: &[u8]) -> u32 {
    let i: u32 = 0;
    unsafe {
        memcpy(addr_of!(i) as *mut c_void, b.as_ptr() as *const c_void, size_of::<u32>());
    };
    return i;
}

fn baz(b: &[u8]) -> u32 {
    let i: u32 = 0;
    unsafe {
        std::ptr::copy_nonoverlapping(b.as_ptr(), std::ptr::addr_of!(i) as *mut u8, size_of::<u32>());
    }
    return i;
}

fn main() {
    let b: [u8; 4] = [1,2,3,4];
    println!("{:X?}", foo(&b));
    println!("{:X?}", bar(&b));
    println!("{:X?}", baz(&b));
}
