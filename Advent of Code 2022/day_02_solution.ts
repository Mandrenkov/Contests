import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.trim().split("\n");

const encoder = new Map<string, number>();

const translator = {A: 0, B: 1, C: 2, X: 0, Y: 1, Z: 2};
for (const [key, value] of Object.entries(translator)) {
    encoder.set(key, value)
}

const sum = (acc: number, cur: number) => acc + cur;

const win = (lhs: number, rhs: number) => (3 + rhs - lhs) % 3 == 1;
const draw = (lhs: number, rhs: number) => lhs === rhs;
const score = (lhs: number, rhs: number) => 6 * +win(lhs, rhs) + 3 * +draw(lhs, rhs) + rhs + 1;

const adapt = (lhs: number, rhs: number) => (lhs + rhs + 2) % 3;;
const parse = (line: string) => [encoder.get(line[0])!, encoder.get(line[2])!];

const part_1_score = lines.map(parse).map(([lhs, rhs]) => score(lhs, rhs)).reduce(sum);
const part_2_score = lines.map(parse).map(([lhs, rhs]) => score(lhs, adapt(lhs, rhs))).reduce(sum);

console.log("Answer to Part 1:", part_1_score);
console.log("Answer to Part 2:", part_2_score);
