extern crate libc;
use libc::size_t;
use libc::c_uchar;

extern{
    fn contactExchange(contactPoint:&dyn FnMut(&str)-> ()) -> &dyn FnMut(&str) -> &str;
}
fn init_frontend(len: size_t, config_buffer:&[c_uchar]){
    println!("pog!");
    unsafe{
        println!("received from C++: {}",contactExchange(|stri: &str|{
            println!("called from C++ with {}", stri);
        })());
    }
}
