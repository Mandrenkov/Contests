use core::cmp::max;

/// Solves the Day 18 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    // Nodes are of the form (v, p, l, r).
    let mut lhs_tree = Vec::<(usize, usize, usize, usize)>::new();
    lhs_tree.push((0, 0, 0, 0));

    let mut first = true;

    for line in input.lines() {
        let chars = line.chars().collect();
        let rhs_tree = parse_tree(&chars, 0, chars.len() - 1);
        let raw_tree = merge(&lhs_tree, &rhs_tree);
        let rdc_tree = if first { raw_tree } else { reduce(&raw_tree) };
        lhs_tree = rdc_tree;

        first = false;
    }

    let mag = magnitude(&lhs_tree, 1);
    println!("{}", mag);
}

/// Solves the Day 18 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    // Nodes are of the form (v, p, l, r).
    let mut trees: Vec<Vec<(usize, usize, usize, usize)>> = vec![];

    for line in input.lines() {
        let chars = line.chars().collect();
        let tree = parse_tree(&chars, 0, chars.len() - 1);
        trees.push(tree);
    }

    let mut best = 0;
    for i in 0..trees.len() {
        for j in 0..trees.len() {
            if i != j {
                let raw_tree = merge(&trees[i], &trees[j]);
                let rdc_tree = reduce(&raw_tree);
                let mag = magnitude(&rdc_tree, 1);
                best = max(best, mag);
            }
        }
    }

    println!("{}", best);
}

/// Converts the subtree rooted at the given node into a string.
#[allow(dead_code)]
fn tree_to_string(tree: &Vec<(usize, usize, usize, usize)>, node: usize) -> String {
    if node == tree.len() {
        return String::new();
    }

    let (v, _, l, r) = tree[node];
    if l == 0 {
        return format!("{}", v);
    } else {
        return format!("[{},{}]", tree_to_string(tree, l), tree_to_string(tree, r));
    }
}

/// Converts the characters of a line into a tree.
fn parse_tree(chars: &Vec<char>, beg: usize, end: usize) -> Vec<(usize, usize, usize, usize)> {
    if beg == end {
        let v = chars[beg].to_digit(10).unwrap() as usize;
        return vec![(0, 0, 0, 0), (v, 0, 0, 0)];
    }

    let mut mid = beg + 1;
    let mut lvl = 0;

    while lvl > 0 || chars[mid] != ',' {
        if chars[mid] == '[' {
            lvl += 1;
        } else if chars[mid] == ']' {
            lvl -= 1;
        }
        mid += 1;
    }

    let lhs = parse_tree(chars, beg + 1, mid - 1);
    let rhs = parse_tree(chars, mid + 1, end - 1);
    return merge(&lhs, &rhs);
}

/// Combines two trees into a single tree (using a single parent node).
fn merge(
    lhs: &Vec<(usize, usize, usize, usize)>,
    rhs: &Vec<(usize, usize, usize, usize)>,
) -> Vec<(usize, usize, usize, usize)> {
    if lhs.len() < 2 {
        return rhs.clone();
    }
    if rhs.len() < 2 {
        return lhs.clone();
    }

    let lhs_offset = 1;
    let rhs_offset = lhs_offset + lhs.len() - 1;

    let mut tree = Vec::<(usize, usize, usize, usize)>::new();
    tree.push((0, 0, 0, 0));
    tree.push((0, 0, lhs_offset + 1, rhs_offset + 1));

    for (v, p, l, r) in lhs.iter().skip(1) {
        let new_p = if *p == 0 { *p } else { p + lhs_offset };
        let new_l = if *l == 0 { *l } else { l + lhs_offset };
        let new_r = if *r == 0 { *r } else { r + lhs_offset };
        tree.push((*v, new_p, new_l, new_r));
    }

    for (v, p, l, r) in rhs.iter().skip(1) {
        let new_p = if *p == 0 { *p } else { p + rhs_offset };
        let new_l = if *l == 0 { *l } else { l + rhs_offset };
        let new_r = if *r == 0 { *r } else { r + rhs_offset };
        tree.push((*v, new_p, new_l, new_r));
    }

    tree[lhs_offset + 1].1 = 1;
    tree[rhs_offset + 1].1 = 1;

    return tree;
}

/// Reduces a tree by exploding and splitting nodes.
fn reduce(tree: &Vec<(usize, usize, usize, usize)>) -> Vec<(usize, usize, usize, usize)> {
    let mut rdc_tree = tree.clone();

    loop {
        let (new_tree, act) = explode(&rdc_tree, 1, 0);
        rdc_tree = new_tree;
        if act {
            continue;
        }

        let (new_tree, act) = split(&rdc_tree, 1);
        rdc_tree = new_tree;
        if !act {
            break;
        }
    }

    return rdc_tree;
}

/// Searches for a node to explode in the subtree rooted at the given node.
fn explode(
    tree: &Vec<(usize, usize, usize, usize)>,
    node: usize,
    depth: usize,
) -> (Vec<(usize, usize, usize, usize)>, bool) {
    if node == 0 {
        return (tree.clone(), false);
    }

    let (_, p, l, r) = tree[node];
    if depth >= 4 && l > 0 && r > 0 && tree[l].2 == 0 && tree[r].2 == 0 {
        let mut new_tree = tree.clone();

        // Find the next adjacent leaf node to the left.
        let mut pl1 = p;
        let mut pl2 = node;
        while pl1 > 0 && tree[pl1].2 == pl2 {
            pl2 = pl1;
            pl1 = tree[pl1].1;
        }

        if pl1 > 0 {
            let mut child = tree[pl1].2;
            while tree[child].3 > 0 {
                child = tree[child].3;
            }
            new_tree[child].0 += tree[l].0;
        }

        // Find the next adjacent leaf node to the right.
        let mut pr1 = p;
        let mut pr2 = node;
        while pr1 > 0 && tree[pr1].3 == pr2 {
            pr2 = pr1;
            pr1 = tree[pr1].1;
        }

        if pr1 > 0 {
            let mut child = tree[pr1].3;
            while tree[child].2 > 0 {
                child = tree[child].2;
            }

            new_tree[child].0 += tree[r].0;
        }

        new_tree[node] = (0, p, 0, 0);

        // Remove the children of this node from the tree.
        for i in 1..new_tree.len() {
            new_tree[i].1 -= (new_tree[i].1 > l) as usize + (new_tree[i].1 > r) as usize;
            new_tree[i].2 -= (new_tree[i].2 > l) as usize + (new_tree[i].2 > r) as usize;
            new_tree[i].3 -= (new_tree[i].3 > l) as usize + (new_tree[i].3 > r) as usize;
        }

        let mut w = 1;
        for i in 1..new_tree.len() {
            if i != l && i != r {
                new_tree[w] = new_tree[i];
                w += 1;
            }
        }
        new_tree.pop();
        new_tree.pop();

        return (new_tree, true);
    }

    let (new_tree, act) = explode(tree, l, depth + 1);
    if act {
        return (new_tree, true);
    }

    return explode(tree, r, depth + 1);
}

/// Searches for a node to split in the subtree rooted at the given node.
fn split(
    tree: &Vec<(usize, usize, usize, usize)>,
    node: usize,
) -> (Vec<(usize, usize, usize, usize)>, bool) {
    if node == 0 {
        return (tree.clone(), false);
    }

    let (v, _, l, r) = tree[node];
    if v > 9 {
        let mut new_tree = tree.clone();
        let vl = v / 2;
        let vr = v - vl;
        new_tree.push((vl, node, 0, 0));
        new_tree.push((vr, node, 0, 0));
        new_tree[node].0 = 0;
        new_tree[node].2 = new_tree.len() - 2;
        new_tree[node].3 = new_tree.len() - 1;
        return (new_tree, true);
    }

    let (new_tree, act) = split(tree, l);
    if act {
        return (new_tree, true);
    }

    return split(tree, r);
}

/// Returns the magnitude of the subtree rooted at the given node.
fn magnitude(tree: &Vec<(usize, usize, usize, usize)>, node: usize) -> usize {
    let (v, _, l, r) = tree[node];
    if l == 0 {
        v
    } else {
        3 * magnitude(tree, l) + 2 * magnitude(tree, r)
    }
}
