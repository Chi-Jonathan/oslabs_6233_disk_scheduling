
#include "oslabs.h"
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Null RCB
const struct RCB NULLRCB = {0, 0, 0, 0, 0};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper functions

void print_rcb(struct RCB rcb) {
    printf("Req ID: %d\n Arr Time: %d\n Cylinder: %d\n Address: %d\n PID: %d\n",
        rcb.request_id, rcb.arrival_timestamp, rcb.cylinder, rcb.address, rcb.process_id);
}



// Function to remove an RCB from the request queue and return the running rcb
struct RCB remove_from_queue(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int req_index) {

    struct RCB running_rcb = request_queue[req_index];

    // Remove rcb from queue
    for (int i = req_index; i < *queue_cnt-1; i++) {
        request_queue[i] = request_queue[i+1];
    }
    *queue_cnt -= 1;

    return running_rcb;
}


// Function to add an RCB to queue
struct RCB add_to_queue(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request) {
    if (current_request.process_id == 0) {
        return new_request;
    }
    else {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// First Come First Serve

// Handles request arrival fcfs
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    return add_to_queue(request_queue, queue_cnt, current_request, new_request);

}



// Handles Request Completion fcfs
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt) {
    int lowest_at_index = -1;
    int lowest_at = 1000000;

    // Find the first in the queue
    for (int i = 0; i < *queue_cnt; i++) {
        if (request_queue[i].arrival_timestamp < lowest_at) {
            lowest_at_index = i;
            lowest_at = request_queue[i].arrival_timestamp;
        }
    }
    struct RCB rcb = remove_from_queue(request_queue, queue_cnt, lowest_at_index);
    return rcb;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Shortest Seek First

// Handles request arrival sstf
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    return add_to_queue(request_queue, queue_cnt, current_request, new_request);
}



// Handles Request Completion sstf
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder) {
    if (*queue_cnt == 0) {
        return NULLRCB;
    }
    int shortest_dist_index[QUEUEMAX];
    int shortest_dist = 1000000;
    int index_cnt = 0;

    // Find the shortest distance cylinder
    for (int i = 0; i < *queue_cnt; i++) {
        if (abs(request_queue[i].cylinder - current_cylinder) < shortest_dist) {
            shortest_dist_index[0] = i;
            shortest_dist = abs(request_queue[i].cylinder - current_cylinder);
            index_cnt = 1;
        }
        else if (abs(request_queue[i].cylinder - current_cylinder) == shortest_dist) {
            shortest_dist_index[index_cnt] = i;
            index_cnt += 1;
        }
    }



    if (index_cnt > 1) {
        int at_index = 0;
        int shortest_at = 10000000;
        for (int i = 0; i < index_cnt; i++) {
            if (request_queue[shortest_dist_index[i]].arrival_timestamp < shortest_at) {
                at_index = shortest_dist_index[i];
                shortest_at = request_queue[at_index].arrival_timestamp;
            }
        }
        struct RCB rcb = remove_from_queue(request_queue, queue_cnt, at_index);
        return rcb;
    }
    else {
        struct RCB rcb = remove_from_queue(request_queue, queue_cnt, shortest_dist_index[0]);
        return rcb;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Look/elevator scheduler

// Handles request arrival look
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    return add_to_queue(request_queue, queue_cnt, current_request, new_request);
}



// Handles Request Completion look
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction) {
    if (*queue_cnt == 0) {
        return NULLRCB;
    }
    int shortest_dist_index = -1;
    int shortest_dist = 1000000;

    // Find the shortest distance cylinder
    for (int i = 0; i < *queue_cnt; i++) {
        if (scan_direction == 0 && current_cylinder > request_queue[i].cylinder && current_cylinder - request_queue[i].cylinder < shortest_dist) {
            shortest_dist_index = i;
            shortest_dist = current_cylinder - request_queue[i].cylinder;
        }
        else if (scan_direction == 1 && current_cylinder < request_queue[i].cylinder && request_queue[i].cylinder - current_cylinder < shortest_dist) {
            shortest_dist_index = i;
            shortest_dist = request_queue[i].cylinder - current_cylinder;
        }
    }
    if (shortest_dist_index == -1) {
        return handle_request_completion_sstf(request_queue, queue_cnt, current_cylinder);
    }
    else {
        struct RCB rcb = remove_from_queue(request_queue, queue_cnt, shortest_dist_index);
        return rcb;
    }
}
















