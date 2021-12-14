use core::cmp::max;
use std::collections::HashSet;

/// Solves the Day 13 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let (mut paper, folds, rows, cols) = parse_input(input);

    if folds.chars().next().unwrap() == 'x' {
        paper = fold_x(&mut paper, cols);
    } else {
        paper = fold_y(&mut paper, rows);
    }

    println!("{}", paper.len());
}

/// Solves the Day 13 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let (mut paper, folds, mut rows, mut cols) = parse_input(input);

    for dir in folds.chars() {
        if dir == 'x' {
            paper = fold_x(&mut paper, cols);
            cols /= 2;
        } else {
            paper = fold_y(&mut paper, rows);
            rows /= 2;
        }
    }

    for r in 0..rows {
        for c in 0..cols {
            if paper.contains(&(r, c)) {
                print!("#");
            } else {
                print!(".");
            }
        }
        println!();
    }
}

/// Parses the input to the Day 13 puzzle.
fn parse_input(input: String) -> (HashSet<(usize, usize)>, String, usize, usize) {
    let mut paper = HashSet::<(usize, usize)>::new();
    let mut folds = String::new();

    let mut rows = 0;
    let mut cols = 0;

    let mut instructions = false;

    for line in input.lines() {
        if line.is_empty() {
            instructions = true;
        } else if !instructions {
            let mut it = line.split(",");
            let c = it.next().unwrap().parse::<usize>().unwrap();
            let r = it.next().unwrap().parse::<usize>().unwrap();
            rows = max(rows, r + 1);
            cols = max(cols, c + 1);
            paper.insert((r, c));
        } else {
            let dir = line.split("=").next().unwrap().chars().last().unwrap();
            folds.push(dir);
        }
    }

    return (paper, folds, rows, cols);
}

/// Folds the paper in half vertically.
fn fold_x(old_paper: &mut HashSet<(usize, usize)>, cols: usize) -> HashSet<(usize, usize)> {
    let mut new_paper = HashSet::<(usize, usize)>::new();
    for (r, c) in old_paper.iter() {
        if *c < cols / 2 {
            new_paper.insert((*r, *c));
        } else {
            new_paper.insert((*r, cols - 1 - *c));
        }
    }
    return new_paper;
}

/// Folds the paper in half horizontally.
fn fold_y(old_paper: &mut HashSet<(usize, usize)>, rows: usize) -> HashSet<(usize, usize)> {
    let mut new_paper = HashSet::<(usize, usize)>::new();
    for (r, c) in old_paper.iter() {
        if *r < rows / 2 {
            new_paper.insert((*r, *c));
        } else {
            new_paper.insert((rows - 1 - *r, *c));
        }
    }
    return new_paper;
}
