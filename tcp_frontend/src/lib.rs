#![feature(lang_items)]
extern crate libc;


use libc::c_char;
use libc::c_uchar;
use libc::size_t;
use std::ffi::{CStr,CString};
extern{
    fn contactExchange(contactPoint:*const fn(*const c_char)-> ()) -> *const fn(*const c_char) -> *const c_char;
}
#[no_mangle]
pub unsafe extern fn init_frontend(_len: size_t, config_buffer:*const c_uchar){
    println!("initializing rust pog");
    let the_func = contactExchange(temp_func as *const fn(*const c_char));
    println!("okay contact exchange didn't fuck it");
    (*the_func)(config_buffer as *const c_char);
}
pub unsafe extern fn temp_func(stri: *const c_char){
    println!("okay so the temp_func call fucks it");
    println!("{}",CStr::from_ptr(stri).to_str().unwrap());
}
//#[panic_handler]
//fn dont_panic(x:&PanicInfo) -> !{
//    loop{}
//}
//#[lang = "eh_personality"] extern fn rust_eh_personality() {}
