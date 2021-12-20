use core::cmp::max;
use rand::seq::SliceRandom;
use rand::thread_rng;
use std::collections::HashSet;

/// Solves the Day 19 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut scanners: Vec<Vec<Vec<isize>>> = parse_input(input);

    let merges = scanners.len() - 1;
    for _ in 0..merges {
        scanners.shuffle(&mut thread_rng());
        'outer: for i in 0..scanners.len() {
            for j in i + 1..scanners.len() {
                if let Some(transform) = overlap(&scanners[i], &scanners[j]) {
                    let merged = merge(&scanners[i], &scanners[j], transform);
                    scanners[i] = merged;
                    for k in j + 1..scanners.len() {
                        scanners[k - 1] = scanners[k].clone();
                    }
                    scanners.pop();
                    break 'outer;
                }
            }
        }
    }

    println!("{}", scanners[0].len());
}

/// Solves the Day 19 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let mut scanners: Vec<Vec<Vec<isize>>> = parse_input(input);

    let mut positions: Vec<(isize, isize, isize)> = vec![(0, 0, 0)];

    let merges = scanners.len() - 1;
    for _ in 0..merges {
        'outer: for i in 0..scanners.len() {
            for j in i + 1..scanners.len() {
                if let Some(transform) = overlap(&scanners[i], &scanners[j]) {
                    positions.push(transform.1);
                    let merged = merge(&scanners[i], &scanners[j], transform);
                    scanners[i] = merged;
                    for k in j + 1..scanners.len() {
                        scanners[k - 1] = scanners[k].clone();
                    }
                    scanners.pop();
                    break 'outer;
                }
            }
        }
    }

    let mut best = 0;
    for (x1, y1, z1) in &positions {
        for (x2, y2, z2) in &positions {
            let dist = (x2 - x1).abs() + (y2 - y1).abs() + (z2 - z1).abs();
            best = max(best, dist);
        }
    }

    println!("{}", best);
}

/// Parses the input for the Day 19 puzzle.
fn parse_input(input: String) -> Vec<Vec<Vec<isize>>> {
    let mut scanners: Vec<Vec<Vec<isize>>> = vec![];

    let lines = &mut input.lines();
    lines.next();

    // Converts a string into a signed integer.
    let str_to_int = |token: &str| token.parse::<isize>().unwrap();

    // Parses a scanner while consuming the input line iterator.
    let mut parse_scanner = || -> (Vec<Vec<isize>>, bool) {
        let mut scanner: Vec<Vec<isize>> = vec![];

        loop {
            let line = lines.next().unwrap();
            if line.is_empty() {
                return (scanner, lines.next() == None);
            }

            let beacon = line.split(",").map(str_to_int).collect();
            scanner.push(beacon);
        }
    };

    loop {
        let (scanner, has_next) = parse_scanner();
        scanners.push(scanner);

        if has_next {
            return scanners;
        }
    }
}

/// Checks if the beacons detected by two scanners overlap.
fn overlap(v1: &Vec<Vec<isize>>, v2: &Vec<Vec<isize>>) -> Option<(usize, (isize, isize, isize))> {
    let mut s1 = HashSet::<(isize, isize, isize)>::new();
    for beacon in v1 {
        let x = beacon[0];
        let y = beacon[1];
        let z = beacon[2];
        s1.insert((x, y, z));
    }

    for orient in 0..24 {
        let mut v3: Vec<Vec<isize>> = vec![];
        for beacon in v2 {
            let (x, y, z) = rotate(beacon[0], beacon[1], beacon[2], orient);
            v3.push(vec![x, y, z]);
        }

        for i in 0..v1.len() {
            for j in 0..v3.len() {
                let offset = (
                    v1[i][0] - v3[j][0],
                    v1[i][1] - v3[j][1],
                    v1[i][2] - v3[j][2],
                );

                let mut s2 = HashSet::<(isize, isize, isize)>::new();
                for beacon in &v3 {
                    let x = beacon[0] + offset.0;
                    let y = beacon[1] + offset.1;
                    let z = beacon[2] + offset.2;
                    s2.insert((x, y, z));
                }

                if s1.intersection(&s2).count() >= 12 {
                    return Some((orient, offset));
                }
            }
        }
    }

    return None;
}

/// Rotates a beacon according to orient which must fall in the range [0, 24).
fn rotate(x: isize, y: isize, z: isize, orient: usize) -> (isize, isize, isize) {
    return match orient {
        // X = (1, 0, 0)
        0 => (
            x,
            y, // Y = (0, 1, 0)
            z, // Z = (0, 0, 1)
        ),
        1 => (
            x,
            -z, // Y = (0, 0, 1)
            y, // Z = (0, -1, 0)
        ),
        2 => (
            x,
            -y, // Y = (0, -1, 0)
            -z, // Z = (0, 0, -1)
        ),
        3 => (
            x,
            z, // Y = (0, 0, -1)
            -y, // Z = (0, 1, 0)
        ),

        // X = (-1, 0, 0)
        4 => (
            -x,
            -y, // Y = (0, -1, 0)
            z, // Z = (0, 0, 1)
        ),
        5 => (
            -x,
            z, // Y = (0, 0, 1)
            y, // Z = (0, 1, 0)
        ),
        6 => (
            -x,
            y, // Y = (0, 1, 0)
            -z, // Z = (0, 0, -1)
        ),
        7 => (
            -x,
            -z, // Y = (0, 0, -1)
            -y, // Z = (0, -1, 0)
        ),

        // X = (0, 1, 0)
        8 => (
            z,
            x, // Y = (0, 0, 1)
            y, // Z = (1, 0, 0)
        ),
        9 => (
            y,
            x, // Y = (1, 0, 0)
            -z, // Z = (0, 0, -1)
        ),
        10 => (
            -z,
            x, // Y = (0, 0, -1)
            -y, // Z = (-1, 0, 0)
        ),
        11 => (
            -y,
            x, // Y = (-1, 0, 0)
            z, // Z = (0, 0, 1)
        ),

        // X = (0, -1, 0)
        12 => (
            y,
            -x, // Y = (1, 0, 0)
            z, // Z = (0, 0, 1)
        ),
        13 => (
            -z,
            -x, // Y = (0, 0, 1)
            y, // Z = (-1, 0, 0)
        ),
        14 => (
            -y,
            -x, // Y = (-1, 0, 0)
            -z, // Z = (0, 0, -1)
        ),
        15 => (
            z,
            -x, // Y = (0, 0, -1)
            -y, // Z = (1, 0, 0)
        ),

        // X = (0, 0, 1)
        16 => (
            y,
            z, // Y = (1, 0, 0)
            x, // Z = (0, 1, 0)
        ),
        17 => (
            -z,
            y, // Y = (0, 1, 0)
            x, // Z = (-1, 0, 0)
        ),
        18 => (
            -y,
            -z, // Y = (-1, 0, 0)
            x, // Z = (0, -1, 0)
        ),
        19 => (
            z,
            -y, // Y = (0, -1, 0)
            x, // Z = (1, 0, 0)
        ),

        // X = (0, 0, -1)
        20 => (
            z,
            y, // Y = (0, 1, 0)
            -x, // Z = (1, 0, 0)
        ),
        21 => (
            y,
            -z, // Y = (1, 0, 0)
            -x, // Z = (0, -1, 0)
        ),
        22 => (
            -z,
            -y, // Y = (0, -1, 0)
            -x, // Z = (-1, 0, 0)
        ),
        23 => (
            -y,
            z, // Y = (-1, 0, 0)
            -x, // Z = (0, 1, 0)
        ),

        _ => {
            panic!("Illegal orientation!");
        }
    };
}

/// Merges the beacon readings of two scanners.
fn merge(
    v1: &Vec<Vec<isize>>,
    v2: &Vec<Vec<isize>>,
    transform: (usize, (isize, isize, isize)),
) -> Vec<Vec<isize>> {
    let (orient, translate) = transform;

    let mut s1 = HashSet::<(isize, isize, isize)>::new();
    for beacon in v1 {
        let x = beacon[0];
        let y = beacon[1];
        let z = beacon[2];
        s1.insert((x, y, z));
    }

    for beacon in v2 {
        let (mut x, mut y, mut z) = rotate(beacon[0], beacon[1], beacon[2], orient);
        x += translate.0;
        y += translate.1;
        z += translate.2;
        if !s1.contains(&(x, y, z)) {
            s1.insert((x, y, z));
        }
    }

    let mut v3: Vec<Vec<isize>> = vec![];
    for (x, y, z) in s1 {
        v3.push(vec![x, y, z]);
    }

    return v3;
}
