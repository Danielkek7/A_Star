#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE 4
#define SQR_SIZE (SIZE * SIZE)
#define TRUE 1 
#define FALSE 0

typedef struct Node {
    int blocks[SIZE][SIZE];
    int total, path, heuristic;
    short empty_row, empty_col;
    struct Node *next;
    struct Node *parent;
} Node;

int goal_rows[SQR_SIZE];
int goal_cols[SQR_SIZE];

Node *init, *end;
Node *unvisited = NULL, *visited = NULL;
Node *successor_nodes[4];

void print_node(Node *node) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            printf("%2d ", node->blocks[i][j]);
        printf("\n");
    }
    printf("\n");
}

Node *initialize(char **argv) {
    Node *node = (Node *)malloc(sizeof(Node));
    int index = 1, block;
    for (int j = 0; j < SIZE; j++)
        for (int k = 0; k < SIZE; k++) {
            block = atoi(argv[index++]);
            node->blocks[j][k] = block;
            if (block == 0) {
                node->empty_row = j;
                node->empty_col = k;
            }
        }
    node->total = 0;
    node->path = 0;
    node->heuristic = 0;
    node->next = NULL;
    node->parent = NULL;
    init = node;
    printf("Initial state\n");
    print_node(init);

    node = (Node *)malloc(sizeof(Node));
    goal_rows[0] = SIZE - 1;
    goal_cols[0] = SIZE - 1;
    for (index = 1; index < SQR_SIZE; index++) {
        int j = (index - 1) / SIZE;
        int k = (index - 1) % SIZE;
        goal_rows[index] = j;
        goal_cols[index] = k;
        node->blocks[j][k] = index;
    }
    node->blocks[SIZE - 1][SIZE - 1] = 0;
    node->total = 0;
    node->path = 0;
    node->heuristic = 0;
    node->next = NULL;
    end = node;
    printf("Goal state\n");
    print_node(end);

    return init;
}

int abs_value(int k) {
    return k < 0 ? -k : k;
}

void swap(int row1, int col1, int row2, int col2, Node *node) {
    int temp = node->blocks[row1][col1];
    node->blocks[row1][col1] = node->blocks[row2][col2];
    node->blocks[row2][col2] = temp;
}

int heuristicFunction(int entry, int row, int col) {
    int x = 0, y = 0;
    if (entry == 0) return 0;
    while (x < SQR_SIZE) {
        while (y < SQR_SIZE) {
            if (end->blocks[x][y] == entry) {
                return abs_value(row - x) + abs_value(col - y);
                y += 1;
            } else
                y += 1;
        }
        x += 1;
    }
}

void update_fgh(Node *node) {  
    int cost1 = 0, cost2 = 0, nl=0, x=0, l=0;
    if (node -> parent == NULL) { 
        node ->path = 1;   
    }
    else { 
        int temp = node->parent->path + 1;
        node->path = temp;  
    }
    while (nl < pow(SIZE,2)) {
        while (nl < pow(SIZE,2)) {
            l++;
            if (node -> blocks[x][nl] != l) cost1++;
            int addOn= heuristicFunction(node -> blocks[x][nl], x, nl);
            cost2 += addOn;
            nl++;  
        }
        nl++;  
    }
    if (cost1 > cost2) node->heuristic = cost1;
    else node -> heuristic = cost2;
    node -> total = node -> path  + node -> heuristic;
}

void merge_open() { 
    int num =0;
    while (num < 4) {
        Node * idx = unvisited, * swappable = unvisited, * mainNode;
        if (successor_nodes[num] != NULL) {
            mainNode = ((Node * ) calloc(1, sizeof(Node)));
            for (int x = 0; x < SIZE; x++)
                for (int z = 0; z < SIZE; z++) mainNode -> blocks[x][z] = successor_nodes[num] -> blocks[x][z];
            mainNode -> empty_row = successor_nodes[num] -> empty_row;
            mainNode -> empty_col = successor_nodes[num] -> empty_col;
            mainNode -> parent = successor_nodes[num] -> parent;
            mainNode -> total = successor_nodes[num] -> total;
            mainNode -> path = successor_nodes[num] -> path;
            mainNode -> heuristic = successor_nodes[num] -> heuristic;
            if (idx == NULL) unvisited = mainNode; 
            else if (mainNode -> total < idx -> total || mainNode -> total == idx -> total) {
                if (mainNode -> heuristic == idx -> heuristic || mainNode -> heuristic < idx -> heuristic) {
                    mainNode -> next = idx;
                    unvisited = mainNode;
                } else {
                    for (;;) {
                        swappable = idx;
                        idx = idx -> next;
                        if (idx == NULL) {
                            swappable -> next = mainNode;
                            mainNode -> next = NULL;
                            break;
                        }
                        if (mainNode -> heuristic < idx -> heuristic || mainNode -> heuristic == idx -> heuristic) {
                            mainNode -> next = idx;
                            swappable -> next = mainNode;
                            break;  
                        }  
                    }   
                }
            } else {
                for (;;) { 
                    swappable = idx;
                    idx = idx -> next;
                    if (idx == NULL) {
                        swappable -> next = mainNode;
                        mainNode -> next = NULL;
                        break;
                    }
                    if (mainNode -> total < idx -> total || mainNode -> total == idx -> total) {
                        mainNode -> next = idx;
                        swappable -> next = mainNode;
                        break;  
                    }   
                }   
            } 
        }
        num++; 
    }   
}

void move_downwards(Node *node) {
    if (node == NULL || node->empty_row >= SIZE - 1) {
        return;
    }
    swap(node->empty_row, node->empty_col, node->empty_row + 1, node->empty_col, node);
    node->empty_row += 1;
}

void move_rightwards(Node *node) {
    if (node == NULL || node->empty_col >= SIZE - 1) {
        return;
    }
    swap(node->empty_row, node->empty_col, node->empty_row, node->empty_col + 1, node);
    node->empty_col += 1;
}

void move_upwards(Node *node) {
    if (node == NULL || node->empty_row <= 0) {
        return;
    }
    swap(node->empty_row, node->empty_col, node->empty_row - 1, node->empty_col, node);
    node->empty_row -= 1;
}

void move_leftwards(Node *node) {
    if (node == NULL || node->empty_col <= 0) {
        return;
    }
    swap(node->empty_row, node->empty_col, node->empty_row, node->empty_col - 1, node);
    node->empty_col -= 1;
}

void expand_node(Node *selected) {
    for (int i = 0; i < SIZE; i++) {
        successor_nodes[i] = (Node *)calloc(1, sizeof(Node));
        memcpy(successor_nodes[i]->blocks, selected->blocks, SQR_SIZE * sizeof(int));
        successor_nodes[i]->empty_col = selected->empty_col;
        successor_nodes[i]->empty_row = selected->empty_row;
        successor_nodes[i]->parent = selected;
    }
    move_downwards(successor_nodes[0]);
    move_rightwards(successor_nodes[1]);
    move_upwards(successor_nodes[2]);
    move_leftwards(successor_nodes[3]);
    for (int i = 0; i < SIZE; i++) {
        update_fgh(successor_nodes[i]);
    }
}

int nodes_are_same(Node *a, Node *b) {
    return memcmp(a->blocks, b->blocks, sizeof(int) * SQR_SIZE) == 0 ? TRUE : FALSE;
}

void filter_succ_nodes(int i, Node *node_list) {
    Node *current = node_list;
    if (successor_nodes[i] != NULL) {
        while (current != NULL) {
            if (nodes_are_same(successor_nodes[i], current)) {
                if (current->total <= successor_nodes[i]->total) {
                    successor_nodes[i] = NULL;
                    break;
                }
            }
            current = current->next;
        }
    }
}

void output_solution(Node *solution_path, int pathlen) {
    printf("Path length: %d\n", pathlen);
    Node *current = solution_path;
    while (current != NULL) {
        print_node(current);
        current = current->next;
    }
}

int main(int argc, char **argv) {
    int iter;
    Node *current_unvisited;

    init = initialize(argv);
    unvisited = init;

    for (iter = 0; unvisited != NULL; ++iter) {
        current_unvisited = unvisited;
        unvisited = unvisited->next;

        if (nodes_are_same(current_unvisited, end)) {
            Node *solution_path = NULL;
            int path_length = 0;
            do {
                current_unvisited->next = solution_path;
                solution_path = current_unvisited;
                current_unvisited = current_unvisited->parent;
                path_length++;
            } while (current_unvisited != NULL);
            output_solution(solution_path, path_length);
            break;
        }

        expand_node(current_unvisited);
        for (int i = 0; i < 4; i++) {
            filter_succ_nodes(i, unvisited);
            filter_succ_nodes(i, visited);
        }
        merge_open();
        current_unvisited->next = visited;
        visited = current_unvisited;

        if (iter % 1000 == 0) printf("Iteration %d\n\n", iter);
        if (iter >= 60000) {
            printf("No solution\n");
            break;
        }
    }
    return 0;
}

