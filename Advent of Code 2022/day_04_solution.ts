import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.trim().split("\n");

const overlaps = ([l1, r1]: [number, number], [l2, r2]: [number, number]) => l1 <= l2 && r1 >= l2 || l2 <= l1 && r2 >= l1;
const contains = ([l1, r1]: [number, number], [l2, r2]: [number, number]) => l1 <= l2 && r2 <= r1 || l2 <= l1 && r1 <= r2;

const parse = (asgn: string) => asgn.split("-").map(x => +x) as [number, number];
const split = (line: string) => line.split(",").map(parse);

const part_1_pairs = lines.map(split).filter(([one, two]) => contains(one, two)).length
const part_2_pairs = lines.map(split).filter(([one, two]) => overlaps(one, two)).length

console.log("Answer to Part 1:", part_1_pairs);
console.log("Answer to Part 2:", part_2_pairs);
