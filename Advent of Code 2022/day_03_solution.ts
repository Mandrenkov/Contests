import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.trim().split("\n");

const groups = [];
for (let i = 0; i < lines.length; i += 3) {
    groups.push([lines[i], lines[i + 1], lines[i + 2]]);
}

const sum = (acc: number, cur: number) => acc + cur;

const lower = (item: string) => item.charCodeAt(0) - "a".charCodeAt(0) + 1;
const upper = (item: string) => item.charCodeAt(0) - "A".charCodeAt(0) + 27;
const prioritize = (item: string) => item == item.toLowerCase() ? lower(item) : upper(item);

const common_1 = (lhs: Set<string>, rhs: Set<string>) => Array.from(lhs).filter(item => rhs.has(item)).at(0)!;
const common_2 = (lhs: Set<string>, rhs: Set<string>) => new Set(Array.from(lhs).filter(item => rhs.has(item)));
const common_3 = (top: Set<string>, mid: Set<string>, bot: Set<string>) => common_2(common_2(top, mid), bot).values().next().value;

const search_1 = (lhs: string, rhs: string) => common_1(new Set(lhs), new Set(rhs));
const search_3 = (top: string, mid: string, bot: string) => common_3(new Set(top), new Set(mid), new Set(bot));

const split_1 = (line: string) => [line.slice(0, line.length / 2), line.slice(line.length / 2)];
const split_3 = (group: Array<string>) => [group[0], group[1], group[2]];

const part_1_sum = lines.map(split_1).map(([lhs, rhs]) => search_1(lhs, rhs)).map(prioritize).reduce(sum);
const part_2_sum = groups.map(split_3).map(([top, mid, bot]) => search_3(top, mid, bot)).map(prioritize).reduce(sum);

console.log("Answer to Part 1:", part_1_sum);
console.log("Answer to Part 2:", part_2_sum);
