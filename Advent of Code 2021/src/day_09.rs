/// Solves the Day 09 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let (heights, w, h) = parse_input(input);

    let mut sum = 0;
    for r in 0..h {
        for c in 0..w {
            let mut neighbours = Vec::<u32>::new();
            if r > 0 {
                neighbours.push(heights[r - 1][c]);
            }
            if r < h - 1 {
                neighbours.push(heights[r + 1][c]);
            }
            if c > 0 {
                neighbours.push(heights[r][c - 1]);
            }
            if c < w - 1 {
                neighbours.push(heights[r][c + 1]);
            }

            let low = neighbours.iter().all(|h| h > &heights[r][c]);
            if low {
                sum += heights[r][c] + 1;
            }
        }
    }

    println!("{}", sum);
}

/// Solves the Day 09 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let (heights, w, h) = parse_input(input);

    let mut visited = vec![vec![false; w]; h];

    let mut basins = Vec::<usize>::new();
    for r in 0..h {
        for c in 0..w {
            if !visited[r][c] {
                let basin = dfs(&heights, &mut visited, h, w, r, c);
                if basin > 0 {
                    basins.push(basin);
                }
            }
        }
    }

    basins.sort();
    basins.reverse();

    let product = basins[0] * basins[1] * basins[2];
    println!("{}", product);
}

/// Parses the heightmap (along with its width and height) from the given input.
fn parse_input(input: String) -> (Vec<Vec<u32>>, usize, usize) {
    let mut heights = Vec::<Vec<u32>>::new();

    for line in input.lines() {
        let row: Vec<u32> = line.chars().map(char_to_u32).collect();
        heights.push(row);
    }

    let h = heights.len();
    let w = heights[0].len();

    return (heights, w, h);
}

/// Parses a character representing a digit to an unsigned integer.
fn char_to_u32(digit: char) -> u32 {
    return digit.to_digit(10).unwrap();
}

/// Returns the remaining area of the basin containing location (r, c).
fn dfs(
    heights: &Vec<Vec<u32>>,
    visited: &mut Vec<Vec<bool>>,
    h: usize,
    w: usize,
    r: usize,
    c: usize,
) -> usize {
    if visited[r][c] || heights[r][c] == 9 {
        return 0;
    }

    visited[r][c] = true;

    let mut area = 1;
    if r > 0 {
        area += dfs(heights, visited, h, w, r - 1, c);
    }
    if r < h - 1 {
        area += dfs(heights, visited, h, w, r + 1, c);
    }
    if c > 0 {
        area += dfs(heights, visited, h, w, r, c - 1);
    }
    if c < w - 1 {
        area += dfs(heights, visited, h, w, r, c + 1);
    }
    return area;
}
