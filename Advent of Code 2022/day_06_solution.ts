import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.trim().split("\n");

const unique = (sequence: string) => new Set(sequence).size == sequence.length;
const search = (line: string, size: number): number => unique(line.substring(0, size)) ? size : 1 + search(line.substring(1), size);

const part_1_chars = lines.map(line => search(line, 4));
const part_2_chars = lines.map(line => search(line, 14));

console.log("Answer to Part 1:", part_1_chars);
console.log("Answer to Part 2:", part_2_chars);
