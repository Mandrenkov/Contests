use std::collections::HashMap;

/// Solves the Day 24 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let params = parse_input(input);
    let order = [9, 8, 7, 6, 5, 4, 3, 2, 1];
    let model = solve(&params, &order, 0, 0, 0).unwrap();
    println!("{}", model);
}

/// Solves the Day 24 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let params = parse_input(input);
    let order = [1, 2, 3, 4, 5, 6, 7, 8, 9];
    let model = solve(&params, &order, 0, 0, 0).unwrap();
    println!("{}", model);
}

/// Parses the given input into an array of parameter triplets.
fn parse_input(input: String) -> [(isize, isize, isize); 14] {
    let mut program: Vec<Vec<&str>> = Vec::new();
    for line in input.lines() {
        let parts = line.split(" ").collect();
        program.push(parts);
    }

    let mut params = [(0, 0, 0); 14];
    for i in 0..14 {
        let offset = 18 * i;
        let a = program[offset + 4][2].parse::<isize>().unwrap();
        let b = program[offset + 5][2].parse::<isize>().unwrap();
        let c = program[offset + 15][2].parse::<isize>().unwrap();
        params[i] = (a, b, c);
    }

    params
}

/// Runs a program.
#[allow(dead_code)]
fn run(program: &Vec<Vec<&str>>, input: &Vec<isize>) -> isize {
    let mut memory = HashMap::<&str, isize>::from([("w", 0), ("x", 0), ("y", 0), ("z", 0)]);
    let mut it = input.iter();

    let get = |memory: &HashMap<&str, isize>, token: &str| -> isize {
        if memory.contains_key(token) {
            return *memory.get(token).unwrap();
        }
        return token.parse::<isize>().unwrap();
    };

    for code in program {
        match code[0] {
            "inp" => {
                let dst = code[1];
                memory.insert(dst, *it.next().unwrap());
            }
            "add" => {
                let dst = code[1];
                let src = code[2];
                let res = get(&memory, dst) + get(&memory, src);
                memory.insert(dst, res);
            }
            "mul" => {
                let dst = code[1];
                let src = code[2];
                let res = get(&memory, dst) * get(&memory, src);
                memory.insert(dst, res);
            }
            "div" => {
                let dst = code[1];
                let src = code[2];
                if get(&memory, src) == 0 {
                    return -1;
                }
                let res = get(&memory, dst) / get(&memory, src);
                memory.insert(dst, res);
            }
            "mod" => {
                let dst = code[1];
                let src = code[2];
                if get(&memory, src) == 0 {
                    return -1;
                }
                let res = get(&memory, dst) % get(&memory, src);
                memory.insert(dst, res);
            }
            "eql" => {
                let dst = code[1];
                let src = code[2];
                let res = get(&memory, dst) == get(&memory, src);
                memory.insert(dst, res as isize);
            }
            _ => {
                panic!("There is an unknown instruction in the program.");
            }
        }
    }

    return *memory.get("z").unwrap();
}

/// Recursively finds a model number which makes the final value of ``z`` zero
/// by greedily choosing model numbers (i.e., w) that skip the if branch on line
/// 5 of the encoded program:
///
/// 1. (a, b, c) = params[index]
/// 2. w = get_input_digit()
/// 3. x = (z % 26) + b
/// 4. z /= a
/// 5. if x != w:
/// 6.     z = 26 * z + w + c
fn solve(
    params: &[(isize, isize, isize); 14],
    order: &[isize; 9],
    index: usize,
    model: isize,
    z: isize,
) -> Option<isize> {
    if index == 14 {
        return match z {
            0 => Some(model),
            _ => None,
        };
    }

    let x = z % 26 + params[index].1;
    let z = z / params[index].0;

    if 1 <= x && x <= 9 {
        // This discards some candidate answers, but those are unlikely to pan out anyway.
        return solve(params, order, index + 1, 10 * model + x, z);
    }

    for w in order {
        let i = index + 1;
        let m = 10 * model + w;
        let z = 26 * z + w + params[index].2;
        if let Some(answer) = solve(params, order, i, m, z) {
            return Some(answer);
        }
    }

    None
}
