#include <stdint.h>
#define N 4
#define M 4
#define MAX_OUTDEGREE 2
struct In {
  int vertex_data[N];
  int edge_strings[M * 2 + N];
};

// The following graph:
//    ...(1)...
// (0)         (2)
//    ...(3)...

// size n
uint8_t vertex_data[] = {0, 1, 2, 3};
// size m * 2 + n
uint8_t edge_strings[] = {1, 3, 0, 2, 1, 3, 0, 2};

void bfs4_input_gen (mpq_t * input_q, int num_inputs, char *argv[]) {
  int j = 0;
  for (int i = 0; i < N; i++) {
    mpq_set_ui(input_q[j++], vertex_data[i], 1);
  }
  for (int i = 0; i < N * MAX_OUTDEGREE; i++) {
    mpq_set_ui(input_q[j++], edge_strings[i], 1);
  }
}
