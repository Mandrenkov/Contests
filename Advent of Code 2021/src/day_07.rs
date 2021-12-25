use std::cmp::min;

/// Solves the Day 07 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut crabs: Vec<i32> = input.split(",").map(str_to_int).collect();
    crabs.sort();

    let mut best = 0;
    for crab in &crabs {
        best += crab - crabs[0];
    }

    // Use a sweeping line algorithm with prefix and postfix components to
    // calculate the fuel cost at each crab's position.
    let mut fuel = best;
    for i in 1..crabs.len() {
        let dist = crabs[i] - crabs[i - 1];
        let before = i as i32;
        let after = crabs.len() as i32 - before;

        fuel += dist * before;
        fuel -= dist * after;
        best = min(best, fuel);
    }

    println!("{}", best);
}

/// Solves the Day 07 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let crabs: Vec<i32> = input.split(",").map(str_to_int).collect();

    let mut best = i32::MAX;

    // The best position lies somewhere between the leftmost and rightmost crabs.
    let min_x: i32 = *crabs.iter().min().unwrap();
    let max_x: i32 = *crabs.iter().max().unwrap();
    for pos in min_x..max_x + 1 {
        let mut fuel = 0;
        for crab in &crabs {
            let diff = (crab - pos).abs() as i32;
            fuel += diff * (diff + 1) / 2;
        }
        best = min(fuel, best);
    }

    println!("{}", best);
}

/// Parses a string into an integer.
fn str_to_int(token: &str) -> i32 {
    return token.parse::<i32>().unwrap();
}
