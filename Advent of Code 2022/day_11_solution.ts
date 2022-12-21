import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const parts = input.trim().split("\n\n");

class Monkey {
    items: Array<bigint>;
    op: (old: bigint) => bigint;
    test: (val: bigint) => number;
    test_div: bigint;
    inspected: number;

    constructor(items: Array<bigint>, op: (old: bigint) => bigint, test: (val: bigint) => number, test_div: bigint) {
        this.items = items;
        this.op = op;
        this.test = test;
        this.test_div = test_div;
        this.inspected = 0;
    }

    inspect(divide: boolean, modulo: bigint) {
        const item = this.items.shift()!;
        const worry = (divide ? this.op(item) / BigInt(3) : this.op(item)) % modulo;

        const index = this.test(worry);
        monkeys[index].items.push(worry);
        this.inspected += 1;
    }
};

const make_monkey = (lines_str: string) => {
    const lines_arr = lines_str.split("\n");
    const op_arr = lines_arr[2].split(" ");

    const items = lines_arr[1].substring(17).split(", ").map(x => BigInt(+x));

    let op: (old: bigint) => bigint;
    if (op_arr[op_arr.length - 2] == "+") {
        const last = op_arr[op_arr.length - 1];
        if (last === "old") {
            op = (old: bigint) => old + old;
        } else {
            op = (old: bigint) => old + BigInt(+last); 
        }
    } else {
        const last = op_arr[op_arr.length - 1];
        if (last === "old") {
            op = (old: bigint) => old * old;
        } else {
            op = (old: bigint) => old * BigInt(+last); 
        }
    }

    const div_arr = lines_arr[3].split(" ");
    const test_div = BigInt(+div_arr[div_arr.length - 1]);

    const test = (val: bigint) => {        
        if (val % test_div == BigInt(0)) {
            const line_arr = lines_arr[lines_arr.length - 2].split(" ");
            return +line_arr[line_arr.length - 1];
        } else {
            const line_arr = lines_arr[lines_arr.length - 1].split(" ");
            return +line_arr[line_arr.length - 1];
        }
    }

    return new Monkey(items, op, test, test_div);
};

const monkeys_1 = parts.map(make_monkey);
const monkeys_2 = parts.map(make_monkey);

const simulate_round = (divide: boolean, modulo: bigint, index: number) => {
    if (index < monkeys.length) {
        if (monkeys[index].items.length > 0) {
            monkeys[index].inspect(divide, modulo);
            simulate_round(divide, modulo, index);
        } else {
            simulate_round(divide, modulo, index + 1);
        }
    }
};

const sub = (lhs: number, rhs: number) => lhs - rhs;
const mul_n = (lhs: bigint, rhs: number) => lhs * BigInt(rhs);
const mul_b = (lhs: bigint, rhs: bigint) => (lhs * rhs) as bigint;

const modulo = monkeys_1.map(m => m.test_div).reduce(mul_b, BigInt(1));

let monkeys = monkeys_1;
Array.from(Array(20).keys()).forEach(_ => simulate_round(true, modulo, 0));
const part_1_ans = monkeys.map(m => m.inspected).sort(sub).slice(monkeys.length - 2).reduce(mul_n, BigInt(1));

monkeys = monkeys_2;
Array.from(Array(10000).keys()).forEach(_ => simulate_round(false, modulo, 0));
const part_2_ans = monkeys.map(m => m.inspected).sort(sub).slice(monkeys.length - 2).reduce(mul_n, BigInt(1));

console.log("Answer to Part 1:", part_1_ans);
console.log("Answer to Part 2:", part_2_ans);
