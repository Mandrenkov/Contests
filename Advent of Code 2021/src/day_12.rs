use std::collections::{HashMap, HashSet};

/// Solves the Day 12 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    println!("{}", paths(input, true));
}

/// Solves the Day 12 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    println!("{}", paths(input, false));
}

/// Solves the Day 12 puzzle with respect to the given input and ``twice``.
fn paths(input: String, twice: bool) -> usize {
    let mut graph = HashMap::<&str, Vec<&str>>::new();

    for line in input.lines() {
        let mut halves = line.split("-");
        let tail = halves.next().unwrap();
        let head = halves.next().unwrap();

        if graph.get(head) == None {
            graph.insert(head, Vec::new());
        }
        if graph.get(tail) == None {
            graph.insert(tail, Vec::new());
        }

        graph.get_mut(head).unwrap().push(tail);
        graph.get_mut(tail).unwrap().push(head);
    }

    let mut visited = HashSet::<String>::new();
    visited.insert("start".to_string());

    return dfs(&graph, "start", &mut visited, twice);
}

/// Returns the number of paths from the given node to "end" where ``twice``
/// indicates whether a small cave has already been visited twice.
fn dfs(
    graph: &HashMap<&str, Vec<&str>>,
    node: &str,
    visited: &mut HashSet<String>,
    twice: bool,
) -> usize {
    if node == "end" {
        return 1;
    }

    let mut paths = 0;

    for next in graph.get(node).unwrap() {
        if next == &next.to_uppercase() {
            paths += dfs(graph, next, visited, twice);
            continue;
        }

        let next_str_1 = next.to_string();
        let next_str_2 = next.to_string();

        if !visited.contains(&next_str_1) {
            visited.insert(next_str_1);
            paths += dfs(graph, next, visited, twice);
            visited.remove(&next_str_2);

        } else if !twice && next != &"start" {
            paths += dfs(graph, next, visited, true);
        }
    }

    return paths;
}
