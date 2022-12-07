import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.split("\n");

const [num_stacks, num_lines] = lines[0].split(" ").map(x => +x) as [number, number];

// Parse the crates.

const range = (i: number) => Array.from(Array(i).keys());
const index = (i: number, line: string) => [i, line[1 + 4 * i]] as [number, string];
const empty = ([_, symbol]: [number, string]) => symbol != " ";
const stack_one = ([i, symbol]: [number, string]) => stacks[i as number].push(symbol)
const stack_all = (line: string) => range(num_stacks).map(i => index(i, line)).filter(empty).forEach(stack_one);

const stacks = new Array(num_stacks).fill(undefined).map(_ => new Array());
lines.slice(1, num_lines).reverse().forEach(stack_all);

const stacks_1 = JSON.parse(JSON.stringify(stacks)) as Array<Array<string>>;
const stacks_2 = JSON.parse(JSON.stringify(stacks)) as Array<Array<string>>;

// Move the creates.

const move_queue = ([times, src, dst]: [number, number, number]) => stacks_2[dst].push(...stacks_2[src].splice(stacks_2[src].length - times).reverse());
const move_stack = ([times, src, dst]: [number, number, number]) => stacks_1[dst].push(...stacks_1[src].splice(stacks_1[src].length - times));
const align = ([times, src, dst]: [number, number, number]) => [times, src - 1, dst - 1] as [number, number, number];
const parse = (parts: Array<string>) => [parts[1], parts[3], parts[5]].map(x => +x) as [number, number, number];
const split = (line: string) => line.split(" ");

lines.slice(num_lines + 2).forEach(line => move_queue(align(parse(split(line)))));
lines.slice(num_lines + 2).forEach(line => move_stack(align(parse(split(line)))));

// Read the top crate of each stack.

const sum = (acc: string, cur: string) => acc + cur;

const part_1_ans = stacks_1.map(stack => stack[stack.length - 1]).reduce(sum);
const part_2_ans = stacks_2.map(stack => stack[stack.length - 1]).reduce(sum);

console.log("Answer to Part 1:", part_1_ans);
console.log("Answer to Part 2:", part_2_ans);
