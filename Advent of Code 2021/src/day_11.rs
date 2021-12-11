/// Solves the Day 11 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut energies = parse_input(input);

    let mut flashes = 0;
    for _step in 0..100 {
        flashes += simulate(&mut energies);
    }

    println!("{}", flashes);
}

/// Solves the Day 11 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let mut energies = parse_input(input);

    let mut steps = 1;
    while simulate(&mut energies) < 100 {
        steps += 1;
    }

    println!("{}", steps);
}

/// Parses the given input into a 2D array of energy levels.
fn parse_input(input: String) -> [[usize; 10]; 10] {
    let mut energies = [[0; 10]; 10];

    let char_to_u32 = |rune: char| rune.to_digit(10).unwrap();

    for (i, line) in input.lines().enumerate() {
        for (j, energy) in line.chars().map(char_to_u32).enumerate() {
            energies[i][j] = energy as usize;
        }
    }

    return energies;
}

/// Simulates a step and returns the number of flashes that occurred.
fn simulate(energies: &mut [[usize; 10]; 10]) -> usize {
    let mut flashes = Vec::<(usize, usize)>::new();

    for i in 0..10 {
        for j in 0..10 {
            energies[i][j] += 1;
            if energies[i][j] == 10 {
                energies[i][j] = 0;
                flashes.push((i, j));
            }
        }
    }

    let mut counter = 0;

    while counter < flashes.len() {
        let beg = counter;
        let end = flashes.len();

        for k in beg..end {
            let (i, j) = flashes[k];
            for dr in 0..3 {
                for dc in 0..3 {
                    if dr == 1 && dc == 1 || i + dr == 0 || j + dc == 0 {
                        continue;
                    }

                    let r = i + dr - 1;
                    let c = j + dc - 1;
                    if r < 10 && c < 10 && energies[r][c] > 0 {
                        energies[r][c] += 1;
                        if energies[r][c] == 10 {
                            energies[r][c] = 0;
                            flashes.push((r, c));
                        }
                    }
                }
            }
        }

        counter = end;
    }

    return counter;
}
