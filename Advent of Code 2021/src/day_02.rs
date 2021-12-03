/// Solves the Day 02 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let tokens = input.split_whitespace();

    let str_to_int = |token: &str| token.parse::<i64>().unwrap();

    let cmd_it = tokens.clone().step_by(2);
    let val_it = tokens.clone().skip(1).step_by(2).map(str_to_int);

    let mut x: i64 = 0;
    let mut y: i64 = 0;

    for (cmd, val) in cmd_it.zip(val_it) {
        match cmd {
            "up" => y -= val,
            "down" => y += val,
            _ => x += val,
        }
    }

    println!("{}", x * y);
}

/// Solves the Day 02 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let tokens = input.split_whitespace();

    let str_to_int = |token: &str| token.parse::<i64>().unwrap();

    let cmd_it = tokens.clone().step_by(2);
    let val_it = tokens.clone().skip(1).step_by(2).map(str_to_int);

    let mut x: i64 = 0;
    let mut y: i64 = 0;
    let mut aim: i64 = 0;

    for (cmd, val) in cmd_it.zip(val_it) {
        match cmd {
            "up" => aim -= val,
            "down" => aim += val,
            _ => {
                x += val;
                y += aim * val;
            }
        }
    }

    println!("{}", x * y);
}
