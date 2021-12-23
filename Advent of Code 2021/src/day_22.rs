use core::cmp::{max, min};

/// Solves the Day 22 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let steps = parse_input(input)
        .into_iter()
        .filter(|step| step.cuboid.min_x.abs() <= 50)
        .filter(|step| step.cuboid.min_y.abs() <= 50)
        .filter(|step| step.cuboid.min_z.abs() <= 50)
        .filter(|step| step.cuboid.max_x.abs() <= 51)
        .filter(|step| step.cuboid.max_y.abs() <= 51)
        .filter(|step| step.cuboid.max_z.abs() <= 51)
        .collect();
    let cuboids = apply(steps);
    let lit = cuboids.iter().map(volume).sum::<i64>();
    println!("{}", lit);
}

/// Solves the Day 22 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let steps = parse_input(input);
    let cuboids = apply(steps);
    let lit = cuboids.iter().map(volume).sum::<i64>();
    println!("{}", lit);
}

struct Step {
    action: String,
    cuboid: Cuboid,
}

#[derive(Debug)]
struct Cuboid {
    min_x: i64,
    min_y: i64,
    min_z: i64,
    max_x: i64,
    max_y: i64,
    max_z: i64,
}

/// Parses the input to the Day 22 puzzle into a vector of steps.
fn parse_input(input: String) -> Vec<Step> {
    let mut steps: Vec<Step> = Vec::new();

    for line in input.lines() {
        let mut halves = line.split(" ");
        let action = halves.next().unwrap();
        let bounds = halves.next().unwrap();

        let parse_range = |range: &str| -> (i64, i64) {
            let suffix: String = range.chars().skip(2).collect();
            let mut halves = suffix.split("..");
            let lo = halves.next().unwrap().parse::<i64>().unwrap();
            let hi = halves.next().unwrap().parse::<i64>().unwrap();
            return (lo, hi);
        };

        let mut coords = bounds.split(",");
        let (min_x, max_x) = parse_range(coords.next().unwrap());
        let (min_y, max_y) = parse_range(coords.next().unwrap());
        let (min_z, max_z) = parse_range(coords.next().unwrap());
        let cuboid = Cuboid {
            min_x: min_x,
            min_y: min_y,
            min_z: min_z,
            max_x: max_x + 1,
            max_y: max_y + 1,
            max_z: max_z + 1,
        };

        let step = Step {
            action: action.to_string(),
            cuboid: cuboid,
        };
        steps.push(step);
    }

    return steps;
}

/// Applies the given steps and returns the remaining (lit) cuboids.
fn apply(steps: Vec<Step>) -> Vec<Cuboid> {
    let mut cuboids: Vec<Cuboid> = Vec::new();
    for step in steps {
        let mut next_cuboids: Vec<Cuboid> = Vec::new();

        if step.action == "on" {
            let mut parts = vec![step.cuboid];

            for cuboid in &cuboids {
                let mut next_parts = Vec::new();

                for prev_part in parts.drain(..) {
                    for next_part in intersect(&cuboid, &prev_part) {
                        next_parts.push(next_part);
                    }
                }

                parts = next_parts;
            }

            for cuboid in cuboids {
                next_cuboids.push(cuboid);
            }
            for part in parts {
                next_cuboids.push(part);
            }
        } else {
            for cuboid in cuboids.drain(..) {
                for part in intersect(&step.cuboid, &cuboid) {
                    next_cuboids.push(part);
                }
            }
        }

        cuboids = next_cuboids;
    }

    return cuboids;
}

/// Partitions dst into a set of non-intersecting cuboids such that the returned
/// cuboids cover the same cubes as dst except for those which are also reside
/// in src.
fn intersect(src: &Cuboid, dst: &Cuboid) -> Vec<Cuboid> {
    let center = Cuboid {
        min_x: max(dst.min_x, min(dst.max_x, src.min_x)),
        min_y: max(dst.min_y, min(dst.max_y, src.min_y)),
        min_z: max(dst.min_z, min(dst.max_z, src.min_z)),
        max_x: min(dst.max_x, max(dst.min_x, src.max_x)),
        max_y: min(dst.max_y, max(dst.min_y, src.max_y)),
        max_z: min(dst.max_z, max(dst.min_z, src.max_z)),
    };

    return [
        // -Z
        Cuboid {
            min_x: dst.min_x,
            max_x: dst.max_x,
            min_y: dst.min_y,
            max_y: dst.max_y,
            min_z: dst.min_z,
            max_z: center.min_z,
        },
        // +Z
        Cuboid {
            min_x: dst.min_x,
            max_x: dst.max_x,
            min_y: dst.min_y,
            max_y: dst.max_y,
            min_z: center.max_z,
            max_z: dst.max_z,
        },
        // -Y
        Cuboid {
            min_x: dst.min_x,
            max_x: dst.max_x,
            min_y: dst.min_y,
            max_y: center.min_y,
            min_z: center.min_z,
            max_z: center.max_z,
        },
        // +Y
        Cuboid {
            min_x: dst.min_x,
            max_x: dst.max_x,
            min_y: center.max_y,
            max_y: dst.max_y,
            min_z: center.min_z,
            max_z: center.max_z,
        },
        // -X
        Cuboid {
            min_x: dst.min_x,
            max_x: center.min_x,
            min_y: center.min_y,
            max_y: center.max_y,
            min_z: center.min_z,
            max_z: center.max_z,
        },
        // +X
        Cuboid {
            min_x: center.max_x,
            max_x: dst.max_x,
            min_y: center.min_y,
            max_y: center.max_y,
            min_z: center.min_z,
            max_z: center.max_z,
        },
    ]
    .into_iter()
    .filter(|cuboid| volume(cuboid) > 0)
    .collect();
}

/// Returns the volume of the given cuboid.
fn volume(cuboid: &Cuboid) -> i64 {
    let dx = cuboid.max_x - cuboid.min_x;
    let dy = cuboid.max_y - cuboid.min_y;
    let dz = cuboid.max_z - cuboid.min_z;
    return dx * dy * dz;
}
