use std::env;
use std::process;

use crate::monny::monny::Monny;

mod monny;

fn main() {
    let args: Vec<String> = env::args().collect();

    let argc: usize = args.len();
    let argv = args;

    if argc > 2 {
        println!("[ERROR]: Usage {} main.mn", argv[0]);
        process::exit(-1);
    }

    if argc == 2 {
        let _ = Monny::run_file_script(&argv[1]);
    } else {
        Monny::run_repl();
    }
}
