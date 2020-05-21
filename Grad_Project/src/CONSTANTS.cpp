#define N 101
#define M 101
#define L 10.0
#define W 10.0
#define V 1.0
#define A 100.0
#define MAX_LINE 80


#define start_t 0.0
#define end_t 1.0
#define dt 0.0002
#define c dt * dt * V * V / ( L * W /(N-1) /(M-1))

#define damp 0.3 * dt
#define amp 10.0 / (L/(N-1))
#define tot N * M