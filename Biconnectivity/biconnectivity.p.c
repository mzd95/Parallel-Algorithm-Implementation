#include <xmtc.h>
#include <xmtio.h>
#include "/opt/xmt/class/include/library.h"

int D[N];
int T[M];

int linkl[N];

int visited[N];
int parent[N][2];//parent, edge number of (p(v), v)

int next[2*N-2];
int nextC[2*N-2];
int R[2*N-2];

int eulerT[2*N-2][2];
int count;
int used[N];
int tree_edge[2*N-2];
int anti_e[2*N-2];
int preorder[N];
int size[N];

int low[N];
int high[N];
int lowC[N];//copy of low[]
int highC[N];//copy of high[]

//properties of the auxiliary graph
int auxG[M];
int D_auG[N];
int used_num[M];

int new_edge[N-1][3];
int new_R[N];
int dist[N];

//properties of the tree graph
int t_edge[2*N-2][3];
int t_anti[2*N-2];
int antiparallel_c[M];
int t_degrees[N];
int t_vertices[N];

int max_num; 

void normalizeR(){
	int num = N-1;
	for(int i = 2*N - 1; i >= 0; i--) {
		if(i == max_int(new_R, N)) {
			for(int j = 0; j < N; j++) {
				if(new_R[j] == i) {
					new_R[j] = -1;
					dist[j] = num;
					num--;
				}
			}
		}
	}
}

void findParent(){
	spawn(0,N-2) {
		parent[new_edge[$][1]][0] = new_edge[$][0];
		parent[new_edge[$][1]][1] = new_edge[$][2];
	}
}

//get a tree graph out of the results from step 1
void getGraph(){
	int pfs[M];
	prefix_sum_int(T, M, pfs);
	
	spawn(0,M-1){
		if(T[$] == 1) {
			t_edge[pfs[$] - 1][0] = edges[$][0];
			t_edge[pfs[$] - 1][1] = edges[$][1];
			t_edge[pfs[$] - 1][2] = $;
			antiparallel_c[antiparallel[$]] = pfs[$] - 1;
		}
	}
	spawn(0,M-1){
		if(T[$] == 1) {
			t_anti[pfs[$] - 1] = antiparallel_c[$];
		}
	}
	spawn(0,N-1){
		t_vertices[$] = pfs[vertices[$]] - T[vertices[$]];
		if($ != N-1) {
			t_degrees[$] = pfs[vertices[$ + 1]] - pfs[vertices[$]] - T[vertices[$ + 1]] + T[vertices[$]];
		}
	}
	t_vertices[0] = 0;
	t_degrees[N-1] = 2*N - 2 - pfs[vertices[N-1]] + T[vertices[N-1]];
}

void DFS(int v) {
	visited[v] = 1;
	
	for(int i = 0; i < degrees[v]; i++) {
    	int edge_num = vertices[v] + i;
    	int w = edges[vertices[v] + i][1];
    	
    	if(visited[w] == 0 && T[edge_num] == 1) {
    		int temp = count;
    		eulerT[count][0] = v;
    		eulerT[count][1] = w;
    		tree_edge[count] = 1;
    		count++;
    		parent[w][0] = v;
    		parent[w][1] = edge_num;
    		DFS(w);
    		
    		anti_e[temp] = count;
    		anti_e[count] = temp;
    		eulerT[count][0] = w;
    		eulerT[count][1] = v;
    		tree_edge[count] = 0;
    		count++;
		}
	}
}

int isRelated(int v, int w) {
	if(preorder[v] < preorder[w] && preorder[v] + size[v] - 1 < preorder[w]) {
		return 0;
	}
	if(preorder[v] > preorder[w] && preorder[v] > size[w] + preorder[w] - 1) {
		return 0;
	}
	return 1;
}

void verticesNormalization() {
	int first = 0;
	for(int i = 0; i < N; i++) {
		if(used_num[D_auG[i]] == -1) {
			used_num[D_auG[i]] = first;
			first = first + 1;
		}
		D_auG[i] = used[D_auG[i]];
	}
	max_num = first - 1;
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
	
	//////////////--------step 1--------//////////////
	connectivityTG(edges, antiparallel, N, M, D, T);
//	for(int i =0; i < M; i++){
//		printf("edge %d - %d is a tree edge: %d\n", edges[i][0], edges[i][1], T[i]);
//	}
	int count_e = 0;
	for(int i =0; i < M; i++){
		if(T[i] == 1){
			printf("edge %d: %d - %d\n", count_e, edges[i][0], edges[i][1]);
			count_e++;
		}
	}
			
	
	//////////////--------step 2--------//////////////
	spawn(0, N-1) {
		visited[$] = 0;
		used_num[$] = -1;
	}
	parent[0][0] = 0;
	parent[0][0] = -1;
	count = 0;
	spawn(0,M-1){
		next[$] = -1;
	}
	
	getGraph();
//	for(int i =0; i < 2*N-2; i++){
//		printf("t_anti[%d] = %d\n", i, t_anti[i]);
//	}
//	for(int i =0; i < N; i++){
//		printf("t_vertices[%d] = %d\n", i, t_vertices[i]);
//		printf("t_degrees[%d] = %d\n", i, t_degrees[i]);
//	}
	spawn(0,N-1){
		for(int i = 0; i < t_degrees[$]; i++) {
			int edge_num = t_vertices[$] + i;
			int temp = t_vertices[$] + (i + 1) % t_degrees[$];
			next[t_anti[edge_num]] = temp;
		}
	}
	
//	for(int i =0; i < 2*N-2; i++){
//		printf("next[%d] = %d\n", i, next[i]);
//	}
	//DFS(0);
	
	//////////////--------step 3--------//////////////
	spawn(0,2*N-2){
		if(next[$] != 0) {
			nextC[$] = next[$];
		} else {
			nextC[$] = $;
		}
		R[$] = 1;
	}
	
	list_rank(nextC, R, 2*N-2, 0);
//	for(int i =0; i < 2*N-2; i++){
//		printf("R[%d] = %d\n", i, R[i]);
//	}
	
	spawn(0,2*N-2){
		if(R[$] > R[t_anti[$]]) {
			tree_edge[$] = 1;
		} else {
			tree_edge[$] = 0;
		}
	}
//	for(int i =0; i < 2*N-2; i++){
//		printf("tree_edge[%d] = %d\n", i, tree_edge[i]);
//		printf("t_edge[%d]: %d - %d\n", i, t_edge[i][0], t_edge[i][1]);
//	}
	
	int sum[2*N-2];
	prefix_sum_int(tree_edge, 2*N-2, sum);
//	for(int i =0; i < 2*N-2; i++){
//		printf("sum[%d] = %d\n", i, sum[i]);
//	}
	
	spawn(0,2*N-2){
		if(tree_edge[$] == 1) {
			new_R[sum[$] - 1] = R[$];
			new_edge[sum[$] - 1][0] = t_edge[$][0];
			new_edge[sum[$] - 1][1] = t_edge[$][1];
			new_edge[sum[$] - 1][2] = t_edge[$][2];
			size[t_edge[$][1]] = (R[$] - R[t_anti[$]] + 1)/2;
		}
	}
//	for(int i =0; i < N; i++){
//		printf("new_R[%d] = %d\n", i, new_R[i]);
//	}
	normalizeR();
	
//	for(int i =0; i < N; i++){
//		//printf("new_list[%d] = %d\n", i, new_list[i]);
//		printf("dist[%d] = %d\n", i, dist[i]);
//		//printf("new_edge[%d]: %d - %d\n", i, new_edge[i][0], new_edge[i][1]);
//	}
	
	spawn(0, N-2){
		preorder[new_edge[$][1]] = N - dist[$];
	}
	preorder[0] = 0;
	size[0] = N;
	
	for(int i =0; i < N; i++){
		printf("preorder[%d] = %d\n", i, preorder[i]);
	}
	for(int i =0; i < N; i++){
		printf("size[%d] = %d\n", i, size[i]);
	}
	
	//////////////--------step 4--------//////////////
	spawn(0,N-1){
		int t = $;
		int pre = preorder[t];
		lowC[pre] = pre;
		highC[pre] = pre;
		for(int i = 0; i < degrees[t]; i++) {
			int edge_num = vertices[t] + i;
			int w = edges[vertices[t] + i][1];
			if(T[edge_num] == 0) {
				if(preorder[w] < lowC[pre]) {
					lowC[pre] = preorder[w];
				}
				if(preorder[w] > highC[pre]) {
					highC[pre] = preorder[w];
				}
			}
		}
	}
	for(int i =0; i < N; i++){
		printf("lowC[%d] = %d\n", i, lowC[i]);
		printf("highC[%d] = %d\n", i, highC[i]);
	}
	
	spawn(0,N-1){
		int begin = preorder[$];
		int end = begin + size[$] - 1;
		low[$] = query_range_min_int(lowC, N, begin, end);
		high[$] = query_range_max_int(highC, N, begin, end);
	}
	for(int i =0; i < N; i++){
		printf("low[%d] = %d\n", i, low[i]);
		printf("high[%d] = %d\n", i, high[i]);
	}
	
	//////////////--------step 5--------//////////////
	findParent();
	spawn(0,M-1){
		//for each edge in G-T
		if(T[$] == 0) {
			int v = edges[$][0];
			int w = edges[$][1];
			if(preorder[v] < preorder[w]) {
				if(!isRelated(v, w)) {
					auxG[parent[v][1]] = 1;
					auxG[parent[w][1]] = 1;
				}
			}
		}
		
		//for each edge in T
		if(T[$] == 1) {
			int v = edges[$][0];
			int w = edges[$][1];
			if(preorder[v] < preorder[w]) {
				if(low[w] < preorder[v] || high[w] >= preorder[v] + size[v]) {
					//printf("add edge %d - %d to the auxiliary graph. parent[%d]\n", parent[v][0], v, v);
					auxG[parent[v][1]] = 1;
					//printf("add edge %d - %d to the auxiliary graph.\n", v, w);
					auxG[$] = 1;
				}
			}
		}
	}
	
	for(int i =0; i < M; i++){
		printf("edge %d - %d is in the auxiliary graph: %d\n", edges[i][0], edges[i][1], auxG[i]);
	}
	
	//////////////--------step 6--------//////////////
	int pfs[M];
	prefix_sum_int(auxG, M, pfs);
//	for(int i =0; i < M; i++){
//		printf("pfs[%d] = %d\n", i, pfs[i]);
//	}
	int n_edge = pfs[M-1];
	int v_auG[N];
	//new auxiliary graph
	int au_edges[M][2];
	spawn(0, M-1){
		au_edges[$][0] = 0;
		au_edges[$][1] = 0;
	}
	spawn(0,M-1){
		used_num[$] = -1;
		bcc[$] = -1;
		if(auxG[$] == 1){
			au_edges[pfs[$] - 1][0] = edges[$][0];
			au_edges[pfs[$] - 1][1] = edges[$][1];
		}
	}
	//printf("number of edges in the auxiliary graph: %d\n", n_edge);
	connectivity(au_edges, N, n_edge, D_auG);
//	for(int i =0; i < M; i++){
//		printf("au_edge %d - %d\n", au_edges[i][0], au_edges[i][1]);
//	}
	
	//verticesNormalization();
	spawn(0,N-1){
		used_num[D_auG[$]] = 1;
	}
	for(int i =0; i < N; i++){
		printf("D_auG[%d] = %d\n", i, D_auG[i]);
	}
	
	for(int i = 0; i < N; i++) {
		int temp = 0;
		for(int j = 0; j < degrees[i]; j++) {
			int edge_num = vertices[i] + j;
			int w = edges[vertices[i] + j][1];
			//for all the tree edges
			if(T[edge_num] == 1 && bcc[edge_num] == -1) {
				int b = D_auG[edges[edge_num][0]];
				//if the edge is in the graph
				if(auxG[edge_num] == 1) {
					bcc[edge_num] = b;
					bcc[antiparallel[edge_num]] = b;
				} 
				
				//if the edge is not in the graph
				else {
					if(b == D_auG[edges[edge_num][1]]) {
						bcc[edge_num] = b;
					}
					else {
						while(used_num[temp] == 1) {
							temp++;
						}
						bcc[edge_num] = temp;
						bcc[antiparallel[edge_num]] = temp;
						used_num[temp] = 1;
					}
				}
			}
			
	//////////////--------step 7--------//////////////
			else if(T[edge_num] == 0) {
				int b;
				if(preorder[edges[edge_num][0]] < preorder[edges[edge_num][1]]) {
					b = bcc[parent[edges[edge_num][1]][1]];
				} else {
					b = bcc[parent[edges[edge_num][0]][1]];
				}
				bcc[edge_num] = b;
				bcc[antiparallel[edge_num]] = b;
				
			}
		}
	}
	
	//normalize the number
	//normalization();
	
	for(int i = 0; i < M; i++) {
		printf("bcc[%d - %d] = %d\n", edges[i][0], edges[i][1], bcc[i]);
	}
}
