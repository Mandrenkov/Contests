/// Solves the Day 20 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let lit = solve(input, 2);
    println!("{}", lit);
}

/// Solves the Day 20 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let lit = solve(input, 50);
    println!("{}", lit);
}

/// Solves the Day 20 puzzle with respect to the given input and enhancements.
fn solve(input: String, steps: usize) -> usize {
    let alg: Vec<char> = input.lines().next().unwrap().chars().collect();

    let mut img: Vec<Vec<char>> = Vec::new();
    for line in input.lines().skip(2) {
        let row: Vec<char> = line.chars().collect();
        img.push(row);
    }

    img = pad(&img, '.');

    for _ in 0..steps {
        img = enhance(&img, &alg);
        img = pad(&img, img[0][0]);
    }

    return img.into_iter().flatten().filter(|&c| c == '#').count();
}

/// Pads the given image with two cells of the given character.
fn pad(old_img: &Vec<Vec<char>>, default: char) -> Vec<Vec<char>> {
    let h = old_img.len();
    let w = old_img[0].len();

    let mut new_img = vec![vec![default; w + 4]; h + 4];
    for r in 0..h {
        for c in 0..w {
            new_img[r + 2][c + 2] = old_img[r][c];
        }
    }

    return new_img;
}

/// Enhances the given image using the provided image enhancement algorithm.
fn enhance(old_img: &Vec<Vec<char>>, alg: &Vec<char>) -> Vec<Vec<char>> {
    let h = old_img.len();
    let w = old_img[0].len();

    let mut new_img = vec![vec![old_img[0][0]; w]; h];
    for r in 0..h {
        for c in 0..w {
            let mut index = 0;
            for dr in 0..3 {
                for dc in 0..3 {
                    index <<= 1;
                    let i = r + dr;
                    let j = c + dc;
                    if 0 < i && i <= h && 0 < j && j <= w {
                        index += (old_img[i - 1][j - 1] == '#') as usize;
                    } else {
                        index += (old_img[0][0] == '#') as usize;
                    }
                }
            }
            new_img[r][c] = alg[index];
        }
    }

    return new_img;
}
