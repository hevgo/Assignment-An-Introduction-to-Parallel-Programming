int divisor = 1;
int n_thread = get_thread_total();
while (divisor < n_thread){
    for (size_t i = 0; i < n_thread, i += 2 * divisor){
        if (i % (2*divisor) == 0){
            sum(core i) += sum(core i + divisor);
        }
        // else{
        //     send sum to core i - divisor;
        // }
    }
    divisor *= 2;
}