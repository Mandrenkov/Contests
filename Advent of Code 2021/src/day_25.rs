use colored::*;

/// Solves the Day 25 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut map = Vec::new();
    for line in input.lines() {
        let row: Vec<char> = line.chars().collect();
        map.push(row);
    }

    let rows = map.len();
    let cols = map[0].len();

    let mut changed = true;
    let mut steps = 0;

    while changed {
        // println!("Steps = {}.", steps);
        // draw(&map);
        // println!();

        let mut prev_map = map.clone();
        let mut next_map = prev_map.clone();

        for r in 0..rows {
            for c1 in 0..cols {
                if prev_map[r][c1] == '>' {
                    let c2 = (c1 + 1) % cols;
                    if prev_map[r][c2] == '.' {
                        next_map[r][c1] = '.';
                        next_map[r][c2] = '>';
                    }
                }
            }
        }

        prev_map = next_map.clone();

        for r1 in 0..rows {
            let r2 = (r1 + 1) % rows;
            for c in 0..cols {
                if prev_map[r1][c] == 'v' {
                    if prev_map[r2][c] == '.' {
                        next_map[r1][c] = '.';
                        next_map[r2][c] = 'v';
                    }
                }
            }
        }

        steps += 1;
        changed = map != next_map;
        map = next_map;
    }

    println!("{}", steps);
}

/// Solves the Day 25 Part 2 puzzle with respect to the given input.
pub fn part_2(_input: String) {
    println!("Merry Christmas!");
}

/// Draws the current state of the map using pretty colours!
#[allow(dead_code)]
fn draw(map: &Vec<Vec<char>>) {
    for row in map {
        for symbol in row {
            match symbol {
                '>' => print!("{}", ">".green()),
                'v' => print!("{}", "v".red()),
                _ => print!("."),
            }
        }
        println!();
    }
}
