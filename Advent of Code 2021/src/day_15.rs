use std::cmp::Ordering;
use std::collections::BinaryHeap;

/// Solves the Day 15 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let risks = parse_input(input);
    let adjs = make_adjacency_list(&risks);
    let risk = shortest_path(&adjs, 0, adjs.len() - 1).unwrap();
    println!("{}", risk);
}

/// Solves the Day 15 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let tile = parse_input(input);

    let rows = tile.len();
    let cols = tile[0].len();

    let mut risks = vec![vec![0usize; 5 * cols]; 5 * rows];

    for r1 in 0..5 {
        for c1 in 0..5 {
            for r2 in 0..rows {
                for c2 in 0..cols {
                    let r = r1 * rows + r2;
                    let c = c1 * cols + c2;
                    risks[r][c] = (tile[r2][c2] + r1 + c1 - 1) % 9 + 1;
                }
            }
        }
    }

    let adjs = make_adjacency_list(&risks);
    let risk = shortest_path(&adjs, 0, adjs.len() - 1).unwrap();
    println!("{}", risk);
}

/// Parses a character representing a digit to an unsigned integer.
fn char_to_usize(digit: char) -> usize {
    return digit.to_digit(10).unwrap() as usize;
}

/// Parses the input to the Day 15 puzzle.
fn parse_input(input: String) -> Vec<Vec<usize>> {
    let mut risks = Vec::<Vec<usize>>::new();

    for line in input.lines() {
        let row: Vec<usize> = line.chars().map(char_to_usize).collect();
        risks.push(row);
    }

    return risks;
}

/// Constructs an adjacency list of ``Edge``s from the given risk matrix.
fn make_adjacency_list(risks: &Vec<Vec<usize>>) -> Vec<Vec<Edge>> {
    let rows = risks.len();
    let cols = risks[0].len();

    let make_edge = |r: usize, c: usize| Edge {
        node: r * cols + c,
        cost: risks[r][c],
    };

    let mut adjacencies = Vec::<Vec<Edge>>::new();
    for r in 0..rows {
        for c in 0..cols {
            let mut edges = Vec::<Edge>::new();

            if r > 0 {
                let edge = make_edge(r - 1, c);
                edges.push(edge);
            }
            if r < rows - 1 {
                let edge = make_edge(r + 1, c);
                edges.push(edge);
            }
            if c > 0 {
                let edge = make_edge(r, c - 1);
                edges.push(edge);
            }
            if c < cols - 1 {
                let edge = make_edge(r, c + 1);
                edges.push(edge);
            }

            adjacencies.push(edges);
        }
    }

    return adjacencies;
}

//------------------------------------------------------------------------------
// The code below this dashed line was copied from:
// https://doc.rust-lang.org/std/collections/binary_heap/index.html

#[derive(Copy, Clone, Eq, PartialEq)]
struct State {
    cost: usize,
    position: usize,
}

// The priority queue depends on `Ord`.
// Explicitly implement the trait so the queue becomes a min-heap
// instead of a max-heap.
impl Ord for State {
    fn cmp(&self, other: &Self) -> Ordering {
        // Notice that the we flip the ordering on costs.
        // In case of a tie we compare positions - this step is necessary
        // to make implementations of `PartialEq` and `Ord` consistent.
        other
            .cost
            .cmp(&self.cost)
            .then_with(|| self.position.cmp(&other.position))
    }
}

// `PartialOrd` needs to be implemented as well.
impl PartialOrd for State {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

// Each node is represented as a `usize`, for a shorter implementation.
struct Edge {
    node: usize,
    cost: usize,
}

// Dijkstra's shortest path algorithm.

// Start at `start` and use `dist` to track the current shortest distance
// to each node. This implementation isn't memory-efficient as it may leave duplicate
// nodes in the queue. It also uses `usize::MAX` as a sentinel value,
// for a simpler implementation.
fn shortest_path(adj_list: &Vec<Vec<Edge>>, start: usize, goal: usize) -> Option<usize> {
    // dist[node] = current shortest distance from `start` to `node`
    let mut dist: Vec<_> = (0..adj_list.len()).map(|_| usize::MAX).collect();

    let mut heap = BinaryHeap::new();

    // We're at `start`, with a zero cost
    dist[start] = 0;
    heap.push(State {
        cost: 0,
        position: start,
    });

    // Examine the frontier with lower cost nodes first (min-heap)
    while let Some(State { cost, position }) = heap.pop() {
        // Alternatively we could have continued to find all shortest paths
        if position == goal {
            return Some(cost);
        }

        // Important as we may have already found a better way
        if cost > dist[position] {
            continue;
        }

        // For each node we can reach, see if we can find a way with
        // a lower cost going through this node
        for edge in &adj_list[position] {
            let next = State {
                cost: cost + edge.cost,
                position: edge.node,
            };

            // If so, add it to the frontier and continue
            if next.cost < dist[next.position] {
                heap.push(next);
                // Relaxation, we have now found a better way
                dist[next.position] = next.cost;
            }
        }
    }

    // Goal not reachable
    None
}
