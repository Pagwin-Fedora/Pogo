extern crate libc;


use libc::c_char;
use libc::c_uchar;
use libc::size_t;
use std::ffi::{CStr,CString};
extern{
    fn contactExchange(contactPoint:*const fn(*const c_char)-> ()) -> *const fn(*const c_char) -> *const c_char;
}
#[no_mangle]
pub unsafe extern fn init_frontend(_len: size_t, _config_buffer:*const c_uchar){
    println!("initializing rust pog");
    let the_func = contactExchange(temp_func as *const fn(*const c_char));
    println!("okayaaaaaaaaaaaaaaaaa contact exchange didn't fuck it");
    //the function below causes a seg fault for some reason
//    println!("{}",CString::from_raw(config_buffer as *mut c_char).as_c_str().to_str().unwrap());
//    loop {}
//    panic!("test");
    let b = *the_func;
    println!("pointer dereference is fine");
    let a = CString::new("some message to C").unwrap().into_raw();
    println!("rust str to C string is fine");
    b(a);
    //println!("god dammit it was the function pointer bullshit");
}
pub unsafe extern fn temp_func(stri: *const c_char){
    println!("{}",CStr::from_ptr(stri).to_str().unwrap());
}
//#[panic_handler]
//fn dont_panic(x:&PanicInfo) -> !{
//    loop{}
//}
//#[lang = "eh_personality"] extern fn rust_eh_personality() {}
