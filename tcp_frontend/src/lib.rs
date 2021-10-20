extern crate libc;

use libc::c_char;
use libc::c_uchar;
use libc::size_t;
use std::default::Default;
use std::ffi::CStr;
use std::io::prelude::*;
use tokio::net::{TcpStream,TcpListener};
use std::sync::{Arc,Mutex};
use std::sync::mpsc;
extern{
    fn StringForward(data:*mut c_char) -> *mut c_char;
}

#[derive(serde::Serialize,serde::Deserialize,Clone, Copy)]
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
        unsafe{
            serde_json::from_slice(CStr::from_ptr(bytes as *const i8).to_bytes()).unwrap()
        }
    }
}
#[derive(Default)]
pub struct FrontendState{
    conns:Vec<TcpStream>,
    config:ConfigInfo,
}
#[no_mangle]
pub extern fn initFrontend(_len: size_t, config_buffer:*const c_uchar)-> *const Mutex<FrontendState>{
    let state_ptr:Arc<Mutex<FrontendState>> = Arc::default();
    let mut state = state_ptr.lock().unwrap();
    state.config = ConfigInfo::from(config_buffer);
    std::mem::forget(tokio::runtime::Builder::new_multi_thread()
        .enable_all()
        .build()
        .unwrap()
        .enter()
        );
    
    tokio::spawn(connection_lookout(state_ptr.clone(),state.config.port));
    drop(state);
    
    //to avoid state immediately getting deallocated when we return assuming that's a potential problem it was originally mem::forgot en but apparently that's a no from rust
    
    return Arc::<Mutex<FrontendState>>::into_raw(state_ptr)
}

async fn connection_lookout(state_ptr:Arc<Mutex<FrontendState>>,port:u32){
        let listener = TcpListener::bind(format!("127.0.0.1:{}",port)).await.unwrap();
        loop{
            let result = listener.accept().await;
            match result {
                Ok((mut connection,_addr)) => {
                    negotiate(&mut connection);

                    push_conn(&state_ptr,connection);
                },
                Err(_) => continue
            }

        }
}
fn push_conn(state_ptr:&Arc<Mutex<FrontendState>>,conn:TcpStream){
    state_ptr.lock().unwrap().conns.push(conn);
}
#[allow(non_snake_case)]
#[no_mangle]
pub unsafe extern fn StringBack(state_ptr:*mut Mutex<FrontendState>,message: *const c_char){
    let state = (*state_ptr).lock().unwrap();
    let _data = CStr::from_ptr(message);
    //I hate th is work around but I can't think ofa  better solution
    let (pipe_in,pipe_out) = mpsc::channel::<*mut Mutex<FrontendState>>();
    let _pipe_mutex = Mutex::from(pipe_out);
    let num_of_conns = state.conns.len();
    for _connection_index in 0..num_of_conns {
        tokio::spawn( async {
            //let pipe_out = pipe_mutex.lock().unwrap();
            //let state_ptr = pipe_out.recv().unwrap();
            //let state = (*state_ptr).lock().unwrap();
            //let connection = state.conns[connection_index];
            //drop(state);
            //connection.writable().await;
        });
        pipe_in.send(state_ptr).unwrap();
    }
}

#[allow(non_snake_case)]
#[no_mangle]
///This is a stub to avoid a linker error, it should be deleted when the state storage backend is
///being implemented
pub unsafe extern fn MessageReceive(_msg_ptr:*mut ()){
    println!("Message received but we don't care");
}
///Every tcp socket is passed to this function upon connection to handle initial communication
fn negotiate(_conn:&mut TcpStream){
       
}
