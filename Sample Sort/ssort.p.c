#include <xmtc.h>

int rand = 0;
void quickSort(int arr[], int low, int high) {
	if (low < high) {
		// select the element with the highest index in the array as the pivot	
		int pivot = arr[high];
		
		int i = low - 1;
		int temp;
		
		// partition
		for(int j = low; j <= high - 1; j++) {
			if (arr[j] <= pivot) {
				i++;
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
		
		i++;
		temp = arr[i];
		arr[i] = pivot;
		arr[high] = temp;
		
		//recursively partition
		quickSort(arr, low, i - 1);
		quickSort(arr, i + 1, high);
	}
}

int binarySearch(int arr[], int left, int right, int x)
{
	while (left < right) {
		int middle = (left + right) / 2;
		
		if (x == arr[middle]) {
			return middle + 1;
		}
		
		if (x < arr[middle]) {
			right = middle - 1;
		} else {
			left = middle + 1;
		}
	}
	
	return (x >= arr[left])? (left + 1): left;
}

int main(){
	int set[s * NTCU];
	int i, j;
	
	i = 1;
	
	// hight of the binary tree
	int h = 0;
	while (i < NTCU) {
		h = h + 1;
		i = i * 2;
	}
	
	int count[NTCU][NTCU];
	int partition[N];
	int serial[N];
	
	// step 1
	for (i = 0; i < s * NTCU; i++) {
		set[i] = A[RANDOM[rand] % N];
		rand = (rand + 1) % NRAND;
	}
	
	quickSort(set, 0, s * NTCU - 1);
	int sP[NTCU - 1];
	for (i = 1; i < NTCU; i++) {
		sP[i - 1] = set[i * s];
	}
	
	for (i = 0; i < NTCU; i++) {
		for (int k = 0; k < NTCU; k++) {
			count[i][k] = 0;
		}
	}
	
	int num = N / NTCU;
	
	// step 2
	spawn(0,NTCU-1) {
		int min = $ * num;
		for (int k = min; k < min + num; k++) {
			int pos = 0;
			
			pos = binarySearch(sP, 0, NTCU - 2, A[k]);
			partition[k] = pos;
			serial[k] = count[$][pos];
			count[$][pos]++;
		}
	}
	
	int p_s[NTCU][NTCU];
	int sum[NTCU];
	int global_ps[NTCU + 1];
	
	// step 3
	spawn(0,NTCU-1) {
		sum[$] = 0;
		for (int k = 0; k < NTCU; k++) {
			p_s[k][$] = sum[$];
			sum[$] = sum[$] + count[k][$];
		}
	}
	
	// coutinue step 3
//	int temp = 0;
//	for (i = 0; i < NTCU; i++) {
//		global_ps[i] = temp;
//		temp = temp + sum[i];
//	}
//	global_ps[NTCU] = temp;
	
	// continue step 3, implement prefix_sum in parallel
	int res[NTCU][NTCU];
	int B[NTCU][NTCU];
	global_ps[0] = 0;
	spawn(0,NTCU-1){
		B[0][$] = sum[$];
		int t = 2;
		int index = $ + 1;
		for (int k = 1; k <= h; k ++) {
			if (index >= 1 && index <= NTCU/t) {
				B[k][index - 1] = B[k - 1][2 * index - 2] + B[k - 1][2 * index - 1];
			}
			else {
				B[k][index - 1] = 0;
			}
			t = t * 2;
		}
		res[h][$] = B[h][$];
		
		t = NTCU/2;
		for (int k = h; k > 0; k--) {
			if (index % 2 == 0 && index <= NTCU/t) {
				res[k - 1][index - 1] = res[k][index/2 - 1];
			}
			else if (index == 1) {
				res[k - 1][0] = B[k - 1][0];
			}
			else if (index % 2 == 1 && index <= NTCU/t) {
				res[k - 1][index - 1] = res[k][(index-1)/2 - 1] + B[k-1][index-1];
			}
			else {
				res[k - 1][index - 1] = 0;
			}
			t = t/2;
		}
		
		global_ps[index] = res[0][index - 1];
	}
	
	spawn(0,NTCU-1){
		int min = $ * num;
		int c = $;
		
		// step 4
		for (int k = min; k < min + num; k++) {
			int position = global_ps[partition[k]] + p_s[c][partition[k]] + serial[k];
			result[position] = A[k];
		}
	}
	
	// step 5
	spawn(0,NTCU-1){
		quickSort(result, global_ps[$], global_ps[$ + 1] - 1);
	}
	
	// validation
//	for (i = 0; i < N; i++) {
//		if (i < N-1 && result[i] > result[i + 1]) {
//			printf("wrong answer!");
//			printf(" %d %d \n", result[i], result[i+1]);
//		}
//	}
	
}
