pub struct Monny {}

use std::fs::File;
use std::io::{self, Read, Write};
use std::path::Path;
use std::process;

impl Monny {
    pub fn run_file_script(path: &String) -> std::io::Result<()> {
        if !Path::new(path).exists() {
            println!("Arquivo {} não encontrado", path);
            process::exit(66);
        }

        let mut file = File::open(path)?;

        let mut buffer: Vec<u8> = Vec::new();
        file.read_to_end(&mut buffer)?;

        Monny::run(&buffer);

        Ok(())
    }

    pub fn run_repl() {
        let mut line: Vec<u8> = Vec::new();
        let mut buf = [0u8; 1];
        let mut stdin = io::stdin();

        loop {
            print!("\nmonny> ");
            io::stdout().flush().unwrap();

            line.clear();

            loop {
                let n = stdin.read(&mut buf).unwrap();

                if n == 0 {
                    return;
                }

                let b = buf[0];

                if b == b'\n' {
                    break;
                }

                line.push(b);
            }

            let input = String::from_utf8_lossy(&line);

            if input.trim() == "exit" {
                println!("");
                break;
            }

            Monny::run(&line);
        }
    }

    fn run(source: &Vec<u8>) {
        println!("{:?}", source);
    }
}
