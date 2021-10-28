use libc::c_char;
use libc::c_uchar;
use libc::size_t;
use tokio::runtime::Runtime;
use std::default::Default;
use std::ffi::{CStr,CString};
use tokio::net::{TcpStream,TcpListener};
use std::sync::Arc;
use futures::lock::Mutex;
extern{
    fn StringForward(data:*const c_char) -> *const c_char;
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
unsafe impl Sync for ConfigInfo{}

impl From<*const c_uchar> for ConfigInfo {
    fn from(bytes:*const c_uchar)->Self{
        //TODO: actually implement from here
        unsafe{
            serde_json::from_slice(CStr::from_ptr(bytes as *const i8).to_bytes()).unwrap()
        }
    }
}
struct ConnectionContainer{
    alive:bool,
    conn:tokio::net::TcpStream
}
impl ConnectionContainer{
    fn new(conn:tokio::net::TcpStream)->Self{
        return ConnectionContainer{alive:true,conn};
    }
}
pub struct FrontendState{
    conns:Vec<Arc<Mutex<ConnectionContainer>>>,
    config:ConfigInfo,
    runtime:Runtime
}
impl FrontendState{
    fn new(config:ConfigInfo,runtime:Runtime) -> Self{
        FrontendState{config,runtime,conns:Vec::default()}
    }
}
#[no_mangle]
pub extern fn initFrontend(_len: size_t, config_buffer:*const c_uchar)-> *const Mutex<FrontendState>{
    let config = ConfigInfo::default();//ConfigInfo::from(config_buffer);
    let runtime = tokio::runtime::Builder::new_multi_thread()
        .enable_all()
        .build()
        .unwrap();

    let ptr = Arc::new(Mutex::new(FrontendState::new(config,runtime)));
    let ptrc = ptr.clone();
    let ptrcc = ptr.clone();
    let state = futures::executor::block_on(ptrc.lock());

    state.runtime.spawn(async move {    
        let state = ptrcc.lock().await;
        state.runtime.spawn(connection_lookout(ptrcc.clone()));
    });

    
    //to avoid state immediately getting deallocated when we return assuming that's a potential problem it was originally mem::forgot en but apparently that's a no from rust
    
    return Arc::into_raw(ptr)
}
async fn dead_streams_cleaner(state_ptr:Arc<Mutex<FrontendState>>){
    loop {
//cleaner runs on regular intervals rather than whenever we need to deal with the streams to avoid having to think about how to avoid deadlock
        tokio::time::sleep(std::time::Duration::from_secs(1)).await;
        let mut state = state_ptr.lock().await;
        for i in 0..state.conns.len() {
            let unnecessary_var = state.conns[i].clone();
            let maybe_conn = unnecessary_var.lock().await;
            if !maybe_conn.alive{
                state.conns.remove(i);
            }
        }
    }
}
async fn connection_lookout(state_ptr: Arc<Mutex<FrontendState>>){
    let mut state = state_ptr.lock().await;
    let listener = TcpListener::bind(format!("127.0.0.1:{}",state.config.port)).await.unwrap();
    loop{
        let result = listener.accept().await;
        match result {
            Ok((mut connection,_addr)) => {
                negotiate(&mut connection);
                let connection_ptr = Arc::new(Mutex::new(ConnectionContainer::new(connection)));
                let ptr_clone = connection_ptr.clone();
                state.runtime.spawn(async move{
                    loop{
                        let mut c = ptr_clone.lock().await;
                        let connection = &c.conn;
                        connection.readable().await.unwrap();
                        let mut buf = [0_u8;1024];
                        let byte_count = match connection.try_read(&mut buf){
                            Ok(count) => {
                                if count != 0 {
                                    count
                                }
                                else {
                                    c.alive = false;
                                    break;
                                }
                            },
                            Err(_)=> {continue;}
                        };

                        unsafe{
                            let cstr = CString::new(&buf[0..byte_count]).unwrap();
                            StringForward(cstr.as_ptr());
                            
                        }

                    }
                });
                state.conns.push(connection_ptr);
            },
            Err(_) => continue
        }

    }
}
#[allow(non_snake_case)]
#[no_mangle]
pub unsafe extern fn StringBack(state_ptr:*mut Arc<Mutex<FrontendState>>,message: *const c_char){
    let ptr = (*state_ptr).clone();
    let state = futures::executor::block_on((ptr).lock());
    
    let data = CStr::from_ptr(message).to_bytes();
    for connection_index in 0..state.conns.len(){
        let ptrc = ptr.clone();
        tokio::spawn( async move {
            let state = ptrc.lock().await;
            let connection = &state.conns[connection_index].lock().await.conn;
            connection.writable().await.unwrap();
            //TODO: might need to do stuff to make sure all the data gets pushed through
            connection.try_write(data).unwrap();
        });
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
