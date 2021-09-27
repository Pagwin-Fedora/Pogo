extern crate libc;

use libc::c_char;
use libc::c_uchar;
use libc::size_t;
extern{
    fn contactExchange(contactPoint:*const fn(*const c_char)-> ()) -> *const fn(*const c_char) -> *const c_char;
}

#[no_mangle]
pub extern fn init_frontend(len: size_t, config_buffer:*const c_uchar){
   println!("pog!");
    let mut s:String = String::from("Hello From rust!");

    unsafe{
        let func_ptr: *const fn(*const c_char) -> () = Box::into_raw(Box::new(temp_func)) as *const fn(*const c_char);
        println!("received from C++: {}", std::ffi::CStr::from_ptr((*contactExchange(func_ptr))(s.as_mut_str().as_ptr() as *const c_char)).to_str().unwrap());
    }
}
pub unsafe extern fn temp_func(stri: *const c_char){
    println!("called from C++ with {}", std::ffi::CStr::from_ptr(stri).to_str().unwrap());
}
