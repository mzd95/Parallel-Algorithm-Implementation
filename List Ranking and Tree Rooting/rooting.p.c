#include <xmtc.h>

int next[M];
int next_write[M];
int dist[M];
int dist_write[M];

void List_Rank() {
	spawn(0, M-1) {
		if (next[$] == NIL) {
			dist[$] = 0;
		} else {
			dist[$] = 1;
		}
		next_write[$] = next[$];
		dist_write[$] = dist[$];
	}
	
	//number of log(M) iterations
	for (int k = 0; k < log(M); k++) {
		spawn(0, M-1) {
			if (next[$] != NIL) {
				dist_write[$] = dist[$] + dist[next[$]];
				next_write[$] = next[next[$]];
			}
		}
		
		spawn(0, M-1) {
			dist[$] = dist_write[$];
			next[$] = next_write[$];
		}
	}
}

int log(int m) {
	int h = 0;
	int temp = 1;
	while (temp < m) {
		h = h + 1;
		temp = temp * 2;
	}
	return h;
}

int main(){
	//set pointer for every directed edge
	spawn(0, N-1) {
		for (int i = 0; i < deg[$]; i++) {
			int ind = $;
			if(i + 1 != deg[ind]) {
				next[ptr[V[ind] + i]] = V[ind] + i + 1;
			} else {
				next[ptr[V[ind] + i]] = V[ind];
			}
		}
	}
	
	next[ptr[V[root]]] = NIL;
//	printf("root is number %d\n", root);
//	printf("number of vertices is %d\n", N);
//	printf("number of edges is %d\n", M);
//	
//	for (int i = 0; i < M; i ++) {
//		if(next[i] != NIL) {
//			printf("%d to %d next is %d to %d\n", E[i][0], E[i][1], E[next[i]][0], E[next[i]][1]);
//		}
//		else {
//			printf("%d to %d next is -1\n", E[i][0], E[i][1]);
//		}
//	}
	
	List_Rank();
	
//	for (int i = 0; i < M; i ++) {
//		printf("%d to %d distance is %d\n", E[i][0], E[i][1], dist[i]);
//	}
	
	spawn(0, M-1) {
		if (dist[$] < dist[ptr[$]]) {
			used[$] = 1;
		} else {
			used[$] = 0;
		}
	}
	
//	for (int i = 0; i < M; i ++) {
//		if(used[i] == 1) {
//			printf("%d-%d", E[i][0], E[i][1]);
//		}
//	}
}
