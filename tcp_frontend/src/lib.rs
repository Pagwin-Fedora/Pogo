extern crate libc;
use libc::size_t;
use libc::c_uchar;
extern{
    fn contactExchange(contactPoint:&dyn Fn(&str)-> ()) -> &dyn FnMut(&mut str) -> &str;
}
fn init_frontend(_len: size_t, _config_buffer:&[c_uchar]){
    println!("pog!");
    let mut s:String = String::from("Hello From rust!");
    let mut closure = |stri: &str|{
                    println!("called from C++ with {}", stri);
                };
    unsafe{
        println!("received from C++: {}", contactExchange(&closure
        )(s.as_mut_str()));
    }
}
