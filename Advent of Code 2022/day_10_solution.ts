import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.trim().split("\n");

let cycle = 0;
let register = 1;

let part_1_ans = 0;
let part_2_ans: Array<Array<string>> = [];

const part_2_fill_1 = (row: number) => { if (row < 6) { part_2_ans.push([]); part_2_fill_2(row, 0); part_2_fill_1(row + 1) } };
const part_2_fill_2 = (row: number, col: number) => { if (col < 40) { part_2_ans[row].push("."); part_2_fill_2(row, col + 1) } };
part_2_fill_1(0);

const draw = () => {
    const row = Math.floor(cycle / 40);
    const col = cycle % 40;
    if (Math.abs(col - register) <= 1) {
        part_2_ans[row][col] = "#";
    }
}

const wait = () => {
    draw();
    cycle += 1;
    if ((cycle - 20) % 40 == 0) {
        part_1_ans += cycle * register;
    }
}

const noop = () => wait();
const addx = (delta: number) => { wait(); wait(); register += delta; };
const exec = (line: string) => line[0] == "n" ? noop() : addx(+line.split(" ")[1]);

lines.map(exec);

console.log("Answer to Part 1:", part_1_ans);
console.log("Answer to Part 2:", part_2_ans.map(row => row.flat().join("")));