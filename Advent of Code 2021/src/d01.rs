/// Solves the Day 01 Part 1 puzzle with respect to the given input.
pub fn p1(input: String) {
    let depths = parse_input(input);

    let mut increases = 0;
    for i in 1 .. depths.len() {
        let increase = depths[i - 1] < depths[i];
        increases += increase as i32;
    }

    println!("{}", increases);
}

/// Solves the Day 01 Part 2 puzzle with respect to the given input.
pub fn p2(input: String) {
    let depths = parse_input(input);

    let mut prefixes: Vec<i32> = vec![0; depths.len() + 1];
    for i in 1 .. prefixes.len() {
        prefixes[i] = prefixes[i - 1] + depths[i - 1];
    }

    let mut increases = 0;
    for i in 3 .. depths.len() {
        let prev = prefixes[i] - prefixes[i - 3];
        let next = prefixes[i + 1] - prefixes[i - 2];
        let increase = prev < next;
        increases += increase as i32;
    }

    println!("{}", increases);
}

/// Parses the given input into a vector of integers.
fn parse_input(input: String) -> Vec<i32> {
    let mut depths: Vec<i32> = Vec::new();
    for depth in input.split("\n") {
        depths.push(depth.parse().unwrap());
    }
    return depths;
}
