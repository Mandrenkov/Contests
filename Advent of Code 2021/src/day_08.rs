use itertools::Itertools;
use std::collections::HashSet;

/// Solves the Day 08 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut counter = 0;

    let uniques: [usize; 4] = [2, 3, 4, 7];
    let desired = |digit: &&str| uniques.contains(&digit.len());

    for line in input.lines() {
        let halves = line.split(" | ");
        let digits = halves.last().unwrap().split(" ");
        counter += digits.filter(desired).count();
    }

    println!("{}", counter);
}

/// Solves the Day 08 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let mut counter = 0;

    let want_segs_vec = vec![
        String::from("abcefg"),
        String::from("cf"),
        String::from("acdeg"),
        String::from("acdfg"),
        String::from("bcdf"),
        String::from("abdfg"),
        String::from("abdefg"),
        String::from("acf"),
        String::from("abcdefg"),
        String::from("abcdfg"),
    ];

    let want_segs_set = HashSet::from_iter(want_segs_vec.iter().cloned());

    for line in input.lines() {
        let mut halves = line.split(" | ");
        let inputs: Vec<&str> = halves.next().unwrap().split(" ").collect();
        let digits: Vec<&str> = halves.next().unwrap().split(" ").collect();

        for mapping in (0..7).permutations(7) {
            let seg_to_num = |seg: char| "abcdefg".find(seg).unwrap();
            let num_to_num = |num: usize| mapping[num];
            let num_to_seg = |num: usize| "abcdefg".chars().nth(num).unwrap();

            let mut have_segs_set = HashSet::<String>::new();
            for input in &inputs {
                let seg: String = input
                    .chars()
                    .map(seg_to_num)
                    .map(num_to_num)
                    .map(num_to_seg)
                    .sorted()
                    .collect();
                have_segs_set.insert(seg);
            }

            if have_segs_set == want_segs_set {
                let mut display = 0;
                for digit in &digits {
                    let seg: String = digit
                        .chars()
                        .map(seg_to_num)
                        .map(num_to_num)
                        .map(num_to_seg)
                        .sorted()
                        .collect();
                    let num = want_segs_vec.iter().position(|s| s == &seg);
                    display = 10 * display + num.unwrap();
                }

                counter += display;
                break;
            }
        }
    }

    println!("{}", counter);
}
