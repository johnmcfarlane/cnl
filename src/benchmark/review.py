#!/usr/bin/env python3

from argparse import ArgumentParser
from io import StringIO
from itertools import chain
from os import chdir, curdir, path
from subprocess import CalledProcessError, check_output, PIPE
from sys import argv, stderr

from report import benchmarks_from_buffer, csv_from_report, table_from_benchmarks


def run_command(dir, command):
    chdir(dir)
    return check_output(command, stderr=PIPE).decode('ascii')

def run_from_repo(args, command):
    return run_command(args.repo, command)

def run_from_build(args, command):
    return run_command(args.build, command)


def run_benchmarks(args):
    # configure
    run_from_build(args, ["cmake", args.repo, "-DCMAKE_BUILD_TYPE=Release", "-DCNL_DEV=ON"])

    # build
    run_from_build(args, ["make", "Benchmark", "-j", str(args.jobs or 1)])

    # benchmark
    benchmark_args = [
        "--benchmark_format=csv",
        "--benchmark_filter="+args.filter
    ]
    buffer = run_from_build(args, ["./Benchmark"] + benchmark_args)

    # clean
    run_from_build(args, ["make", "clean"])

    return buffer

def extract_names(collection):
    return sorted(set(chain.from_iterable(benchmarks for commit, benchmarks in collection)))

def make_row(commit, benchmarks, names):
    return [commit] + [benchmarks.get(name, '-') for name in names]


# get a list of the commit SHAs in ascending chronological order
def get_commits(args):
    watch_files = ["--", "include", "src/benchmarks/*", "CMakeLists.txt", "*.cmake"]
    git_log_args = (["--merges"] if args.merges else []) + (["--no-merges"] if args.no_merges else []) + [
                       "--all",
                       "--date-order",
                       "--reverse",
                       "--format=format:%H",
                       args.range or "--all"
                   ] + (["-{}".format(args.max_commits)] if args.max_commits else []) + ([] if args.merges else watch_files)
    return run_from_repo(args, ["git", "log"] + git_log_args).split('\n')

def benchmark(args, commit, commits):
    # print progress indicator
    stderr.write("\r{}/{} {}".format(commits.index(commit), len(commits), commit))
    stderr.flush()

    # checkout the given commit
    run_from_repo(args, ["git", "checkout", "--force", commit])

    try:
        # run the benchmarks and store results as a stream
        stream = StringIO(run_benchmarks(args))
    except CalledProcessError as e:
        # failure likely means a commit from before the benchmarks existed
        return {}

    # decode stream as CSV table
    benchmarks = benchmarks_from_buffer(stream)

    # refine table
    report = table_from_benchmarks(benchmarks)

    # determine the columns to extract
    title_row = report[0]
    name_index = title_row.index('name')
    cpu_time_index = title_row.index('cpu_time')

    # return list of tuples of benchmark test name to result
    return {cell[name_index]: cell[cpu_time_index] for cell in report[1:]}

def collect(args, commits):
    return [(commit, benchmark(args, commit, commits)) for commit in commits]

def collate(collection):
    names = extract_names(collection)
    return [["commit"] + names] + [make_row(commit, benchmarks, names) for commit, benchmarks in collection if benchmarks]


def main(args):
    commits = get_commits(args)
    collection = collect(args, commits)
    table = collate(collection)
    return csv_from_report(table)


if __name__ == "__main__":
    parser = ArgumentParser(description="For each commit (chronologically) of the repository, run the benchmarking target and chart results as a table of benchmarks by commit.")
    parser.add_argument("repo", help="path to the scaled_integer repository - preferably a different copy than the one from which this script is run")
    parser.add_argument("--build", help="path to cmake build folder; defaults to current folder", default=curdir)
    parser.add_argument("--range", help="range or revisions to visit")
    parser.add_argument("--filter", help="filters benchmarks based on regex pattern", default=".*")
    parser.add_argument("--merges", help="visit only merge commits", type=bool, default=False)
    parser.add_argument("--no-merges", help="skip merge commits", type=bool, default=False)
    parser.add_argument("--max_commits", help="maximum number of commits to test (going back from most recent)", type=int)
    parser.add_argument("-j", "--jobs", help="number of parallel build jobs", type=int, default=1)

    args = parser.parse_args()
    args.repo = path.abspath(args.repo)
    args.build = path.abspath(args.build)

    print(main(args))
