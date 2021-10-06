extern crate libc;

use libc::c_char;
use libc::c_uchar;
use libc::size_t;
use std::default::Default;
use std::ffi::{CStr,CString};
use std::io::prelude::*;
use std::net::{TcpStream,TcpListener};
extern{
    fn StringForward(data:*mut c_char) -> *mut c_char;
}
#[repr(C)]
#[derive(Clone, Copy)]
pub struct ConfigInfo{
    port:u32
}
impl Default for ConfigInfo{
    fn default() -> Self {
        ConfigInfo{
            //change default port
            port:1337
        }
    }
}
impl From<*const c_uchar> for ConfigInfo {
    fn from(bytes:*const c_uchar)->Self{
        //TODO: actually implement from here
        Self::default()
    }
}
#[repr(C)]
#[derive(Default)]
pub struct FrontendState{
    conns:Vec<TcpStream>,
    config:ConfigInfo
}
#[no_mangle]
pub extern fn initFrontend(_len: size_t, config_buffer:*const c_uchar)-> *mut FrontendState{
    let mut state:Box<FrontendState> = Box::default();
    state.config = ConfigInfo::from(config_buffer);
    //somehow I need to do stuff with state here with thread safety and I need to return state when
    //I'm done with this function, fuck
    std::thread::spawn(||{
        let listener = TcpListener::bind(format!("127.0.0.1:{}",state.config.port)).unwrap();
        for result in listener.incoming(){
            match result {
                Ok(conn) => {
                    negotiate(&mut conn);
                    state.conns.push(conn);
                },
                Err(_) => continue
            }

        }
    });



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
fn negotiate(conn:&mut TcpStream){
    
}
