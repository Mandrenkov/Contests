use core::cmp::{max, min};
use std::collections::HashMap;

/// Solves the Day 21 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut positions = parse_input(input);
    let mut scores = [0, 0];

    let mut throws = 0;

    while scores[0] < 1000 && scores[1] < 1000 {
        let turn = throws % 2;
        for _ in 0..3 {
            let die = throws % 100 + 1;
            positions[turn] = (positions[turn] - 1 + die) % 10 + 1;
            throws += 1;
        }
        scores[turn] += positions[turn];
    }

    let product = throws * min(scores[0], scores[1]);
    println!("{}", product);
}

/// Solves the Day 21 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let state = State {
        throws: 0,
        positions: parse_input(input),
        scores: [0, 0],
    };

    let (u1, u2) = count(&mut HashMap::new(), state);
    let universes = max(u1, u2);
    println!("{}", universes);
}

/// Parses the input to the Day 21 puzzle into a pair of positions.
fn parse_input(input: String) -> [usize; 2] {
    let mut positions = [0, 0];

    for (i, line) in input.lines().enumerate() {
        let suffix = line.split(": ").last().unwrap();
        positions[i] = suffix.parse::<usize>().unwrap();
    }

    return positions;
}

/// Represents a state in the Dirac Dice game.
#[derive(PartialEq, Eq, PartialOrd, Ord, Hash)]
struct State {
    throws: usize,
    positions: [usize; 2],
    scores: [usize; 2],
}

/// Returns the number of universes in which the first and second player win
/// from the current state of the Dirac Dice game, respectively.
fn count(cache: &mut HashMap<State, (usize, usize)>, state: State) -> (usize, usize) {
    if cache.contains_key(&state) {
        return *cache.get(&state).unwrap();
    } else if state.scores[0] >= 21 {
        return (1, 0);
    } else if state.scores[1] >= 21 {
        return (0, 1);
    }

    let turn = state.throws % 2;

    let p2 = state.positions[1 - turn];
    let s2 = state.scores[1 - turn];

    let mut counters = (0, 0);

    for r1 in 1..4 {
        for r2 in 1..4 {
            for r3 in 1..4 {
                let roll = r1 + r2 + r3;

                let p1 = (state.positions[turn] - 1 + roll) % 10 + 1;
                let s1 = state.scores[turn] + p1;

                let next = State {
                    throws: state.throws + 1,
                    positions: if turn == 0 { [p1, p2] } else { [p2, p1] },
                    scores: if turn == 0 { [s1, s2] } else { [s2, s1] },
                };

                let (u1, u2) = count(cache, next);
                counters.0 += u1;
                counters.1 += u2;
            }
        }
    }

    cache.insert(state, counters);
    return counters;
}
