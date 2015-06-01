#![crate_type="rlib"]

extern crate iron;
extern crate router;
extern crate urlencoded;

use iron::prelude::*;
use iron::status;
use router::{Router};
use urlencoded::UrlEncodedBody;

use std::process::Command;

fn main() {
    let mut router = Router::new();
    router.post("/data", handler);

    Iron::new(router).http("0.0.0.0:5000").unwrap();

    fn handler(req: &mut Request) -> IronResult<Response> {
        match req.get_ref::<UrlEncodedBody>() {
            Ok(hashmap) => {
                println!("Received data: \n {:?}", hashmap);
                let button: isize = hashmap.get("button").unwrap()[0].parse().unwrap();
                let status = Command::new(format!("./handlers/{:?}.sh", button)).status().unwrap();
                println!("Status: {:?}", status);
            }
            Err(_) => println!("Error")
        }

        Ok(Response::with((status::Ok, "thanks")))
    }
}
