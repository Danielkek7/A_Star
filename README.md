# A_Star
This code is visualizing the layout of command-line arguments in memory. It prints out memory addresses and their contents in both hexadecimal and ASCII representations, from the address of the first argument to the address of the last argument.

"main()" function:
The main function begins by initializing the puzzle from command-line arguments using the "initialize()" function. It then enters a loop where it continues to execute the A* algorithm until a solution is found or until a certain iteration limit is reached. In each iteration, the node with the lowest total cost is selected and removed from the unvisited list. If this node matches the goal state, the solution path is reconstructed by tracing back from the goal to the initial state using parent pointers and then printed out. Otherwise, the selected node is expanded to generate successor nodes, filtered, and merged into the unvisited list in ascending order of their total cost. Finally, the selected node is moved to the visited list.

"initialize(char **argv)" function:
This function creates a Node structure for the initial and goal states. The Node structure contains the configuration of the puzzle (2D array of blocks), the total cost of reaching the state (f), the cost from the start to the current node (g), and the estimated cost from the current node to the goal (h), as well as the position of the empty block. The goal state is defined as the numbered tiles in ascending order with the blank space in the last position.

"update_fgh(Node *node)" function:
This function is used to update the cost values (f, g, h) of a node. The path cost (g) is the number of steps from the start node to the current node. The heuristic cost (h) is estimated using the sum of the Manhattan distances from each block to its position in the goal state. The total cost (f) is the sum of g and h.

"expand_node(Node *selected)" function:
This function generates successor nodes by moving the empty block in each possible direction (up, down, left, right) using the "move_downwards()", "move_rightwards()", "move_upwards()", and "move_leftwards()" functions. After the successor nodes are generated, their cost values are updated using the "update_fgh()" function.

"filter_succ_nodes(int i, Node *node_list)" and "merge_open()" functions:
These functions are used to process the successor nodes. The "filter_succ_nodes()" function removes any nodes from the successor nodes that are already present in the unvisited or visited lists and have a lower or equal total cost. The "merge_open()" function inserts the remaining successor nodes into the unvisited list in ascending order of their total cost.
