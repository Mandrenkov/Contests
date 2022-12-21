import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.trim().split("\n");

const vis_up = (val: number, r: number, c: number): boolean => r < 0 || (+lines[r][c] < val && vis_up(val, r - 1, c));
const vis_dn = (val: number, r: number, c: number): boolean => r >= lines.length || (+lines[r][c] < val && vis_dn(val, r + 1, c));
const vis_lt = (val: number, r: number, c: number): boolean => c < 0 || (+lines[r][c] < val && vis_lt(val, r, c - 1));
const vis_rt = (val: number, r: number, c: number): boolean => c >= lines[r].length || (+lines[r][c] < val && vis_rt(val, r, c + 1));
const vis = (r: number, c: number) => +(
    vis_up(+lines[r][c], r - 1, c) ||
    vis_dn(+lines[r][c], r + 1, c) ||
    vis_lt(+lines[r][c], r, c - 1) ||
    vis_rt(+lines[r][c], r, c + 1)
);

const scn_up = (val: number, r: number, c: number): number => r < 0 ? 0 : (+lines[r][c] < val ? 1 + scn_up(val, r - 1, c) : 1);
const scn_dn = (val: number, r: number, c: number): number => r >= lines.length ? 0 : (+lines[r][c] < val ? 1 + scn_dn(val, r + 1, c) : 1);
const scn_lt = (val: number, r: number, c: number): number => c < 0 ? 0 : (+lines[r][c] < val ? 1 + scn_lt(val, r, c - 1) : 1);
const scn_rt = (val: number, r: number, c: number): number => c >= lines[r].length ? 0 : (+lines[r][c] < val ? 1 + scn_rt(val, r, c + 1) : 1);
const scn = (r: number, c: number) =>
    scn_up(+lines[r][c], r - 1, c) *
    scn_dn(+lines[r][c], r + 1, c) *
    scn_lt(+lines[r][c], r, c - 1) *
    scn_rt(+lines[r][c], r, c + 1);

const part_1_loop_1 = (r: number): number => r == lines.length ? 0 : part_1_loop_2(r, 0) + part_1_loop_1(r + 1);
const part_1_loop_2 = (r: number, c: number): number => c == lines[r].length ? 0 : vis(r, c) + part_1_loop_2(r, c + 1);

const part_2_loop_1 = (r: number): number => r == lines.length ? 0 : Math.max(part_2_loop_2(r, 0), part_2_loop_1(r + 1));
const part_2_loop_2 = (r: number, c: number): number => c == lines[r].length ? 0 : Math.max(scn(r, c), part_2_loop_2(r, c + 1));

const part_1_ans = part_1_loop_1(0);
const part_2_ans = part_2_loop_1(0);

console.log("Answer to Part 1:", part_1_ans);
console.log("Answer to Part 2:", part_2_ans);
