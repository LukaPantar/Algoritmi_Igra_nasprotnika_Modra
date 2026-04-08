
import os
import random
# import np
from typing import Optional
from ortools.linear_solver import pywraplp
import time

TEST_FPATH = "../tests/"
OPTIMAL_SOLUTION_FEXT = "_optimal.out"

def write_input_file(values: list, fname: str):
    full_fpath = os.path.join(
        os.path.abspath(os.path.dirname(__file__)),
        TEST_FPATH,
        fname)

    with open(full_fpath, "w") as f:
        f.write(f"{len(values)}\n")  # write the number of values
        for val in values:
            f.write(f"{val}\n")

def write_output_file(bins: list, fname: str):
    full_fpath = os.path.join(
        os.path.abspath(os.path.dirname(__file__)),
        TEST_FPATH,
        os.path.splitext(fname)[0] + OPTIMAL_SOLUTION_FEXT)

    with open(full_fpath, "w") as f:
        f.write(f"{len(bins)}\n")  # write the number of bins
        for bin in bins:
            for value in bin:
                f.write(f"{value + 1} ")
            f.write("\n")

def gen_uniform(n: int, seed: Optional[int] = None) -> list:
    values = []
    random.seed(seed)
    for _ in range(n):
        # function random() already generates float with 15 decimal places
        val = random.random()
        if val != 0.0:
            values.append(val)  
    return values

def gen_constant(n: int, constant: int) -> list:
    values = [constant for _ in range(n)]
    return values

def gen_linear(n: int, start: float = 0.0, end: float = 1.0) -> list:
    eps = 1e-15
    step = (end - start) / n
    values = [start + eps + i * step for i in range(n)]
    return values

def gen_normal(n: int, mean: float = 0.5, std: float = 0.5, seed: Optional[int] = None) -> list:
    random.seed(seed)
    values = []
    while len(values) < n:
        val = random.gauss(mean, std)
        if 0.0 < val < 1.0:
            values.append(val)
    return values

def gen_exponential(n: int, lambd: float = 2.0, seed: Optional[int] = None) -> list:
    random.seed(seed)
    values = []
    while len(values) < n:
        val = random.expovariate(lambd)
        if 0.0 < val < 1.0:
            values.append(val)
    return values

def optimal_solver(values: list):
    solver = pywraplp.Solver.CreateSolver("SCIP_MIXED_INTEGER_PROGRAMMING")
    if not solver:
        return
    
    n = len(values)
    max_bins = n
    bin_size = 1.0
    epsilon = 1e-15

    # Decision variables
    # x - i represents the value that is put into j-th bin
    x = {(i, j): solver.BoolVar(f"x_{i}_{j}")
         for i in range(n) for j in range(max_bins)}

    # y - j-th bin will be used in the solution (before checking if it is full)
    y = {j: solver.BoolVar(f"y_{j}") for j in range(max_bins)}

    # Each value used at most once, therefore the sum of each row is 1 (if used) or 0 (not used at all)
    for i in range(n):
        solver.Add(sum(x[i, j] for j in range(max_bins)) <= 1)

    # Bin covering constraint with tolerance (so 0.99...99 also counts as filled bin)
    for j in range(max_bins):
        solver.Add(
            sum(values[i] * x[i, j] for i in range(n))
            >= bin_size * y[j] - epsilon
        )

    # Maximize covered bins
    # the biggest sum means the most filled bins
    solver.Maximize(solver.Sum(y[j] for j in range(max_bins)))

    status = solver.Solve()

    if status != pywraplp.Solver.OPTIMAL:
        print("No optimal solution found.")
        return []

    bins = []
    for j in range(max_bins):
        # the optimal solution has value > 0.5 (true)
        if y[j].solution_value() > 0.5:
            bin_items = [
                i
                for i in range(n)
                if x[i, j].solution_value() > 0.5
            ]
            bins.append(bin_items)

    return bins

def wrapper_gen_solve(fname: str, values: list, optimal_solve: bool):    
    write_input_file(values, fname)

    if optimal_solve:
        start_solve_optimal = time.time()
        optimal_bins = optimal_solver(values)
        stop_solve_optimal = time.time()
    
        write_output_file(optimal_bins, fname)
        print(f"Time for optimal solution for {fname}: {stop_solve_optimal - start_solve_optimal} s")

if __name__=="__main__":

    N = 30
    OPTIMAL_SOLVE = True

    wrapper_gen_solve("uniform.txt", gen_uniform(N), OPTIMAL_SOLVE)
    wrapper_gen_solve("constant.txt", gen_constant(N, 0.11), OPTIMAL_SOLVE)
    wrapper_gen_solve("linear.txt", gen_linear(N), OPTIMAL_SOLVE)
    wrapper_gen_solve("normal.txt", gen_normal(N), OPTIMAL_SOLVE)
    wrapper_gen_solve("exponential.txt", gen_exponential(N), OPTIMAL_SOLVE)
