import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.split("\n\n")

const add = (acc: number, cur: number) => acc + cur;
const sub = (lhs: number, rhs: number) => lhs - rhs;

const elves = lines.map(line => line.trim().split("\n").map((cal) => +cal).reduce(add)).sort(sub);

console.log("Answer to Part 1:", elves.slice(-1).reduce(add));
console.log("Answer to Part 2:", elves.slice(-3).reduce(add));
