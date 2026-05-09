#!/usr/bin/python3
import click
from cont_mem_algos import first_fit, best_fit, worst_fit
from utils.utils import print_memory_map,read_reqs_file,read_memmap_file

def cmas(algo_str):
    if algo_str == 'all':
        return [
            {"name" : "First fit",
             "function" : first_fit },
            {"name" : "Best fit",
             "function" : best_fit },
            {"name" : "Worst fit",
             "function" : worst_fit},
        ]
    elif algo_str == 'first':
        return [
            {"name" : "First fit",
             "function" : first_fit },
        ]
    elif algo_str == 'best':
        return [
            {"name" : "Best fit",
             "function" : best_fit },
        ]
    elif algo_str == 'worst':
        return [
            {"name" : "Worst fit",
             "function" : worst_fit},
        ]
    else:
        return None

@click.command()
@click.option('--memmap', help='file with the memory description')
@click.option('--reqs', help='requirement file')
@click.option('--function', default='all', help='Algorithm to Execute: first, best, worst, all')
@click.option('--pos', default=0, help='initial position')
def process(memmap, reqs, function, pos):
    memory = read_memmap_file(memmap)
    requirements = read_reqs_file(reqs)
    cont_mem_algo = cmas(function)
    if memory == None or requirements == None or cont_mem_algo == None:
        return

    first_pos = pos
    
    work_memory = memory[:]

    for cmae in cont_mem_algo:

        work_memory = memory[:]
        index = first_pos
        print(cmae["name"])
        print_memory_map(work_memory)
        
        for req in requirements:
            search = cmae["function"](work_memory, req, index)
            
            if search == None:
                print(f"Not found: {req:#0{8}x}")
            else:
                work_memory,base,limit,index = search
                print(f"Assigned to the requirement:{req:#0{8}x} [base,limit]:[{base:#0{8}x},{limit:#0{8}x}]")
                print(f"Index: {index}")
                print_memory_map(work_memory)

if __name__ == '__main__':
    process()
