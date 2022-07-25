#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
/*
 * T_S, T_I, T_B:
 * T_S: The specification requires that a BFS has been run
 * T_I: The old implementation will still abuse nondeterminism; it will
 *      simply also iterate over useless edges
 * T_B: The "better" implementation will allow us to run BFS without iterating over
 *      useless edges
 * Witness generation code: I know exactly what this would look like, so let's do that first.
 */

/* TODO: test this with pequin, code up the version that works as a checker (by taking edges in the order in which they're traversed), code up the checker version with pqn, and see how this can be extended w/ binning */

/* TODO: replace all randomly accessed arrays with a big blob of merkle memory; perhaps using macros to make it more cute to access. Then test that in pequin. */

/* TOOD port to pequin, testing each of these little pieces 1 by 1 */

// Array repr:
// We'll have:
// vertices 0..N
// Then each vertex will index into a list spec'ing the start index for the adjacencies
// and a list spec'ing the end index for the adjacencies (or terminated with -1)
// allowing for efficient bfs
// or maybe i should look into msc
// okay msc only gives edges, which would be really inefficient for bfs, unless
// it's bfs on a directed graph and the edges (v, u) are already sorted by v.
// However, edges sorted by which of (v, u) is taken in the BFS would make sense
// if we consider that we can leverage nodeterministic information.
// It all depends on pacakaging:
// -- we could imagine packaging this as "extracting a tree out of the graph", much like mst, in which case binning is just an optimal low level gadget, or
// -- we could imagine packaging this as "a compiler optimization", like with bfs, in which case binning is a backend
// The latter is probably best for **packaging** purposes. The former is obviously asymptotically more optimal, and gives better insights on binning networks themselves

// The simplest example:
// size n
uint8_t vertex_to_edge_string_offset[] = {0, 3, 6, 9};
// size n
int vertex_data[] = {0, 1, 2, 3};
// size n * (max_outdegree)
int edge_strings[] = {1, 3, 0, 2, 1, 3, 0, 2};

#define MAX_OUTDEGREE 2
#define M 4
#define N 4

// Then the bfs will look like the following:
// (Where queue is at least as long as the vertices)

#define STATE_PROCESS_QUEUE 0
#define STATE_ENQUEUE_ADJACENCIES 1
#define ramput(i, src) {memcpy((void *) &(memory[(i)]), (void *) (src), sizeof(int));}
#define ramget(dest, i) {memcpy((void *) (dest), (void *) &(memory[(i)]), sizeof(int));}
#define VISIT(vertex, state) {ramget(&vertex_data_tmp, QUEUE_SIZE + VISITED_SIZE + ADJACENCIES_SIZE + vertex); state -= vertex_data_tmp;}
#define ENQUEUE(element) {ramput(last_enqueued, &(element)); last_enqueued += 1;}
#define DEQUEUE(element) {ramget((element), first_enqueued); first_enqueued += 1;}
#define GET_VISITED(i, visited) {ramget((visited), (i) + 2 * M);}
#define SET_VISITED(i) {ramput((i) + 2 * M, &one);}
#define EMPTY() (first_enqueued > last_enqueued)
#define QUEUE_SIZE (M * 2)
#define VISITED_SIZE (N)
#define ADJACENCIES_SIZE (MAX_OUTDEGREE * N)
#define VERTEX_DATA_SIZE N
#define GET_NEIGHBOR(v, j, dest) {ramget(dest, QUEUE_SIZE + VISITED_SIZE + (v * MAX_OUTDEGREE) + j)}
int bfs(int *vertex_data, int *edge_strings) {
  // First copy over the edge strings
  int *memory = calloc(QUEUE_SIZE + VISITED_SIZE + ADJACENCIES_SIZE + VERTEX_DATA_SIZE, sizeof(int));
  memset(memory, 0, sizeof(int) * (QUEUE_SIZE + VISITED_SIZE + ADJACENCIES_SIZE + VERTEX_DATA_SIZE));
  for (int i = 0; i < ADJACENCIES_SIZE; i++) {
    ramput(QUEUE_SIZE + VISITED_SIZE + i, &(edge_strings[i]));
  }

  for (int i = 0; i < VERTEX_DATA_SIZE; i++) {
    ramput(QUEUE_SIZE + VISITED_SIZE + ADJACENCIES_SIZE + i, &(vertex_data[i]));
  }

  int last_enqueued;
  int first_enqueued;
  last_enqueued = 0;
  first_enqueued = 0;

  int result = 0;
  int state = 0;
  int v = 0;
  int j = 0;
  int one = 1;
  int visited = 0;
  int neighbor;
  int vertex_data_tmp;


  // Enqueue the start vertex
  ENQUEUE(v);
  // 2*m iterations (handshake lemma, the degree of each vertex is added to the queue)
  // and two states per iteration (once dequeing, once enqueueing)
  for (int _i = 0; _i < 4 * M; _i++) {
    if (state == STATE_PROCESS_QUEUE && !EMPTY()) {
      // Dequeue
      DEQUEUE(&v);
      GET_VISITED(v, &visited);
      if (!visited) {
        // Visit
        VISIT(v, result);
        SET_VISITED(v);
        // Enqueue neighbors
        j = 0;
        state = STATE_ENQUEUE_ADJACENCIES;
      }
    } else if (state == STATE_ENQUEUE_ADJACENCIES) {
      GET_NEIGHBOR(v, j, &neighbor);
      if (neighbor != 255) {
        ENQUEUE(neighbor);
      }
      j += 1;
      state = (j == MAX_OUTDEGREE) ? STATE_PROCESS_QUEUE : STATE_ENQUEUE_ADJACENCIES;
    }
  }

  return result;
}

/*
 * For the checker code:
 * Edges are input as an array E
 * such that each pair E_i, E_i+1 where i % 2 == 0 specifies a pair of vertices V_(E_i) V_(E_i/2)
 */
int main(int argc, char **argv) {
  // int arr[] = {0, 0, 0, 0, 0};
  printf("BFS result: %d\n", bfs(vertex_data, edge_strings));
  return 0;
}
