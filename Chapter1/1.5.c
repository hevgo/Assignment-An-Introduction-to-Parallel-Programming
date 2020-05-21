int divisor = 1;
int n_thread = get_thread_total();
while (divisor < n_thread){
    for (size_t i = 0; i < n_thread, i += 2 * divisor){
        if (i % (2*divisor) == 0){
            if (i + divisor < n_thread){  // adding for this problem
                sum(core i) += sum(core i + divisor);
            }
        }
    }
    divisor *= 2;
}


bit = 0;
int n_thread = get_thread_total();
while ( (1 << bit) < n_thread){
    bitmask = 1 << bit;
    for (size_t i = 0; i < n_thread, i += bitmask){
        if ( (i & bitmask) == 0){
            if (i + bitmask < n_thread){  // adding for this problem
                sum(core i) += sum(core i + bitmask);
            }
            
        }
    }
    bit += 1;
}