use core::cmp::max;

/// Solves the Day 17 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let ((min_x, max_x), (min_y, max_y)) = parse_input(input);
    let pairs = find_velocities(min_x, max_x, min_y, max_y);
    let peak = pairs
        .iter()
        .map(|(_, y)| if y > &0 { y * (y + 1) / 2 } else { 0 })
        .max()
        .unwrap();
    println!("{}", peak);
}

/// Solves the Day 17 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let ((min_x, max_x), (min_y, max_y)) = parse_input(input);
    let pairs = find_velocities(min_x, max_x, min_y, max_y);
    println!("{:?}", pairs.len());
}

/// Parses the input to Day 17 puzzle into ((min_x, max_x), (min_y, max_y)).
fn parse_input(input: String) -> ((isize, isize), (isize, isize)) {
    let mut parts = input.split("=");
    parts.next().unwrap();

    let x_part = parts.next().unwrap().split(",").next().unwrap();
    let y_part = parts.next().unwrap();

    let min_max_x = parse_range(x_part);
    let min_max_y = parse_range(y_part);
    return (min_max_x, min_max_y);
}

/// Parses the given string range into two signed integers.
fn parse_range(part: &str) -> (isize, isize) {
    let mut halves = part.split("..");
    let min = halves.next().unwrap().parse::<isize>().unwrap();
    let max = halves.next().unwrap().parse::<isize>().unwrap();
    return (min, max);
}

/// Finds all pairs of velocities that land in the given target area.
fn find_velocities(min_x: isize, max_x: isize, min_y: isize, max_y: isize) -> Vec<(isize, isize)> {
    let mut pairs = Vec::<(isize, isize)>::new();

    for y in min_y..1234 {
        let ts = get_times(min_y, max_y, y);
        for init_x in 1..max_x + 1 {
            for t in &ts {
                // Compute the x-position after t steps by subtracting two sums.
                let mut x = init_x * (init_x + 1) / 2;
                let bound = max(0, init_x - t + 1);
                x -= bound * (bound - 1) / 2;

                if min_x <= x && x <= max_x {
                    pairs.push((x, y));
                    break;
                }
            }
        }
    }

    return pairs;
}

/// Returns the time steps where the y-position falls in the target range.
fn get_times(min_y: isize, max_y: isize, init_y: isize) -> Vec<isize> {
    // Use the quadratic formula to solve: t * y - t * (t - 1) / 2 > max_y.
    let a = -1.0 / 2.0;
    let b = init_y as f64 + 0.5;
    let c = -max_y as f64;

    // This shouldn't be possible if init_y is chosen sensibly.
    let d = b * b - 4.0 * a * c;
    if d < 0.0 {
        return vec![];
    }

    let mut ts = Vec::<isize>::new();

    let tf1 = (-b - d.sqrt()) / (2.0 * a);
    let tf2 = (-b + d.sqrt()) / (2.0 * a);

    for tf in [tf1, tf2].iter().filter(|tf| tf >= &&0.0) {
        let mut t = tf.floor() as isize;
        let mut y = t * init_y - t * (t - 1) / 2;

        while min_y <= y {
            if y <= max_y {
                ts.push(t);
            }
            y += init_y - t;
            t += 1;
        }
    }

    return ts;
}
