import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.trim().split("\n");

const step_1 = new Map<string, number>();
const step_2 = new Map<string, number>();
const bfsq_1 = new Array<string>();
const bfsq_2 = new Array<string>();

const find_start = (row: number) : [number, number] => {
    if (lines[row].search("S") == -1) {
        return find_start(row + 1);
    } else {
        return [row, lines[row].search("S")];
    }
}

const find_a_2 = (row: number, col: number) => {
    if (col < lines[0].length) {
        if (lines[row][col] === "a" || lines[row][col] === "S") {
            const cell = JSON.stringify([row, col]);
            bfsq_2.push(cell);
            step_2.set(cell, 0);
        }
        find_a_2(row, col + 1);
    }
};
const find_a = (row: number) => {
    if (row < lines.length) {
        find_a_2(row, 0);
        find_a(row + 1)
    }
};
find_a(0);

const start = JSON.stringify(find_start(0));

bfsq_1.push(start);
step_1.set(start, 0);

const get_code = (r: number, c: number) => {
    if (lines[r][c] == "S") {
        return "a".charCodeAt(0);
    } else if (lines[r][c] == "E") {
        return "z".charCodeAt(0);
    } else {
        return lines[r].charCodeAt(c);
    }
}
const can_move = (r1: number, c1: number, r2: number, c2: number) => get_code(r2, c2) <= get_code(r1, c1) + 1;

const bfs = () : number | undefined => {
    const top = JSON.parse(bfsq.shift()!);
    const [row, col] = top;

    if (lines[row][col] == "E") {
        bfsq.unshift(JSON.stringify(top));
        return step.get(JSON.stringify(top))!;
    }

    const up = JSON.stringify([row - 1, col]);
    const dn = JSON.stringify([row + 1, col]);
    const lt = JSON.stringify([row, col - 1]);
    const rt = JSON.stringify([row, col + 1]);

    const cur_step = step.get(JSON.stringify(top))!;

    if (row > 0 && !step.has(up) && can_move(row, col, row - 1, col)) {
        step.set(up, cur_step + 1);
        bfsq.push(up);
    }
    if (row < lines.length - 1 && !step.has(dn) && can_move(row, col, row + 1, col)) {
        step.set(dn, cur_step + 1);
        bfsq.push(dn);
    }
    if (col > 0 && !step.has(lt) && can_move(row, col, row, col - 1)) {
        step.set(lt, cur_step + 1);
        bfsq.push(lt);
    }
    if (col < lines[0].length - 1 && !step.has(rt) && can_move(row, col, row, col + 1)) {
        step.set(rt, cur_step + 1);
        bfsq.push(rt);
    }

    return undefined;
}

let bfsq = bfsq_1;
let step = step_1;
const part_1_ans = Array.from(Array(lines.length * lines[0].length).keys()).map(_ => bfs()).pop();

bfsq = bfsq_2;
step = step_2;
const part_2_ans = Array.from(Array(lines.length * lines[0].length).keys()).map(_ => bfs()).pop();

console.log("Answer to Part 1:", part_1_ans);
console.log("Answer to Part 2:", part_2_ans);