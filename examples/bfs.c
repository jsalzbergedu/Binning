#include <stdint.h>

#define N 4
#define M 4
#define MAX_OUTDEGREE 2
#define STATE_PROCESS_QUEUE 0
#define STATE_ENQUEUE_NEIGHBORS 1

struct In {
    int vertex_data[N];
    int edge_strings[N * MAX_OUTDEGREE];
};

struct Out {
    uint8_t result_0[N];
    uint8_t result_1[N * MAX_OUTDEGREE];
    uint8_t bfs_accum_result;
};

void compute(struct In *input, struct Out *output) {
    /**
     * The queue of vertices
     */
    int queue[M * 2];
    /**
     * The visited vertices
     */
    int visited[N];
    /**
     * Indexes into the front / least recently inserted
     * item of the queue
     */
    int first_enqueued;
    /**
     * Indexes into the back / most recently inserted item of the queue
     */
    int last_enqueued;
    /**
     * A state dictating whether the program is visiting a vertex or enqueuing
     * its adjacencies
     */
    int state;
    /**
     * The curent vertex being processed
     */
    int v;
    /**
     * The loop iteration
     */
    int _i;
    /**
     * The index of the adjacency of the vertex being processed
     */
    int j;
    /**
     * A loop index used for initialization
     */
    int k;

    /**
     * The offset of the string of adjacent vertices for the vertex under
     * exploration.
     */
    int vertex_adjacency_start;

    /**
     * The offset of the adjacent vertex being enqueued in the edgestring.
     */
    int neighbor_offset;

    /**
     * The index of the adjacent vertex being enqueued
     */
    int neighbor;
    // Initialize the output.

    // Initialize the queue.
    queue[0] = 0;
    first_enqueued = 0;
    last_enqueued = 0;

    // Choose the state.
    state = STATE_PROCESS_QUEUE;

    output->bfs_accum_result = 1;
    for (_i = 0; _i < 4 * M; _i++) {
        if (state == STATE_PROCESS_QUEUE) {
            // When the queue has an element
            if (first_enqueued <= last_enqueued) {
                /* // Dequeue */
                v = queue[first_enqueued];
                first_enqueued += 1;
                // Visit
                if (visited[v] != 1) {
                    output->bfs_accum_result *= (input->vertex_data[v] + 1);
                    visited[v] = 1;
                    // And enqueue neighbors
                    j = 0;
                    state = STATE_ENQUEUE_NEIGHBORS;
                }
            }
        } else if (state == STATE_ENQUEUE_NEIGHBORS) {
            if (j == MAX_OUTDEGREE) {
                state = STATE_PROCESS_QUEUE;
            } else {
                last_enqueued += 1;
                neighbor_offset = v + j;
                neighbor = input->edge_strings[neighbor_offset];
                if (neighbor != 255) {
                    queue[last_enqueued] = neighbor;
                }
                j += 1;
            }
        }
    }

    // Constrain output slots to keep the compiler happy
    for (_i = 0; _i < N; _i++) {
        output->result_0[_i] = input->vertex_data[_i] * input->vertex_data[_i];
    }
    for (_i = 0; _i < N * MAX_OUTDEGREE; _i++) {
        output->result_1[_i] = input->vertex_data[0] * input->vertex_data[0];
    }
}
