// Devise formulas for the functions that calculate my first i and my last i in
// the global sum example. Remember that each core should be assigned roughly
// the same number of elements of computations in the loop. Hint: First consider
// the case when n is evenly divisible by p.

base = n / p;
remainder = n % p;
for (any core){
	nthread = get_thread_num();
	if (nthread < reminder){
		my_first_i = nthread * (base + 1);
		my_last_i = (nthread + 1) * (base + 1);	
	}
	else{
		my_first_i = nthread * base + remainder;
		my_last_i = (nthread + 1) * base + remainder;	
	}
}
