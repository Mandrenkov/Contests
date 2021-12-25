use std::cmp::{max, min};

/// Solves the Day 05 Part 1 puzzle with respect to the given input.
pub fn part_1(input: String) {
    let mut vents: Vec<Vent> = Vec::new();

    for line in input.lines() {
        let mut segments = line.split(" -> ");

        let mut beg = segments.next().unwrap().split(",");
        let x1 = beg.next().unwrap().parse::<usize>().unwrap();
        let y1 = beg.next().unwrap().parse::<usize>().unwrap();

        let mut end = segments.next().unwrap().split(",");
        let x2 = end.next().unwrap().parse::<usize>().unwrap();
        let y2 = end.next().unwrap().parse::<usize>().unwrap();

        let vent = Vent {
            x1: x1,
            y1: y1,
            x2: x2,
            y2: y2,
        };
        vents.push(vent);
    }

    let w = vents
        .iter()
        .map(|vent| max(vent.x1, vent.x2))
        .max()
        .unwrap()
        + 1;
    let h = vents
        .iter()
        .map(|vent| max(vent.y1, vent.y2))
        .max()
        .unwrap()
        + 1;
    let mut field = vec![vec![0i32; w]; h];

    plot_spots(&mut field, &vents);
    plot_flats(&mut field, &vents);
    plot_verts(&mut field, &vents);

    let points = field.into_iter().flatten().filter(|&v| v > 1).count();
    println!("{}", points);
}

/// Solves the Day 05 Part 2 puzzle with respect to the given input.
pub fn part_2(input: String) {
    let mut vents: Vec<Vent> = Vec::new();

    for line in input.lines() {
        let mut segments = line.split(" -> ");

        let mut beg = segments.next().unwrap().split(",");
        let x1 = beg.next().unwrap().parse::<usize>().unwrap();
        let y1 = beg.next().unwrap().parse::<usize>().unwrap();

        let mut end = segments.next().unwrap().split(",");
        let x2 = end.next().unwrap().parse::<usize>().unwrap();
        let y2 = end.next().unwrap().parse::<usize>().unwrap();

        let vent = Vent {
            x1: x1,
            y1: y1,
            x2: x2,
            y2: y2,
        };
        vents.push(vent);
    }

    let w = vents
        .iter()
        .map(|vent| max(vent.x1, vent.x2))
        .max()
        .unwrap()
        + 1;
    let h = vents
        .iter()
        .map(|vent| max(vent.y1, vent.y2))
        .max()
        .unwrap()
        + 1;
    let mut field = vec![vec![0i32; w]; h];

    plot_spots(&mut field, &vents);
    plot_flats(&mut field, &vents);
    plot_verts(&mut field, &vents);
    plot_diags(&mut field, &vents);

    let points = field.into_iter().flatten().filter(|&v| v > 1).count();
    println!("{}", points);
}

struct Vent {
    x1: usize,
    y1: usize,
    x2: usize,
    y2: usize,
}

/// Plots all the vents which occupy only a single cell.
fn plot_spots(field: &mut Vec<Vec<i32>>, vents: &Vec<Vent>) {
    let filter = |vent: &&Vent| vent.x1 == vent.x2 && vent.y1 == vent.y2;
    for vent in vents.iter().filter(filter) {
        field[vent.y1][vent.x1] += 1;
    }
}

/// Plots all the vents which form a horizontal line.
fn plot_flats(field: &mut Vec<Vec<i32>>, vents: &Vec<Vent>) {
    let filter = |vent: &&Vent| vent.x1 != vent.x2 && vent.y1 == vent.y2;
    for vent in vents.iter().filter(filter) {
        let y = vent.y1;
        let x1 = min(vent.x1, vent.x2);
        let x2 = max(vent.x1, vent.x2);
        for x in x1..x2 + 1 {
            field[y][x] += 1;
        }
    }
}

/// Plots all the vents which form a vertical line.
fn plot_verts(field: &mut Vec<Vec<i32>>, vents: &Vec<Vent>) {
    let filter = |vent: &&Vent| vent.x1 == vent.x2 && vent.y1 != vent.y2;
    for vent in vents.iter().filter(filter) {
        let x = vent.x1;
        let y1 = min(vent.y1, vent.y2);
        let y2 = max(vent.y1, vent.y2);
        for y in y1..y2 + 1 {
            field[y][x] += 1;
        }
    }
}

/// Plots all the vents which form a diagonal line.
fn plot_diags(field: &mut Vec<Vec<i32>>, vents: &Vec<Vent>) {
    let filter = |vent: &&Vent| vent.x1 != vent.x2 && vent.y1 != vent.y2;
    for vent in vents.iter().filter(filter) {
        let same = (vent.x1 < vent.x2) == (vent.y1 < vent.y2);
        let min_x = min(vent.x1, vent.x2);
        let max_x = max(vent.x1, vent.x2);
        for adx in 0..(max_x as i32 - min_x as i32).abs() + 1 {
            let dx = if vent.x1 < vent.x2 { adx } else { -adx };
            let x = vent.x1 as i32 + dx;
            let y = vent.y1 as i32 + if same { dx } else { -dx };
            field[y as usize][x as usize] += 1;
        }
    }
}
