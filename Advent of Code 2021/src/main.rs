use clap::{load_yaml, App};
use colored::*;
use std::fs;

mod day_01;
mod day_02;

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    let day = parse_integer_option(&matches, "DAY", 1, 25);
    let part = parse_integer_option(&matches, "PART", 1, 2);
    let sample = matches.is_present("SAMPLE");

    println!(
        "{} / {} {}",
        format!("Day {:02}", day).green(),
        format!("Part {}", part).red(),
        if sample { "(Sample)" } else { "" },
    );
    println!("{}", "-".repeat(80));

    let suffix = if sample { "sample" } else { "unique" };
    let filename = format!("input/day_{:02}_{}.txt", day, suffix);
    let input = fs::read_to_string(filename).expect("Failed to read input file");

    match day {
        1 => match part {
            1 => day_01::part_1(input),
            _ => day_01::part_2(input),
        },
        2 => match part {
            1 => day_02::part_1(input),
            _ => day_02::part_2(input),
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
