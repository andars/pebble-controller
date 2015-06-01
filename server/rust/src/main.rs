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
        let mut resp = String::new();
        match req.get_ref::<UrlEncodedBody>() {
            Ok(hashmap) => {
                let button: isize = hashmap.get("button").unwrap()[0].parse().unwrap_or(5);
                let status = Command::new(format!("{}/.pc_handlers/{:?}.sh",std::env::home_dir().unwrap().to_str().unwrap(), button)).status();
                match status {
                    Ok(_) => { 
                        resp = "Received".to_string();
                    } 
                    Err(e) => {
                        resp = format!("{:?}", e);
                    }
                }
            }
            Err(_) => println!("Pebble Controller Error")
        }

        Ok(Response::with((status::Ok, &resp[..])))
    }
}
