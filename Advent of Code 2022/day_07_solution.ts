import { readFileSync } from "fs";

const path: string = process.argv.slice(2, 3)[0];
const input = readFileSync(path, "utf8");
const lines = input.trim().split("\n").map(x => x.trim());

class File {
    name: string;
    size: number;

    constructor(name: string, size: number) {
        this.name = name;
        this.size = size;
      }
}

class Directory {
    files: Array<File>;
    parent: Directory | undefined;
    subdirs: Array<Directory>;
    size: number;

    constructor(parent: Directory | undefined = undefined) {
      this.files = [];
      this.parent = parent;
      this.subdirs = [];
      this.size = 0;
    }
}

const parse_ls = (dir: Directory, index: number) => parse(dir, index + 1);
const parse_cd = (dir: Directory, index: number) => {
    if (lines[index].split(" ")[2] === "..") {
        parse(dir.parent as Directory, index + 1);
    } else {
        const subdir = new Directory(dir);
        dir.subdirs.push(subdir);
        parse(subdir, index + 1);
    }
}
const parse_out = (dir: Directory, index: number) => {
    const [one, two] = lines[index].split(" ") as [string, string];
    if (one != "dir") {
        const file = new File(two, +one);
        dir.files.push(file);
    }
    parse_out(dir, index + 1);
}
const parse = (dir: Directory, index: number) => {
    if (index == lines.length) {
        return;
    } else if (lines[index] == "$ ls") {
        parse_ls(dir, index);
    } else if (lines[index][0] === "$") {
        parse_cd(dir, index);
    } else {
        parse_out(dir, index);
    }
}

const sum = (acc: number, cur: number) => acc + cur;
const min = (acc: number, cur: number) => Math.min(acc, cur);

const size_of_files = (dir: Directory) => dir.files.map(file => file.size).reduce(sum, 0);
const size_of_subdirs = (dir: Directory) => dir.subdirs.map(subdir => size_of_dir(subdir)).reduce(sum, 0);
const size_of_dir = (dir: Directory) => {dir.size = size_of_files(dir) + size_of_subdirs(dir); return dir.size; }
const sizes = (dir: Directory): Array<number> => [dir.size].concat(...dir.subdirs.map(sizes));

const build = () => {const root = new Directory(); parse(root, 1); size_of_dir(root); return root; }

const root = build();
const part_1_ans = sizes(root).filter(size => size <= 100000).reduce(sum);
const part_2_ans = sizes(root).filter(size => size >= root.size - 40000000).reduce(min);

console.log("Answer to Part 1:", part_1_ans);
console.log("Answer to Part 2:", part_2_ans);