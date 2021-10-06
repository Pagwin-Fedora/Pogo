extern crate libc;

use libc::c_char;
use libc::c_uchar;
use libc::size_t;
use std::ffi::{CStr,CString};
extern{
    fn StringForward(data:*mut c_char) -> *mut c_char;
}
#[repr(C)]
pub struct FrontendState{
    
}
impl std::default::Default for FrontendState {
    fn default() -> Self {
        return FrontendState{}   
    }
}
#[no_mangle]
pub extern fn initFrontend(_len: size_t, config_buffer:*const c_uchar)-> *mut FrontendState{
    let a = CString::new("some message to C").unwrap().into_raw();
    unsafe {
        StringForward(a);
        println!("{}",CStr::from_ptr(config_buffer as *const i8).to_str().unwrap());
    }
    let state:Box<FrontendState> = Box::default();
    //hmmm
    return Box::<FrontendState>::into_raw(state);
}
#[allow(non_snake_case)]
#[no_mangle]
pub unsafe extern fn StringBack(_state:*mut FrontendState,stri: *const c_char){
    println!("{}",CStr::from_ptr(stri).to_str().unwrap());
}
#[allow(non_snake_case)]
#[no_mangle]
pub unsafe extern fn MessageReceive(_msg_ptr:*mut ()){
    println!("Message received but we don't care");
}
//#[panic_handler]
//fn dont_panic(x:&PanicInfo) -> !{
//    loop{}
//}
//#[lang = "eh_personality"] extern fn rust_eh_personality() {}
