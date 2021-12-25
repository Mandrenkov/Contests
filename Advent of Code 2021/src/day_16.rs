use core::cmp::{max, min};

/// Solves the Day 16 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    for line in input.split("\n") {
        let bits = parse_line(line);

        let mut sum = 0;
        let mut beg = 0;
        let end = bits.len();

        while beg < end - 10 {
            let (ver, _, next) = read(&bits, beg);
            sum += ver;
            beg = next;
        }

        println!("{}", sum);
    }
}

/// Solves the Day 16 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    for line in input.split("\n") {
        let bits = parse_line(line);
        let (_, ans, _) = read(&bits, 0);
        println!("{}", ans);
    }
}

/// Parses a hexadecimal string into a binary vector.
fn parse_line(line: &str) -> Vec<bool> {
    let mut bits = Vec::<bool>::new();
    for hex in line.chars() {
        let quartet = match hex {
            '0' => [0, 0, 0, 0],
            '1' => [0, 0, 0, 1],
            '2' => [0, 0, 1, 0],
            '3' => [0, 0, 1, 1],
            '4' => [0, 1, 0, 0],
            '5' => [0, 1, 0, 1],
            '6' => [0, 1, 1, 0],
            '7' => [0, 1, 1, 1],
            '8' => [1, 0, 0, 0],
            '9' => [1, 0, 0, 1],
            'A' => [1, 0, 1, 0],
            'B' => [1, 0, 1, 1],
            'C' => [1, 1, 0, 0],
            'D' => [1, 1, 0, 1],
            'E' => [1, 1, 1, 0],
            'F' => [1, 1, 1, 1],
            _ => panic!("Unknown digit"),
        };
        for bit in quartet {
            bits.push(bit == 1);
        }
    }
    return bits;
}

/// Reads a packet and returns a (sum, ans, idx) tuple.
fn read(bits: &Vec<bool>, beg: usize) -> (usize, usize, usize) {
    let (ver, idx) = bin_to_dec(bits, beg, beg + 3);
    let (typ, idx) = bin_to_dec(bits, idx, idx + 3);

    let end: usize;

    // Used for part 1.
    let mut sum = ver;

    // Used for part 2.
    let mut ans = match typ {
        1 => 1,
        2 => usize::MAX,
        _ => 0,
    };

    if typ == 4 {
        let mut idx = idx;

        while bits[idx] {
            let val = bin_to_dec(bits, idx + 1, idx + 5).0;
            ans = 16 * ans + val;
            idx += 5;
        }

        let val = bin_to_dec(bits, idx + 1, idx + 5).0;
        ans = 16 * ans + val;
        end = idx + 5;
    } else if !bits[idx] {
        let (size, mut idx) = bin_to_dec(bits, idx + 1, idx + 16);
        end = idx + size;
        while idx < end {
            let (ver, val, cur) = read(bits, idx);
            idx = cur;
            sum += ver;

            match typ {
                0 => {
                    ans += val;
                }
                1 => {
                    ans *= val;
                }
                2 => {
                    ans = min(ans, val);
                }
                3 => {
                    ans = max(ans, val);
                }
                5 => {
                    if idx == end {
                        ans = (ans > val) as usize;
                    } else {
                        ans = val;
                    }
                }
                6 => {
                    if idx == end {
                        ans = (ans < val) as usize;
                    } else {
                        ans = val;
                    }
                }
                _ => {
                    if idx == end {
                        ans = (ans == val) as usize;
                    } else {
                        ans = val;
                    }
                }
            }
        }
    } else {
        let (subs, mut idx) = bin_to_dec(bits, idx + 1, idx + 12);
        for i in 0..subs {
            let (ver, val, cur) = read(bits, idx);
            idx = cur;
            sum += ver;

            match typ {
                0 => {
                    ans += val;
                }
                1 => {
                    ans *= val;
                }
                2 => {
                    ans = min(ans, val);
                }
                3 => {
                    ans = max(ans, val);
                }
                5 => ans = if i == 1 { (ans > val) as usize } else { val },
                6 => ans = if i == 1 { (ans < val) as usize } else { val },
                _ => ans = if i == 1 { (ans == val) as usize } else { val },
            }
        }
        end = idx;
    }

    return (sum, ans, end);
}

/// Converts a segment of bits into a decimal number; returns (num, end).
fn bin_to_dec(bits: &Vec<bool>, beg: usize, end: usize) -> (usize, usize) {
    let mut num = 0;
    for i in beg..end {
        num = 2 * num + bits[i] as usize;
    }
    return (num, end);
}
