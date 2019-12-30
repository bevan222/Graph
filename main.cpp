#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*struct type*/
struct AdjListNode {
	int data;
	struct AdjListNode* next;
};

struct AdjList {
	struct AdjListNode* first;
};

struct Graph {
	int vertex;
	struct AdjList* arr;
};

struct Node {
	int data;
	struct Node* next;
};

struct Queue {
	struct Node* front;
	struct Node* rear;
	int size;
};

/*function*/
struct Graph* createGraph(int vertex);
void addEdge(struct Graph* graph, int vertex1, int vertex2);
void bfs(struct Graph* graph, int start_vertex);
void dfs(struct Graph* graph, int start_vertex);
void dfsVisit(struct Graph* graph, int start_vertex, bool* visited);
void showAdjList(struct Graph* graph);
struct Node* createNode(int value);
struct Queue* createQueue();
void enqueue(struct Queue* queue, int value);
int dequeue(struct Queue* queue);
bool outRange(int input, int vertex_num);
bool edgeIsExist(struct Graph* graph, int vertex1, int vertex2);

int main(void) {
	/*addEdge(graph, 0, 1);
	addEdge(graph, 0, 2);
	addEdge(graph, 1, 2);
	addEdge(graph, 1, 4);
	addEdge(graph, 1, 3);
	addEdge(graph, 2, 4);
	addEdge(graph, 3, 4);
	addEdge(graph, 5, 6);*/
	int vertex_num = 0, vertex1 = 0, vertex2 = 0, start_vertex = 0;
	char continue_loop = 'y';

	/*input how many vertexs*/
	do {
		printf("Please enter how many vertexs do you want to use in a undirected graph (must bigger than 0) =>");
		scanf("%d", &vertex_num);
	} while (vertex_num <= 0);

	struct Graph* graph = createGraph(vertex_num);

	/*input edge*/
	do {
		printf("\nPlease enter two vertexs(0 ~ %d) which you want to add a edge (ex:0 1) =>", vertex_num - 1);
		scanf("%d %d", &vertex1, &vertex2);

		if (outRange(vertex1, vertex_num) || outRange(vertex2, vertex_num)) {
			printf("Input is out of range\n");
			continue;
		}
		else {
			if (!edgeIsExist(graph, vertex1, vertex2)) {
				addEdge(graph, vertex1, vertex2);
			}
			else {
				printf("The edge has existed!!!\n");
			}
		}

		getchar();										//eat '\n'
		printf("Do you want to continue? (y/n) =>");
		scanf("%c", &continue_loop);

	} while (continue_loop == 'y' || continue_loop == 'Y');

	/*input the starting vertex*/
	do {
		printf("\nEnter the vertex which you want start =>");
		scanf("%d", &start_vertex);
	} while (outRange(start_vertex, vertex_num));

	/*show result*/
	printf("\n");
	showAdjList(graph);
	bfs(graph, start_vertex);
	printf("\n");
	dfs(graph, start_vertex);

	system("PAUSE");
	return 0;
}

struct Graph* createGraph(int vertex) {
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	graph->vertex = vertex;

	graph->arr = (struct AdjList*) malloc(vertex * sizeof(struct AdjList));
	for (int i = 0; i < vertex; i++) {
		graph->arr[i].first = NULL;
	}

	return graph;
}

void addEdge(struct Graph* graph, int vertex1, int vertex2) {
	struct AdjListNode* node;
	
	/*vertex2 -> vertex1*/
	node = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	node->data = vertex1;
	node->next = graph->arr[vertex2].first;
	graph->arr[vertex2].first = node;

	/*vertex1 -> vertex2*/
	node = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	node->data = vertex2;
	node->next = graph->arr[vertex1].first;
	graph->arr[vertex1].first = node;
}

void bfs(struct Graph* graph, int start_vertex) {
	struct Queue* queue = createQueue();
	bool* visited = (bool*) malloc(graph->vertex * sizeof(bool));
	
	for (int i = 0; i < graph->vertex; i++) {
		visited[i] = false;
	}

	visited[start_vertex] = true;
	enqueue(queue, start_vertex);

	printf("\nBFS:");

	while (queue->size != 0) {
		int current_vertex = dequeue(queue);

		printf(" %d ->", current_vertex);

		struct AdjListNode* temp = graph->arr[current_vertex].first;			//record this vertex's adjlist

		while (temp) {
			int adj_vertex = temp->data;

			/*if the vertex is not visited, add it in the queue*/
			if (!visited[adj_vertex]) {
				visited[adj_vertex] = true;
				enqueue(queue, adj_vertex);
			}

			temp = temp->next;			//keep tracing the other vertexs connect with this vertex
		}
	}
	printf(" end ");

	/*if there is a vertex which was not visited, bfs it*/
	for (int i = 0; i < graph->vertex; i++) {
		if (!visited[i]) {
			visited[i] = true;
			enqueue(queue, i);

			while (queue->size != 0) {
				int current_vertex = dequeue(queue);

				printf(" %d ->", current_vertex);

				struct AdjListNode* temp = graph->arr[current_vertex].first;

				while (temp) {
					int adj_vertex = temp->data;

					if (!visited[adj_vertex]) {
						visited[adj_vertex] = true;
						enqueue(queue, adj_vertex);
					}
					temp = temp->next;
				}
			}
			printf(" end ");
		}
	}

	printf("\n");
	free(visited);
}

void dfs(struct Graph* graph, int start_vertex) {
	bool* visited = (bool*)malloc(graph->vertex * sizeof(bool));

	for (int i = 0; i < graph->vertex; i++) {
		visited[i] = false;
	}

	printf("\nDFS:");
	dfsVisit(graph, start_vertex, visited);
	printf(" end ");

	/*if there is a vertex which was not visited, dfs it*/
	for (int i = 0; i < graph->vertex; i++) {
		if (!visited[i]) {
			dfsVisit(graph, i, visited);
			printf(" end ");
		}
	}
	
	printf("\n");
	free(visited);
}

void dfsVisit(struct Graph* graph, int start_vertex, bool* visited) {
	struct AdjListNode* temp = graph->arr[start_vertex].first;

	if (!visited[start_vertex]) {
		visited[start_vertex] = true;
		printf(" %d ->", start_vertex);
	}

	while (temp) {
		int vertex = temp->data;
		
		/*if vertex was not visited, dfs it*/
		if (!visited[vertex]) {
			dfsVisit(graph, vertex, visited);
		}

		temp = temp->next;			//keep tracing the other vertexs connect with this vertex
	}
}

void showAdjList(struct Graph* graph) {
	int vertex_number = graph->vertex;
	
	if (vertex_number == 0) {
		printf("There is no vertex in the graph.\n");
	}
	else {
		for (int i = 0; i < vertex_number; i++) {
			printf("Vertex: %d, adjacent to:", i);

			struct AdjListNode* temp = graph->arr[i].first;
			while (temp) {
				printf(" %d", temp->data);
				temp = temp->next;
			}
			printf("\n");
		}
	}
}

struct Node* createNode(int value) {
	struct Node* node = (struct Node*) malloc(sizeof(struct Node));
	node->data = value;
	node->next = NULL;

	return node;
}

struct Queue* createQueue() {
	struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
	queue->front = NULL;
	queue->rear = NULL;
	queue->size = 0;
	return queue;
}

void enqueue(struct Queue* queue, int value) {
	struct Node* node = createNode(value);

	if (queue->size == 0) {
		queue->front = node;
		queue->rear = queue->front;
	}
	else {
		queue->rear->next = node;
		queue->rear = node;
	}

	queue->size++;
}
int dequeue(struct Queue* queue) {
	if (queue->size == 0) {
		printf("\nQueue is empty\n");
		return -1;
	}
	else {
		int result = queue->front->data;
		struct Node* node = queue->front;
		queue->front = queue->front->next;
		free(node);
		node = NULL;
		queue->size--;
		return result;
	}
}

bool outRange(int input, int vertex_num) {
	if (input >= 0 && input < vertex_num) {
		return false;
	}
	else {
		return true;
	}
}

bool edgeIsExist(struct Graph* graph, int vertex1, int vertex2) {
	struct AdjListNode* temp = graph->arr[vertex1].first;

	while (temp) {
		if (temp->data == vertex2) {
			return true;
		}
		temp = temp->next;
	}

	return false;
}