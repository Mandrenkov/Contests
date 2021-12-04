/// Solves the Day 03 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let report: Vec<&str> = input.split_whitespace().collect();

    let length = report.len();
    let digits = report[0].len();
    let mut counters: Vec<usize> = vec![0; digits];

    for number in report {
        for (val, bit) in number.chars().zip(0..digits) {
            if val == '1' {
                counters[bit] += 1;
            }
        }
    }

    let mut g: u64 = 0;
    let mut e: u64 = 0;

    for ones in counters {
        g <<= 1;
        e <<= 1;
        if ones > length / 2 {
            g += 1;
        } else {
            e += 1;
        }
    }

    println!("{}", g * e);
}

/// Solves the Day 03 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let report: Vec<&str> = input.split_whitespace().collect();

    let rating = |criterion: fn(usize, usize) -> bool| {
        let mut candidates: Vec<&str> = report.clone();

        let mut digit = 0;

        while candidates.len() > 1 {
            let is_0 = |num: &str| num.chars().nth(digit).unwrap() == '0';
            let is_1 = |num: &str| num.chars().nth(digit).unwrap() == '1';

            let mut zeros: usize = 0;
            for number in &candidates {
                zeros += is_0(number) as usize;
            }

            let ones = candidates.len() - zeros;

            if criterion(zeros, ones) {
                candidates = candidates.drain(..).filter(|num| is_0(num)).collect();
            } else {
                candidates = candidates.drain(..).filter(|num| is_1(num)).collect();
            }

            digit += 1
        }

        return candidates[0];
    };

    let oxy_str = rating(|zeros, ones| ones < zeros);
    let co2_str = rating(|zeros, ones| ones >= zeros);

    let oxy_int = usize::from_str_radix(oxy_str, 2).unwrap();
    let co2_int = usize::from_str_radix(co2_str, 2).unwrap();

    println!("{}", oxy_int * co2_int);
}
