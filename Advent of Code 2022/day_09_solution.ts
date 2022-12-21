import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.trim().split("\n");

const solve = (num_tails: number) => {
    const head = {x: 0, y: 0};
    const tails: Array<typeof head> = [];

    const fill_tails = (index: number) => {
        if (index < num_tails) {
            tails.push({x: 0, y: 0});
            fill_tails(index + 1);
        }
    };
    fill_tails(0);

    const visited = new Set<string>();
    visited.add(JSON.stringify(tails[0]));

    const move_tails = (prev: typeof head, index: number) => {
        const tail = tails[index] as typeof head;
        const dx = prev.x - tail.x;
        const dy = prev.y - tail.y;

        if (Math.abs(dx) > 1 || Math.abs(dy) > 1) {
            if (dx != 0) tail.x += dx / Math.abs(dx);
            if (dy != 0) tail.y += dy / Math.abs(dy);
            if (index == 0) {
                visited.add(JSON.stringify(tail));
            } else {
                move_tails(tail, index - 1);
            }
        }
    }

    const up = (times: number) => { if (times > 0) { ++head.y; move_tails(head, num_tails - 1); up(times - 1); } };
    const dn = (times: number) => { if (times > 0) { --head.y; move_tails(head, num_tails - 1); dn(times - 1); } };
    const lt = (times: number) => { if (times > 0) { --head.x; move_tails(head, num_tails - 1); lt(times - 1); } };
    const rt = (times: number) => { if (times > 0) { ++head.x; move_tails(head, num_tails - 1); rt(times - 1); } };
    const move_head = {U: up, D: dn, L: lt, R: rt};

    const simulate = ([dir, reps]: [string, string]) => move_head[dir as keyof typeof move_head](+reps);
    lines.forEach(line => simulate(line.split(" ") as [string, string]))

    return visited.size;
}

const part_1_ans = solve(1);
const part_2_ans = solve(9);

console.log("Answer to Part 1:", part_1_ans);
console.log("Answer to Part 2:", part_2_ans);
