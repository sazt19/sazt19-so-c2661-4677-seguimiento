def first_fit(mem_avail, req_size, index):
    searching = True
    found = False
    i = index
    
    if len(mem_avail) == 0 or index < 0 or index >= len(mem_avail):
        searching = False
        
    while searching:
        if mem_avail[i][1] >= req_size:
            found = True
            break
        i = (i + 1) % len(mem_avail)
        if i == index:
            searching = False

    if found:
        base = mem_avail[i][0]
        limit = req_size
        if mem_avail[i][1] == req_size:
            del mem_avail[i]
            i = i % len(mem_avail)
        else:
            mem_pos = mem_avail[i]
            mem_avail[i] = (mem_pos[0] + req_size, mem_pos[1] - req_size) 
        return mem_avail, base, limit, i

    return None

def best_fit(mem_avail, req_size, index):
    searching = True
    found = False
    i = index
    min_val = pow(2,32) - 1 
    min_idx = i
    
    if len(mem_avail) == 0 or index < 0 or index >= len(mem_avail):
        searching = False

    while searching:
        if mem_avail[i][1] <= min_val and mem_avail[i][1] >= req_size:
            min_val = mem_avail[i][1]
            min_idx = i
            found = True
        i = (i + 1) % len(mem_avail)
        if i == index:
            searching = False

    if found:
        base = mem_avail[min_idx][0]
        limit = req_size
        if mem_avail[min_idx][1] == req_size:
            del mem_avail[i]
            min_idx = min_idx % len(mem_avail)
        else:
            mem_pos = mem_avail[min_idx]
            mem_avail[min_idx] = (mem_pos[0] + req_size, mem_pos[1] - req_size) 
        return mem_avail, base, limit, min_idx

    return None

def worst_fit(mem_avail, req_size, index):
    searching = True
    found = False
    i = index
    max_val = 0
    min_idx = i
    
    if len(mem_avail) == 0 or index < 0 or index >= len(mem_avail):
        searching = False
        
    while searching:
        if mem_avail[i][1] > max_val and mem_avail[i][1] >= req_size:
            max_val = mem_avail[i][1]
            max_idx = i
            found = True
        i = (i + 1) % len(mem_avail)
        if i == index:
            searching = False

    if found:
        base = mem_avail[max_idx][0]
        limit = req_size
        if mem_avail[max_idx][1] == req_size:
            del mem_avail[i]
            max_idx = max_idx % len(mem_avail)
        else:
            mem_pos = mem_avail[max_idx]
            mem_avail[max_idx] = (mem_pos[0] + req_size, mem_pos[1] - req_size) 
        return mem_avail, base, limit, max_idx

    return None
