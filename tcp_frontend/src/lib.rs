extern crate libc;
use libc::size_t;
use libc::c_uchar;

extern{
    fn contactExchange(contactPoint:&dyn FnMut(&mut str)-> ()) -> &dyn FnMut(&mut str) -> &str;
}
fn init_frontend(len: size_t, config_buffer:&[c_uchar]){
    println!("pog!");
    let mut s:String = String::from("Hello From rust!");
    unsafe{
        println!("received from C++: {}", contactExchange(&mut move |stri: &mut str|{
            println!("called from C++ with {}", stri);
        })(s.as_mut_str()));
    }
}
