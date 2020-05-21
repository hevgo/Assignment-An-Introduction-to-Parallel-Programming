Hammer login:

yjing
changeMe!123
ssh yjing@hammer.hpc.utulsa.edu

$passwd

changeMe!123 --> Chailina_88558

$ scp file.txt remote_username@10.10.0.2:/remote/directory

$ scp file.txt remote_username@10.10.0.2:~/HPC

-p for specify ssh port
-r for folder

##############
run mpi
$ mpiexec -n <number of processes> ./run
