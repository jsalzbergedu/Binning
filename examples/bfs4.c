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

    /**
     * True only when there are elements in the queue
     */
    bool queue_nonempty;
    /**
     * True only when there are elements in the queue and the program is processing the queue (dequeuing)
     */
    bool processing_nonempty_queue;
    /**
     * True only when the program is processing the queue
     */
    bool processing_queue;
    /**
     * True only when the vertex is not yet visisted
     */
    bool unvisited;
    /**
     * True only when the program is processing a yet unvisited node
     */
    bool processing_new_node;
    /**
     * True only when the program is not processing the queue
     */
    bool not_processing_queue;
    /**
     * True only when the program state is enqueueing neighbors
     * or the program has just exited the dequeuing state
     */
    bool enqueueing_neighbors;
    /**
     * True only when the program is walking the neighbos of a vertex
     */
    bool statechange_enqueueing_neighbors;
    /**
     * True only when the program has walked all of the neighbors
     */
    bool looped_all_neighbors;
    /**
     * True only when the program has not yet finished walking
     * all of the neighbors
     */
    bool not_looped_all_neighbors;
    /**
     * True only when the program has walked all the neighbors,
     * and the program is in the state where it walks the neighbors
     */
    bool looped_all_neighbors_and_enqueueing;
    /**
     * True only when the program has not yet looped
     * all neighbors, and is walking the neighbors
     */
    bool looping_all_neighbors_and_enqueueing;
    /**
     * True only when the program is processing a nonempty neighbor of a vertex
     * or is not processing neighbors of the vertex at all
     */
    bool at_nonempty_neighbor;
    /**
     * True only when the program is processing a nonempty neighbor of the vertex
     */
    bool looping_neighbors_and_enqueueing_nomempty;

    // Initialize the queue.
    queue[0] = 0;
    first_enqueued = 0;
    last_enqueued = 0;

    // Choose the state.
    state = STATE_PROCESS_QUEUE;

    output->bfs_accum_result = 1;

    for (_i = 0; _i < 4 * M; _i++) {
        processing_queue = state == STATE_PROCESS_QUEUE;
        queue_nonempty = first_enqueued <= last_enqueued;
        if (processing_queue) {
            if (queue_nonempty) {
                processing_nonempty_queue = 1;
            }
        } else {
            processing_nonempty_queue = 0;
        }
        v = processing_nonempty_queue ? queue[first_enqueued] : v;
        printf("v: %Zd, state: %Zd", v, state);
        first_enqueued = processing_nonempty_queue ? first_enqueued + 1 : first_enqueued;
        unvisited = visited[v] != 1;
        processing_new_node = (processing_nonempty_queue && unvisited);
        output->bfs_accum_result =
            processing_new_node ? output->bfs_accum_result * (input->vertex_data[v] + 1) : output->bfs_accum_result;
        visited[v] = processing_new_node ? 1 : visited[v];
        j = processing_new_node ? 0 : j;
        state = processing_new_node ? STATE_ENQUEUE_NEIGHBORS : state;
        not_processing_queue = (!processing_queue);
        enqueueing_neighbors = state == STATE_ENQUEUE_NEIGHBORS;
        statechange_enqueueing_neighbors = not_processing_queue && state == enqueueing_neighbors;
        looped_all_neighbors = j == MAX_OUTDEGREE;
        looped_all_neighbors_and_enqueueing = statechange_enqueueing_neighbors && looped_all_neighbors;
        state = looped_all_neighbors_and_enqueueing ? STATE_PROCESS_QUEUE : state;
        not_looped_all_neighbors = !looped_all_neighbors;
        looping_all_neighbors_and_enqueueing = statechange_enqueueing_neighbors && not_looped_all_neighbors;
        last_enqueued = looping_all_neighbors_and_enqueueing ? last_enqueued + 1 : last_enqueued;
        neighbor_offset = looping_all_neighbors_and_enqueueing ? v + j : 0;
        neighbor = looping_all_neighbors_and_enqueueing ? input->edge_strings[neighbor_offset] : neighbor;
        at_nonempty_neighbor = neighbor != 255;
        looping_neighbors_and_enqueueing_nomempty = looping_all_neighbors_and_enqueueing && at_nonempty_neighbor;
        queue[last_enqueued] = looping_neighbors_and_enqueueing_nomempty ? neighbor : queue[last_enqueued];
        j = looping_neighbors_and_enqueueing_nomempty ? j + 1 : j;
    }

    // Constrain output slots to keep the compiler happy
    for (_i = 0; _i < N; _i++) {
        output->result_0[_i] = input->vertex_data[_i] * input->vertex_data[_i];
    }
    for (_i = 0; _i < N * MAX_OUTDEGREE; _i++) {
        output->result_1[_i] = input->vertex_data[0] * input->vertex_data[0];
    }
}
