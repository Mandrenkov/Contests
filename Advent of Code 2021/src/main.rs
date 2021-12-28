use clap::{load_yaml, App};
use colored::*;
use std::fs;

mod day_01;
mod day_02;
mod day_03;
mod day_04;
mod day_05;
mod day_06;
mod day_07;
mod day_08;
mod day_09;
mod day_10;
mod day_11;
mod day_12;
mod day_13;
mod day_14;
mod day_15;
mod day_16;
mod day_17;
mod day_18;
mod day_19;
mod day_20;
mod day_21;
mod day_22;
mod day_23;
mod day_24;

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    // Get and validate the command-line arguments.
    let day = parse_integer_option(&matches, "DAY", 1, 25);
    let part = parse_integer_option(&matches, "PART", 1, 2);
    let sample = matches.is_present("SAMPLE");

    // Display a fancy header.
    println!(
        "{} / {} {}",
        format!("Day {:02}", day).green(),
        format!("Part {}", part).red(),
        if sample { "(Sample)" } else { "" },
    );
    println!("{}", "-".repeat(80));

    // Read the input file into a string.
    let suffix = if sample { "sample" } else { "unique" };
    let filename = format!("input/day_{:02}_{}.txt", day, suffix);
    let input = fs::read_to_string(filename).expect("Failed to read input file");

    // Find the solver function.
    let solver: fn(String) = match day {
        1 => match part {
            1 => day_01::part_1,
            _ => day_01::part_2,
        },
        2 => match part {
            1 => day_02::part_1,
            _ => day_02::part_2,
        },
        3 => match part {
            1 => day_03::part_1,
            _ => day_03::part_2,
        },
        4 => match part {
            1 => day_04::part_1,
            _ => day_04::part_2,
        },
        5 => match part {
            1 => day_05::part_1,
            _ => day_05::part_2,
        },
        6 => match part {
            1 => day_06::part_1,
            _ => day_06::part_2,
        },
        7 => match part {
            1 => day_07::part_1,
            _ => day_07::part_2,
        },
        8 => match part {
            1 => day_08::part_1,
            _ => day_08::part_2,
        },
        9 => match part {
            1 => day_09::part_1,
            _ => day_09::part_2,
        },
        10 => match part {
            1 => day_10::part_1,
            _ => day_10::part_2,
        },
        11 => match part {
            1 => day_11::part_1,
            _ => day_11::part_2,
        },
        12 => match part {
            1 => day_12::part_1,
            _ => day_12::part_2,
        },
        13 => match part {
            1 => day_13::part_1,
            _ => day_13::part_2,
        },
        14 => match part {
            1 => day_14::part_1,
            _ => day_14::part_2,
        },
        15 => match part {
            1 => day_15::part_1,
            _ => day_15::part_2,
        },
        16 => match part {
            1 => day_16::part_1,
            _ => day_16::part_2,
        },
        17 => match part {
            1 => day_17::part_1,
            _ => day_17::part_2,
        },
        18 => match part {
            1 => day_18::part_1,
            _ => day_18::part_2,
        },
        19 => match part {
            1 => day_19::part_1,
            _ => day_19::part_2,
        },
        20 => match part {
            1 => day_20::part_1,
            _ => day_20::part_2,
        },
        21 => match part {
            1 => day_21::part_1,
            _ => day_21::part_2,
        },
        22 => match part {
            1 => day_22::part_1,
            _ => day_22::part_2,
        },
        23 => match part {
            1 => day_23::part_1,
            _ => day_23::part_2,
        },
        24 => match part {
            1 => day_24::part_1,
            _ => day_24::part_2,
        },
        _ => {
            panic!("No solver is available yet for day {}.", day)
        }
    };

    solver(input);
}

/// Parses the given command-line option into an integer.
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
