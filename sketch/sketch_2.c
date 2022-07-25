#include <stdio.h>
#include <stdlib.h>
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

// size n
int vertex_data[] = {0, 1, 2, 3};

// size max_outdegree * n
int edge_strings[] = {1, 3, 0, 2, 0, 2, 1, 3};
int visited[] = {0, 0, 0, 0};

typedef struct {
  int *queue;
  int first_enqueued;
  int last_enqueued;
} Queue;

#define MAX_OUTDEGREE 2
#define INITIALIZE_QUEUE(queuename, value) {(queuename)->queue[0] = (value); (queuename)->first_enqueued = 0; (queuename)->last_enqueued = 0;}
#define VISIT(vertex, state) {state -= vertex_data[(vertex)];}
#define DEQUEUE(into, queuename) {into = (queuename)->queue[queue->first_enqueued]; (queuename)->first_enqueued += 1;}
#define ENQUEUE(element, queuename) {(queuename)->queue[++((queuename)->last_enqueued)] = (element);}
#define EMPTY(queuename) ((queuename)->first_enqueued > (queuename)->last_enqueued)
#define STATE_PROCESS_QUEUE 0
#define STATE_ENQUEUE_ADJACENCIES 1
// Then the bfs will look like the following:
// (Where queue is at least as long as the vertices)
int bfs(int n, int m, int *vertex_data, int *edge_strings, int start_vertex, Queue *queue, int *visited) {
  int result = 0;
  INITIALIZE_QUEUE(queue, start_vertex);
  int state = 0;
  int v = -1;
  int j = 0;
  // 2*m iterations (handshake lemma, the degree of each vertex is added to the queue)
  // and two states per iteration (once dequeing, once enqueueing)
  for (int _i = 0; _i < 4 * m; _i++) {
    if (state == STATE_PROCESS_QUEUE && !EMPTY(queue)) {
      DEQUEUE(v, queue);
      if (!visited[v]) {
        VISIT(v, result);
        visited[v] = 1;
        j = 0;
        state = STATE_ENQUEUE_ADJACENCIES;
      }
    } else if (state == STATE_ENQUEUE_ADJACENCIES) {
      if (j == MAX_OUTDEGREE) {
        state = STATE_PROCESS_QUEUE;
      } else {
        ENQUEUE(edge_strings[v * MAX_OUTDEGREE + j], queue);
        j += 1;
      }
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
  int *arr = (int *) malloc(8 * sizeof(int));
  Queue queue = {.queue = arr};
  printf("BFS result: %d\n", bfs(4, 4, vertex_data, edge_strings, 0, &queue, visited));
  free(arr);
  return 0;
}
