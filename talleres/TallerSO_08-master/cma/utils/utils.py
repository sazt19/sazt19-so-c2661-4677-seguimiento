import sys

def print_memory_map(memory_map):
    for memory in memory_map:
        print(f"({memory[0]:#0{8}x}, {memory[1]:#0{8}x})")

def read_reqs_file(reqs_filename):
    result = []
    try:
        with open(reqs_filename, 'r') as reqsfile:
            for line in reqsfile:
                req = int(line.strip(),16)
                result.append(req)
    except FileNotFoundError:
        print(f'File not found {reqs_filename}', file=sys.stderr)
        return None
    else:
        return result

def read_memmap_file(memmap_filename):
    result = []
    try:
        with open(memmap_filename, 'r') as mmfile:
            for line in mmfile:
                elems = line.strip().split()
                result.append((int(elems[0],16), int(elems[1],16)))
    except FileNotFoundError:
        print(f'File not found {memmap_filename}', file=sys.stderr)
        return None
    else:
        return result
