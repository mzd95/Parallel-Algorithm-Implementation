#include <xmtc.h>
#include <xmtio.h>

int ver_count;
int edge_count;
int num[N];//the order in the DFS traversal
int visited[N];//if the vertex is visited
int low[N];//the lowest num among the descendants of the vetrex
int estack[M][3];
int top = -1;

int parent[N];

int num_bcc;//number of bcc in the graph

int used[N];

//if the stack is empty
int isEmpty() {
   	if(top == -1)
      	return 1;
   	else
      	return 0;
}

//pop data out of the stack
int pop() {
   	int data;
   	if(!isEmpty()) {
   		data = estack[top][2];
	    top = top - 1;
	    //printf("poping edge %d - %d from the stack.\n", estack[top][0], estack[top][1]);
	    return data;
   	} else {
      	//printf("Could not retrieve data, Stack is empty.\n");
   }
}

//push edge (v, w) into stack
int push(int v, int w, int data) {
    top = top + 1;
    estack[top][0] = v;
    estack[top][1] = w;
    estack[top][2] = data;
    edge_count++;
    //printf("adding edge %d - %d to the stack.\n", v, w);
}

void BCCUtil(int v) {
	visited[v] = 1;
	//printf("vertex %d is visited!\n", v);
	
	num[v] = ++ver_count;
	low[v] = num[v];
	
    //printf("visiting vertex: %d\n", v);
 
    // Recur for all the vertices adjacent
    // to this vertex
    for(int i = 0; i < degrees[v]; i++) {
    	int edge_num = vertices[v] + i;
    	int w = edges[vertices[v] + i][1];
    	
    	//except for the edge leading back to the parent
    	if(w != parent[v]) {
    		//if the edge is a tree edge
	    	if(visited[w] == 0) {
	    		parent[w] = v;
	    		push(v, w, edge_num);
	    		BCCUtil(w);
	    		low[v] = (low[v] < low[w]) ? low[v] : low[w];
	    		
	    		//If v is an articulation point,
	            //pop all edges from stack till v -- w
	            if(low[w] >= num[v]) {
	    		//if( (parent[v] == -1 && children[v] > 1) || (parent[v] != -1 && low[w] >= num[v]) ) {
    			    //printf("%d is an articulation point\n", v);
	    			//printf("%d - %d, num[v] = %d, low[w] = %d\n", v, w, num[v], low[w]);
	                while(!isEmpty()){
	                	int temp = pop();
	                	bcc[temp] = num_bcc;
	                	if(temp == edge_num) {
	                		break;
						}
	            	}
	            	num_bcc++;
	            	//printf("num_bcc ++\n");
				}
			}
			
			//if the edge is a back edge
			else if(visited[w] == 1 && num[w] < num[v]) {
				//push edge (v, w) into estack
	    		push(v, w, edge_num);
	    		
	    		low[v] = (low[v] < num[w]) ? low[v] : num[w];
			}
		}
	    	
	}
}

void normalization() {
	int first = 0;
	for(int i = 0; i < M; i++) {
		if(used[bcc[i]] == -1) {
			used[bcc[i]] = first;
			first = first + 1;
		}
		bcc[i] = used[bcc[i]];
	}
}

int main() {
	// TODO: compute biconnectivity
	//printf("number of vertices: %d\n", N);
	
	for(int i = 0; i < N; i++) {
		visited[i] = 0;
		parent[i] = -1;
		used[i] = -1;
	}
	
	for(int i = 0; i < M; i++) {
		bcc[i] = -1;
		//printf("%d - %d\n", edges[i][0], edges[i][1]);
	}
	
	ver_count = 0;
	
	num_bcc = 0;
	
	BCCUtil(0);
	
	while(!isEmpty()) {
		int temp = pop();
	    bcc[temp] = num_bcc;
	    //printf("bcc[%d] = %d\n", temp, num_bcc);
	}
	
	for(int i = 0; i < M; i++) {
		if(bcc[i] != -1) {
			bcc[antiparallel[i]] = bcc[i];
		}
	}
	
//	for(int i = 0; i < M; i++) {
//		printf("bcc[%d] = %d\n", i, bcc[i]);
//	}
	//printf("number of vertices visited: %d\n", ver_count);
	//printf("number of edges visited: %d\n", edge_count);
	
	//normalize the number
	normalization();
	
//	for(int i = 0; i < M; i++) {
//		printf("bcc[%d] = %d\n", i, bcc[i]);
//	}
	
	return 0;
}
