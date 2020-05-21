bit = 0;
int n_thread = get_thread_total();

while ( (1 << bit) < n_thread){
    bitmask = 1 << bit;
    for (size_t i = 0; i < n_thread, i += bitmask){
        if ( (i & bitmask) == 0){
            sum(core i) += sum(core i + bitmask);
        }
        // else{
        //     send sum to core i - bitmask;
        // }
    }
    bit += 1;
}