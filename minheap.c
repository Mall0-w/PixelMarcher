/**
 *                        Min-Heaps
 * 
 * This is the A2 section of your coding assignment. You will only submit this
 * along with the A3 section when A3 is due, but it is recommended that you
 * complete this early on so you have time to work on second part of this
 * assignment (in `marcher.c` ).
 * 
 * Please make sure you read the blurb in `minheap.h` to ensure you understand 
 * how we are implementing min-heaps here and what assumptions / requirements 
 * are being made.
 * 
 * CSCB63 Winter 2021 - Assignment 2/3
 * (c) Mustafa Quraish
 */

#include "minheap.h"



/**
 * Allocate a new min heap of the given size.
 * 
 * TODO: 
 *  Allocate memory for the `MinHeap` object, and the 2 arrays inside it.
 *  `numItems` should initially be set to 0, and all the indices should be
 *   set to -1 to start with (since we don't have anything in the heap).
 */
 void printheap(MinHeap *heap) {
  printf("Heap: (numItems = %d / size = %d)\n", heap->numItems, heap->maxSize);
  printf("  arr = [");
  for (int i=0; i < heap->numItems; i++) {
    printf("(%d, %f), ", heap->arr[i].val, heap->arr[i].priority);
  }
  printf("]\n  ind = [");
  for (int i=0; i < heap->maxSize; i++) {
    printf("%d, ", heap->indices[i]);
  }
  printf("]\n");
}

MinHeap *newMinHeap(int size) {
	//allocate memeory for heap
	MinHeap* new = calloc(sizeof(MinHeap), 1);
	//check if there's enough memory
	if(new == NULL){
		fprintf(stderr, "not enough memeory for minHeap\n");
		return NULL;
	}
	//set default values
	new->numItems = 0;
	new->maxSize = size;
	
	//allocate memeory for arrays
	new->arr = calloc(sizeof(HeapElement), size);
	new->indices = calloc(sizeof(int), size);
	if(new->arr == NULL || new->indices == NULL){
		fprintf(stderr, "not enough memeory for minHeap\n");
		return NULL;
	}
	//set default values for indicies
	for(int i = 0; i < size; i++){
		new->indices[i] = -1;
	}
	

  return new; // Allocate and return heap.
}

/**
 * Swaps elements at indices `a` and `b` in the heap, and also updates their
 * indices. Assumes that `a` and `b` are valid.
 * 
 * NOTE: This is already implemented for you, no need to change anything.
 */
void swap(MinHeap *heap, int a, int b) {
  // Swap the elements
  HeapElement temp = heap->arr[a];
  heap->arr[a] = heap->arr[b];
  heap->arr[b] = temp;

  // Refresh their indices
  heap->indices[heap->arr[a].val] = a;
  heap->indices[heap->arr[b].val] = b;
}

void heapify(MinHeap* heap, int index){
	int parentIndex = floor(((index-1)/2));
	/*printf("--------\n");
	printf("parent : %lf , child: %lf\n",heap->arr[parentIndex].priority, heap->arr[index].priority );
	printheap(heap);
	printf("--------\n");
	*/
	if(index != parentIndex && heap->arr[index].priority < heap->arr[parentIndex].priority){
		//printf("child less \n");
		//percolate up
		//printf("parent : %lf  Child : %lf\n", heap->arr[parentIndex].priority, heap->arr[currentIndex].priority);
		swap(heap, parentIndex, index);
		heapify(heap,parentIndex);
	}else{
		int least = index;
		int right = 2*index + 2;
		int left = 2*index +1;
		if(right < heap->numItems && heap->arr[right].priority < heap->arr[least].priority){
			least = right;
		}
		if(left < heap->numItems && heap->arr[left].priority < heap->arr[least].priority){
			least = left;
		}
		if(least != index){
			swap(heap, least, index);
			heapify(heap, least);
		}
	}
	return;
}

/**
 * Add a value with the given priority into the heap.
 * 
 * TODO: Complete this function, and make sure all the relevant data is updated
 *      correctly, including the `indices` array if you move elements around. 
 *      Make sure the heap property is not violated. 
 * 
 * You may assume the value does not already exist in the heap, and there is
 * enough space in the heap for it.
 */
void heapPush(MinHeap *heap, int val, double priority) {
	//start at leftmost leaf
	int currentIndex = heap->numItems;
	//gather parent index
	int pIndex = floor(((currentIndex-1)/2));
	
	//if heap not full
	
	if(currentIndex <= heap->maxSize){
		heap->arr[currentIndex].priority = priority;
		heap->arr[currentIndex].val = val;
		heap->indices[val] = currentIndex;
		//printf("before heapify\n parent: %lf, child %lf\n", heap->arr[pIndex].priority, heap->arr[currentIndex].priority);
		heap->numItems++;
		heapify(heap, heap->numItems-1);
		
		//printf("testing  parent: %d, %lf    current: %d, %lf\n", heap->arr[pIndex].val, heap->arr[pIndex].priority,heap->arr[currentIndex].val, heap->arr[currentIndex].priority);
	}
	else{
		fprintf(stderr, "inserting into full heap/n");
		return;
	}
	//while child is larger than parent and child not first element in head
	/*while(currentIndex != 0 && heap->arr[currentIndex].priority < heap->arr[parentIndex].priority){
		//percolate up
		//printf("parent : %lf  Child : %lf\n", heap->arr[parentIndex].priority, heap->arr[currentIndex].priority);
		swap(heap, parentIndex, currentIndex);
		currentIndex = parentIndex;
		parentIndex = floor((currentIndex/2));
	}
	*/
	
	
  return; // Push value to heap before returning
}

/**
 * Extract and return the value from the heap with the minimum priority. Store
 *  the priority for this value in `*priority`. 
 * 
 * For example, if `10` was the value with the lowest priority of `1.0`, then
 *  you would have something that is equivalent to:
 *      
 *        *priority = 1.0;
 *        return 10;
 * 
 * TODO: Complete this function, and make sure all the relevant data is updated
 *      correctly, including the `indices` array if you move elements around. 
 *      Make sure the heap property is not violated. 
 * 
 * You may assume there is at least 1 value in the heap.
 */
int heapExtractMin(MinHeap *heap, double *priority) {
	//get priority and everything from first element in heap->arr
	//shift everything in heap->arr one element to the leftmost
	//shift everything in index
  double minPri = heap->arr[0].priority;
  int minVal = heap->arr[0].val;
  if(heap->numItems > 1){
  //printf("min pri: %lf \n", minPri);
  //printf("testing; 0:%lf numItems: %lf\n", heap->arr[0].priority, heap->arr[heap->numItems].priority);
  heap->arr[0] = heap->arr[heap->numItems-1];
  heap->indices[heap->arr[0].val] = 0;
  heap->indices[minVal] = -1;
  heap->numItems--;
  //printf("testing; 0:%lf numItems: %lf\n", heap->arr[0].priority, heap->arr[heap->numItems].priority);
  heapify(heap, 0);
  }
  else{
	  heap->indices[0] = -1;
	  heap->numItems--;
  }
  /*
  for(int i = 0; i < heap->numItems-1 ; i++){
	heap->arr[i] = heap->arr[i+1];
	heap->indices[heap->arr[i].val] = i;
  }
  heap->numItems--;
  */
  *priority = minPri;  // Set correct priority  
  return minVal;         // Return correct value
}

/**
 * Decrease the priority of the given value (already in the heap) with the
 * new priority.
 * 
 * NOTE: You will find it helpful here to first get the index of the value
 *       in the heap from the `indices` array.
 * 
 * TODO: Complete this function, and make sure all the relevant data is updated
 *      correctly, including the `indices` array if you move elements around. 
 *      Make sure the heap property is not violated. 
 * 
 * You may assume the value is already in the heap, and the new priority is
 *  smaller than the old one (caller is responsible for ensuring this).
 */
void heapDecreasePriority(MinHeap *heap, int val, double priority) {
	
	heap->arr[heap->indices[val]].priority = priority;
	heapify(heap, heap->indices[val]);
  return;   // Decrease priority before return
}

/**
 * Free the data for the heap. This won't be marked, but it is always good
 * practice to free up after yourself when using a language like C.
 */
void freeHeap(MinHeap *heap) {
	free(heap->arr);
	free(heap->indices);
	free(heap);
  return;   // Free heap and it's arrays
}
