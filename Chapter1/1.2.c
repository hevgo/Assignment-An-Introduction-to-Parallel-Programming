// We’ve implicitly assumed that each call to Compute next value requires
// roughly the same amount of work as the other calls. How would you change
// your answer to the preceding question if call i = k requires k + 1 times as much
// work as the call with i = 0? So if the first call (i = 0) requires 2 milliseconds,
// the second call (i = 1) requires 4, the third (i = 2) requires 6, and so on.

//solution 1

n_tot = (1+n)*n/2;
each_core_work = n_tot/p;
my_first_i[p];
my_last_i[p];
sum = 0
my_first_i.append(0)
for (size_t i = 0; i < p; i++){
    sum += i + 1;
    if (sum > each_core_work){
        my_last_i.append(i-1)
    }
    sum = i + 1;
    my_first_i.append(i)
}
my_last_i.append(p-1);

for(each_core){
    nthread = get_thread_num();
    my_first_i = my_first_i[nthread]
    my_last_i = my_last_i[nthread]
}

//solution 2

n_each = n/p + (n%p != 0);
assignment[p][n_each];
for (size_t i = 0; i < p; i++){
    if (i%(2*p) < p){
        assignment[i%p].append(i);
    }
    else{
        assignment[p - i%p - 1].append(i);
    }
}

//solution 3

n_each = n/p + (n%p != 0);
assignment[p][n_each];
for (size_t i = 0; i < p; i++){
    if (i%(2*p) < p){
        assignment[i%p].append(n-i);
    }
    else{
        assignment[p - i%p - 1].append(n-i);
    }
}