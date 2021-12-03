use clap::{load_yaml, App};
use colored::*;
use std::fs;

mod d01;
mod d02;

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    let day = parse_integer_option(&matches, "DAY", 1, 25);
    let part = parse_integer_option(&matches, "PART", 1, 2);

    println!(
        "{} / {}",
        format!("Day {:02}", day).green(),
        format!("Part {}", part).red()
    );
    println!("{}", "-".repeat(80));

    let filename = format!("input/d{:02}p{}.txt", day, part);
    let input = fs::read_to_string(filename).expect("Failed to read input file");

    match day {
        1 => match part {
            1 => d01::p1(input),
            _ => d01::p2(input),
        },
        2 => match part {
            1 => d02::p1(input),
            _ => d02::p2(input),
        },
        _ => {
            eprintln!("No solution is available yet for day {}.", day)
        }
    }
}

/// Parses the given CLI option into an integer.
fn parse_integer_option(matches: &clap::ArgMatches, option: &str, min: i32, max: i32) -> i32 {
    if let Some(string) = matches.value_of(option) {
        let value = string.parse().unwrap();
        if value < min || value > max {
            panic!(
                "The value of option '{}' does not fall in the range [{}, {}].",
                option, min, max
            );
        }
        return value;
    }
    panic!("The value of option '{}' could not be retrieved.", option);
}
