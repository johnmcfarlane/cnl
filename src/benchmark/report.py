#!/usr/bin/env python

import csv
import itertools
import sys

help_text = "please provide CSV-formatted google/benchmark output"

def sum_from_rows(rows):
    return ["total"] + [str(sum(float(cell) for cell in column)) for
    column in itertools.islice(zip(*rows), 1, None)]

def report_from_table(table):
    return table + [sum_from_rows(table[1:])]

def table_from_benchmarks(table):
    def filter_row(row):
        return [str(cell[0]) for cell in zip(row, table[0]) if cell[1]
        in ("name","iterations","real_time","cpu_time")]

    return [filter_row(row) for row in table]

def report_from_benchmarks(table):
    return report_from_table(table_from_benchmarks(table))

def benchmarks_from_buffer(buffer):
    return list(csv.reader(buffer, delimiter=',', quotechar='"'))

def report_from_file(filename):
    with open(filename) as file:
        return report_from_benchmarks(
            benchmarks_from_buffer(file))

def csv_from_report(table):
    return '\n'.join([','.join(row) for row in table])

if __name__ == "__main__":
    print(csv_from_report(report_from_file(sys.argv[1]))
        if len(sys.argv) == 2 else help_text)
