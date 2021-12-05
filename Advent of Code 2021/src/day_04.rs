/// Solves the Day 04 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut lines = input.lines();
    let draws = parse_draws(&mut lines);
    let boards = parse_boards(&mut lines);

    let winner = 1;
    let (draw, board) = play(draws, boards, winner);

    let mut left = 0;
    for r2 in 0..5 {
        for c2 in 0..5 {
            if board[r2][c2] != -1 {
                left += board[r2][c2];
            }
        }
    }

    println!("{}", draw * left);
}

/// Solves the Day 04 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let mut lines = input.lines();
    let draws = parse_draws(&mut lines);
    let boards = parse_boards(&mut lines);

    let winner = boards.len() as i32;
    let (draw, board) = play(draws, boards, winner);

    let mut left = 0;
    for r2 in 0..5 {
        for c2 in 0..5 {
            if board[r2][c2] != -1 {
                left += board[r2][c2];
            }
        }
    }

    println!("{}", draw * left);
}

/// Parses a string into an integer.
fn str_to_int(token: &str) -> i32 {
    return token.parse::<i32>().unwrap();
}

/// Returns the draws for the Bingo game.
fn parse_draws<'a>(lines: &mut impl Iterator<Item = &'a str>) -> Vec<i32> {
    let draws = lines.next().unwrap().split(",").map(str_to_int).collect();
    lines.next();
    return draws;
}

/// Returns the boards for the Bingo game.
fn parse_boards<'a>(lines: &mut impl Iterator<Item = &'a str>) -> Vec<[[i32; 5]; 5]> {
    let mut boards: Vec<[[i32; 5]; 5]> = Vec::new();
    loop {
        let mut board = [[0i32; 5]; 5];
        for i in 0..5 {
            let mut row = lines.next().unwrap().split_whitespace().map(str_to_int);
            for j in 0..5 {
                board[i][j] = row.next().unwrap();
            }
        }

        boards.push(board);

        if lines.next() == None {
            return boards;
        }
    }
}

/// Plays Bingo and returns the draw and board of the winning board.
fn play(draws: Vec<i32>, boards: Vec<[[i32; 5]; 5]>, winner: i32) -> (i32, [[i32; 5]; 5]) {
    let mut boards = boards;

    let mut won = vec![false; boards.len()];
    let mut wins = 0;

    for draw in draws {
        for i in 0..boards.len() {
            if !won[i] {
                let board = &mut boards[i];
                for r1 in 0..5 {
                    for c1 in 0..5 {
                        if board[r1][c1] == draw {
                            board[r1][c1] = -1;
                            if bingo(board) {
                                won[i] = true;
                                wins += 1;
                                if wins == winner {
                                    return (draw, board.to_owned());
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    panic!("Not enough boards won at Bingo.")
}

/// Returns true iff there is a row of column filled with -1 in the given board.
fn bingo(board: &[[i32; 5]; 5]) -> bool {
    for i in 0..5 {
        let mut s1 = 0;
        let mut s2 = 0;
        for j in 0..5 {
            s1 += board[i][j];
            s2 += board[j][i];
        }
        if s1 == -5 || s2 == -5 {
            return true;
        }
    }
    return false;
}
