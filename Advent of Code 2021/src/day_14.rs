use std::collections::HashMap;

/// Solves the Day 14 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    println!("{}", solve(input, 10));
}

/// Solves the Day 14 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    println!("{}", solve(input, 40));
}

/// Solves the Day 14 puzzle with respect to the given input and step count.
fn solve(input: String, steps: usize) -> usize {
    let mut lines = input.lines();
    let template: String = lines.next().unwrap().chars().collect();

    lines.next();

    let mut rules = HashMap::<(char, char), char>::new();
    while let Some(rule) = lines.next() {
        let mut halves = rule.split(" -> ");
        let lhs = halves.next().unwrap();
        let rhs = halves.next().unwrap();

        let x = lhs.chars().nth(0).unwrap();
        let y = lhs.chars().nth(1).unwrap();
        let z = rhs.chars().nth(0).unwrap();
        rules.insert((x, y), z);
    }

    let mut pairs = HashMap::<(char, char), usize>::new();
    for (x, y) in template.chars().zip(template.chars().skip(1)) {
        *pairs.entry((x, y)).or_insert(0) += 1;
    }

    for _ in 0..steps {
        pairs = step(&rules, &pairs);
    }

    let counts = count(&template, &pairs);
    let max = counts.values().max().unwrap();
    let min = counts.values().min().unwrap();
    return max - min;
}

/// Simulates a step of the pair insertion process.
fn step(
    rules: &HashMap<(char, char), char>,
    old_pairs: &HashMap<(char, char), usize>,
) -> HashMap<(char, char), usize> {
    let mut new_pairs = HashMap::<(char, char), usize>::new();
    for (pair, count) in old_pairs {
        if rules.contains_key(pair) {
            let (x, y) = pair;
            let z = rules.get(pair).unwrap();
            *new_pairs.entry((*x, *z)).or_insert(0) += *count;
            *new_pairs.entry((*z, *y)).or_insert(0) += *count;
        } else {
            *new_pairs.entry(*pair).or_insert(0) += *count;
        }
    }
    return new_pairs;
}

/// Returns the difference between the smallest and largest letter frequencies.
fn count(template: &String, pairs: &HashMap<(char, char), usize>) -> HashMap<char, usize> {
    let mut counts = HashMap::<char, usize>::new();
    for ((x, _), count) in pairs {
        *counts.entry(*x).or_insert(0) += *count;
    }

    let end = template.chars().last().unwrap();
    *counts.entry(end).or_insert(0) += 1;

    return counts;
}
