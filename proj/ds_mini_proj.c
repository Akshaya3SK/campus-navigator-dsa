#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAX_LOCATIONS 9

typedef struct Graph {
    int adjMatrix[MAX_LOCATIONS][MAX_LOCATIONS];  
    char locations[MAX_LOCATIONS][50];            
} Graph;

typedef struct Queue {
    int items[MAX_LOCATIONS];
    int front, rear;
} Queue;

void initQueue(Queue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

int isQueueEmpty(Queue* queue) {
    return queue->front == -1;
}

void enqueue(Queue* queue, int value) {
    if (queue->rear == MAX_LOCATIONS - 1) {
        printf("Queue overflow\n");
        return;
    }
    if (queue->front == -1) {
        queue->front = 0;
    }
    queue->items[++queue->rear] = value;
}

int dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Queue underflow\n");
        return -1;
    }
    int item = queue->items[queue->front];
    if (queue->front >= queue->rear) {
        queue->front = -1;
        queue->rear = -1;
    } else {
        queue->front++;
    }
    return item;
}
typedef struct Stack {
    int items[MAX_LOCATIONS];
    int top;
} Stack;

typedef struct SearchNode {
    int start;
    int destination;
    struct SearchNode* left;
    struct SearchNode* right;
} SearchNode;

void initStack(Stack* stack) {
    stack->top = -1;
}

void push(Stack* stack, int location) {
    if (stack->top < MAX_LOCATIONS - 1) {
        stack->items[++stack->top] = location;
    } else {
        printf("Stack overflow\n");
    }
}

int pop(Stack* stack) {
    if (stack->top == -1) {
        printf("Stack underflow\n");
        return -1;
    }
    return stack->items[stack->top--];
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

SearchNode* createSearchNode(int start, int destination) {
    SearchNode* newNode = (SearchNode*)malloc(sizeof(SearchNode));
    newNode->start = start;
    newNode->destination = destination;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

SearchNode* insertSearchNode(SearchNode* root, int start, int destination) {
    if (root == NULL) {
        return createSearchNode(start, destination);
    }
    if (start < root->start || (start == root->start && destination < root->destination)) {
        root->left = insertSearchNode(root->left, start, destination);
    } else if (start > root->start || (start == root->start && destination > root->destination)) {
        root->right = insertSearchNode(root->right, start, destination);
    }
    return root;
}

void displaySearchHistory(SearchNode* root, Graph* graph) {
    if (root == NULL) {
        return;
    }
    displaySearchHistory(root->left, graph);
    printf("Path: %s -> %s\n", graph->locations[root->start], graph->locations[root->destination]);
    displaySearchHistory(root->right, graph);
}

void initGraph(Graph* graph) {
    strcpy(graph->locations[0], "Library");
    strcpy(graph->locations[1], "Dormitory");
    strcpy(graph->locations[2], "Cafeteria");
    strcpy(graph->locations[3], "Admin Building");
    strcpy(graph->locations[4], "Gym");
    strcpy(graph->locations[5], "Auditorium");
    strcpy(graph->locations[6], "Lab");
    strcpy(graph->locations[7], "Parking");
    strcpy(graph->locations[8], "Sports Field");  

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        for (int j = 0; j < MAX_LOCATIONS; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }

    graph->adjMatrix[0][1] = 4;  
graph->adjMatrix[1][0] = 4;  
graph->adjMatrix[1][2] = 3; 
graph->adjMatrix[2][1] = 3;  

graph->adjMatrix[2][3] = 2; 
graph->adjMatrix[3][2] = 2;  

graph->adjMatrix[3][4] = 6;  
graph->adjMatrix[4][3] = 6; 

graph->adjMatrix[4][5] = 5;  
graph->adjMatrix[5][4] = 5;  

graph->adjMatrix[1][4] = 1;  
graph->adjMatrix[4][1] = 1;  

graph->adjMatrix[4][7] = 3;  
graph->adjMatrix[7][4] = 3;  

graph->adjMatrix[7][6] = 4;  
graph->adjMatrix[6][7] = 4;  
graph->adjMatrix[6][5] = 2;  
graph->adjMatrix[5][6] = 2; 

graph->adjMatrix[0][7] = 10; 
graph->adjMatrix[7][0] = 10; 

graph->adjMatrix[5][6] = 3; 
graph->adjMatrix[6][5] = 3;  
graph->adjMatrix[4][8] = 7;  
graph->adjMatrix[8][4] = 7; 

graph->adjMatrix[8][7] = 2;  
graph->adjMatrix[7][8] = 2;  

}

void findPathsUtil(Graph* graph, int current, int destination, int visited[], int path[], int pathIndex, int currentWeight) {
    visited[current] = 1;
    path[pathIndex] = current;
    pathIndex++;

    if (current == destination) {
        printf("Path: ");
        for (int i = 0; i < pathIndex; i++) {
            printf("%s", graph->locations[path[i]]);
            if (i < pathIndex - 1) printf(" -> ");
        }
        printf(" (Distance: %d km)\n", currentWeight);
    } else {
        for (int i = 0; i < MAX_LOCATIONS; i++) {
            if (graph->adjMatrix[current][i] != 0 && !visited[i]) {
                findPathsUtil(graph, i, destination, visited, path, pathIndex, currentWeight + graph->adjMatrix[current][i]);
            }
        }
    }

    visited[current] = 0;
}


void findPaths(Graph* graph, int start, int destination) { 
    int visited[MAX_LOCATIONS] = {0}; 
    int path[MAX_LOCATIONS]; 
    int pathIndex = 0; 
    printf("All Possible Paths:\n"); 
    findPathsUtil(graph, start, destination, visited, path, pathIndex, 0); 
}


void findShortestPathDFSUtil(Graph* graph, int current, int destination, int visited[], int path[], int pathIndex, int pathWeight, int* minWeight, int minPath[]) {
    visited[current] = 1;
    path[pathIndex] = current;
    pathIndex++;

    if (current == destination) {
        if (pathWeight < *minWeight) {
            *minWeight = pathWeight;
            for (int i = 0; i < pathIndex; i++) {
                minPath[i] = path[i];
            }
            minPath[pathIndex] = -1; 
        }
    } else {
        for (int i = 0; i < MAX_LOCATIONS; i++) {
            if (graph->adjMatrix[current][i] != 0 && !visited[i]) {
                findShortestPathDFSUtil(graph, i, destination, visited, path, pathIndex, pathWeight + graph->adjMatrix[current][i], minWeight, minPath);
            }
        }
    }

    visited[current] = 0;
}

void findShortestPathDFS(Graph* graph, int start, int destination) {
    int visited[MAX_LOCATIONS] = {0};
    int path[MAX_LOCATIONS];
    int minPath[MAX_LOCATIONS];
    int pathIndex = 0;
    int minWeight = INT_MAX;

    findShortestPathDFSUtil(graph, start, destination, visited, path, pathIndex, 0, &minWeight, minPath);

    if (minWeight == INT_MAX) {
        printf("No path found from %s to %s\n", graph->locations[start], graph->locations[destination]);
    } else {
        printf("Shortest Path (weight %d km): ", minWeight);
        for (int i = 0; minPath[i] != -1; i++) {
            printf("%s", graph->locations[minPath[i]]);
            if (minPath[i + 1] != -1) printf(" -> ");
        }
        printf("\n");
    }
}

void findPathWithIntermediate(Graph* graph, int start, int intermediate, int destination) {
    printf("Finding path from %s to %s via %s:\n", graph->locations[start], graph->locations[destination], graph->locations[intermediate]);
    
    printf("First leg:\n");
    findShortestPathDFS(graph, start, intermediate);

    printf("Second leg:\n");
    findShortestPathDFS(graph, intermediate, destination);
}

void topologicalSortUtil(int v, int visited[], Stack* stack, Graph* graph) {
    visited[v] = 1;

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        if (graph->adjMatrix[v][i] != 0 && !visited[i]) {
            topologicalSortUtil(i, visited, stack, graph);
        }
    }

    push(stack, v);
}

void topologicalSort(Graph* graph) {
    Stack stack;
    initStack(&stack);
    int visited[MAX_LOCATIONS] = {0};

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        if (!visited[i]) {
            topologicalSortUtil(i, visited, &stack, graph);
        }
    }

    printf("Topological Sort: ");
    while (!isEmpty(&stack)) {
        printf("%s ", graph->locations[pop(&stack)]);
    }
    printf("\n");
}

int minDistance(int dist[], int visited[]) {
    int min = INT_MAX, minIndex = -1;

    for (int v = 0; v < MAX_LOCATIONS; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v], minIndex = v;
        }
    }
    return minIndex;
}

void dijkstra(Graph* graph, int src) {
    int dist[MAX_LOCATIONS];
    int visited[MAX_LOCATIONS] = {0};

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;

    for (int count = 0; count < MAX_LOCATIONS - 1; count++) {
        int u = minDistance(dist, visited);
        visited[u] = 1;

        for (int v = 0; v < MAX_LOCATIONS; v++) {
            if (!visited[v] && graph->adjMatrix[u][v] && dist[u] != INT_MAX
                && dist[u] + graph->adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->adjMatrix[u][v];
            }
        }
    }

    printf("Vertex\tDistance from Source\n");
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        printf("%s\t%d\n", graph->locations[i], dist[i]);
    }
}

int minKey(int key[], int mstSet[]) {
    int min = INT_MAX, minIndex;

    for (int v = 0; v < MAX_LOCATIONS; v++) {
        if (mstSet[v] == 0 && key[v] < min) {
            min = key[v], minIndex = v;
        }
    }
    return minIndex;
}

void printMST(int parent[], Graph* graph) {
    printf("Edge \tWeight\n");
    for (int i = 1; i < MAX_LOCATIONS; i++) {
        printf("%s - %s \t%d \n", graph->locations[parent[i]], graph->locations[i], graph->adjMatrix[i][parent[i]]);
    }
}

void primMST(Graph* graph) {
    int parent[MAX_LOCATIONS];
    int key[MAX_LOCATIONS];
    int mstSet[MAX_LOCATIONS];

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        key[i] = INT_MAX;
        mstSet[i] = 0;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < MAX_LOCATIONS - 1; count++) {
        int u = minKey(key, mstSet);
        mstSet[u] = 1;

        for (int v = 0; v < MAX_LOCATIONS; v++) {
            if (graph->adjMatrix[u][v] && mstSet[v] == 0 && graph->adjMatrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph->adjMatrix[u][v];
            }
        }
    }

    printMST(parent, graph);
}

void addNewPaths(Graph* graph) {
    Queue queue;
    initQueue(&queue);

    int start, end, weight;

    printf("Enter the number of new paths to add: ");
    int numPaths;
    scanf("%d", &numPaths);

    for (int i = 0; i < numPaths; i++) {
        printf("Enter start location (0-5): ");
        scanf("%d", &start);
        printf("Enter end location (0-5): ");
        scanf("%d", &end);
        printf("Enter path weight: ");
        scanf("%d", &weight);

        // Enqueue the new path (start, end, weight)
        enqueue(&queue, start);
        enqueue(&queue, end);
        enqueue(&queue, weight);
    }

    while (!isQueueEmpty(&queue)) {
        start = dequeue(&queue);
        end = dequeue(&queue);
        weight = dequeue(&queue);

        graph->adjMatrix[start][end] = weight;
        graph->adjMatrix[end][start] = weight;
    }

    printf("New paths have been added to the graph.\n");
}

void navigateCampus(Graph* graph) {
    Stack history;
    initStack(&history);
    SearchNode* searchHistoryRoot = NULL;
    int start, destination, intermediate;
    int choice;

    while (1) {
        printf("\n1. View Search History\n");
        printf("2. Find New Path\n");
        printf("3. Find Shortest Path\n");
        printf("4. Find Path with Intermediate Node\n");
        printf("5. Topological sort\n");
        printf("6. Find Shortest distance from node \n");
        printf("7. Connect All Locations Efficiently:\n");
        printf("8. Add New Path \n");
        printf("9. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Search History:\n");
            displaySearchHistory(searchHistoryRoot, graph);
        } else if (choice == 2) {
            printf("Campus Locations:\n");
            for (int i = 0; i < MAX_LOCATIONS; i++) {
                printf("%d. %s\n", i, graph->locations[i]);
            }
            printf("\nEnter starting location (0-8): ");
            scanf("%d", &start);
            printf("Enter destination location (0-8): ");
            scanf("%d", &destination);

            findPaths(graph, start, destination);

            searchHistoryRoot = insertSearchNode(searchHistoryRoot, start, destination);
        } else if (choice == 3) {
            printf("Campus Locations:\n");
            for (int i = 0; i < MAX_LOCATIONS; i++) {
                printf("%d. %s\n", i, graph->locations[i]);
            }
            printf("\nEnter starting location (0-8): ");
            scanf("%d", &start);
            printf("Enter destination location (0-8): ");
            scanf("%d", &destination);

            findShortestPathDFS(graph, start, destination);

            searchHistoryRoot = insertSearchNode(searchHistoryRoot, start, destination);
        } else if (choice == 4) {
            printf("Campus Locations:\n");
            for (int i = 0; i < MAX_LOCATIONS; i++) {
                printf("%d. %s\n", i, graph->locations[i]);
            }
            printf("\nEnter starting location (0-8): ");
            scanf("%d", &start);
            printf("Enter intermediate location (0-8): ");
            scanf("%d", &intermediate);
            printf("Enter destination location (0-8): ");
            scanf("%d", &destination);

            findPathWithIntermediate(graph, start, intermediate, destination);

            searchHistoryRoot = insertSearchNode(searchHistoryRoot, start, intermediate);
            searchHistoryRoot = insertSearchNode(searchHistoryRoot, intermediate, destination);
        } else if (choice == 5) {
            topologicalSort(graph);
        } else if (choice == 6) {
            printf("Campus Locations:\n");
            for (int i = 0; i < MAX_LOCATIONS; i++) {
                printf("%d. %s\n", i, graph->locations[i]);
            }
            printf("\nEnter starting location (0-8): ");
            scanf("%d", &start);

            printf("Dijkstra's Algorithm:\n");
            dijkstra(graph, start);
        } else if (choice == 7) {
            printf("Prim's Algorithm for MST:\n");
            primMST(graph);
        } else if (choice == 8) {
            addNewPaths(graph);
        } else if (choice == 9) {
            printf("Exiting program.\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}




int main() {
    Graph graph;
    initGraph(&graph);
    navigateCampus(&graph);
    return 0;
}