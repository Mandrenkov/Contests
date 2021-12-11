/// Solves the Day 10 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut score = 0;

    for line in input.lines() {
        if let Some(value) = compute_corrupted_score(line) {
            score += value;
        }
    }

    println!("{}", score);
}

/// Solves the Day 10 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let mut scores = Vec::<usize>::new();

    for line in input.lines() {
        if let None = compute_corrupted_score(line) {
            let mut stack = Vec::<char>::new();

            for rune in line.chars() {
                if rune == '(' || rune == '[' || rune == '{' || rune == '<' {
                    stack.push(rune);
                } else if rune == ')' || rune == ']' || rune == '}' || rune == '>' {
                    stack.pop();
                }
            }

            stack.reverse();

            let mut score = 0;
            for rune in &stack {
                score *= 5;
                score += "_([{<".find(*rune).unwrap();
            }

            scores.push(score);
        }
    }

    scores.sort();
    let median = scores[scores.len() / 2];

    println!("{}", median);
}

/// Returns the corrupted score of the given line.
fn compute_corrupted_score(line: &str) -> Option<usize> {
    let mut stack = Vec::<char>::new();

    for rune in line.chars() {
        if rune == '(' || rune == '[' || rune == '{' || rune == '<' {
            stack.push(rune);
        } else if rune == ')' || rune == ']' || rune == '}' || rune == '>' {
            let open = match rune {
                ')' => '(',
                ']' => '[',
                '}' => '{',
                _ => '<',
            };

            if stack.len() == 0 || *stack.last().unwrap() != open {
                return match rune {
                    ')' => Some(3),
                    ']' => Some(57),
                    '}' => Some(1197),
                    _ => Some(25137),
                };
            }

            stack.pop();
        }
    }

    return None;
}
