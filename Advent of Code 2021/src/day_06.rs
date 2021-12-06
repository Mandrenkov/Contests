/// Solves the Day 06 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut ages = parse_ages(input);

    for _ in 0..80 {
        simulate(&mut ages);
    }

    let fish: u64 = ages.iter().sum();
    println!("{}", fish);
}

/// Solves the Day 06 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let mut ages = parse_ages(input);

    for _ in 0..256 {
        simulate(&mut ages);
    }

    let fish: u64 = ages.iter().sum();
    println!("{}", fish);
}

/// Parses the ages of the fish from the given input string.
fn parse_ages(input: String) -> [u64; 9] {
    let mut ages = [0u64; 9];

    for age in input.split(",") {
        let age = age.parse::<usize>().unwrap();
        ages[age] += 1;
    }

    return ages;
}

/// Simulates a day of the fish growth cycle.
fn simulate(ages: &mut [u64; 9]) {
    let zero = ages[0];

    for j in 0..8 {
        ages[j] = ages[j + 1];
    }

    ages[6] += zero;
    ages[8] = zero;
}
