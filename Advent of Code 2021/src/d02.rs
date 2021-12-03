/// Solves the Day 02 Part 1 puzzle with respect to the given input.
pub fn p1(input: String) {
    let tokens = input.split_whitespace();

    let cmd_it = tokens.clone().step_by(2);
    let val_it = tokens
        .clone()
        .skip(1)
        .step_by(2)
        .map(|v| v.parse::<i32>().unwrap());

    let mut x: i32 = 0;
    let mut y: i32 = 0;

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
pub fn p2(_input: String) {}
