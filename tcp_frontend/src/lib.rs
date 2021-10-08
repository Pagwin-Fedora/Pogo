extern crate libc;

use libc::c_char;
use libc::c_uchar;
use libc::size_t;
use std::default::Default;
use std::ffi::{CStr,CString};
use std::io::prelude::*;
use std::net::{TcpStream,TcpListener};
use std::sync::{Arc,Mutex,MutexGuard};
extern{
    fn StringForward(data:*mut c_char) -> *mut c_char;
}
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
#[derive(Default)]
pub struct FrontendState{
    conns:Vec<TcpStream>,
    config:ConfigInfo
}
#[no_mangle]
pub extern fn initFrontend(_len: size_t, config_buffer:*const c_uchar)-> *const Mutex<FrontendState>{
    let state_ptr:Arc<Mutex<FrontendState>> = Arc::default();
    let mut state = state_ptr.lock().unwrap();
    state.config = ConfigInfo::from(config_buffer);
    drop(state);
    //somehow I need to do stuff with state here with thread safety and I need to return state when
    //I'm done with this function, fuck
    let ptr_clone = state_ptr.clone();
    std::thread::spawn(move ||{
        //FIXME: state never gets unlocked which is an issue
        let state = ptr_clone.lock().unwrap();
        let listener = TcpListener::bind(format!("127.0.0.1:{}",state.config.port)).unwrap();
        drop(state);
        for result in listener.incoming(){
            match result {
                Ok(mut conn) => {
                    negotiate(&mut conn);
                    let mut state = ptr_clone.lock().unwrap();
                    state.conns.push(conn);
                },
                Err(_) => continue
            }

        }
    });

    //to avoid state immediately getting deallocated when we return assuming that's a potential problem
    
    return Arc::<Mutex<FrontendState>>::into_raw(state_ptr);
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
///Every tcp socket is passed to this function upon connection to handle initial communication
fn negotiate(conn:&mut TcpStream){
       
}
