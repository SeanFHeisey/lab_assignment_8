#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Sean Heisey
// COP 3502
// 4/5/2023
// lab 8

int extraMemoryAllocated;

// swaps x and y
// swap function assists with heapify & heapSort
void swap(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

// heapify function assists with heapSort
void heapify(int arr[], int n, int i)
{
	// largest value of root and children
    int largest = i,
		l = 2 * i + 1,
		r = 2 * i + 2;
    if(l<n&&arr[l]>arr[largest])
        largest = l;
    if(r<n&&arr[r]>arr[largest])
        largest = r;
	// swap if value isn't largest
    if(largest!=i){
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	// create max heap
	for(int i=n/2-1; i>=0; i--)
        heapify(arr, n, i);
	// sort
    for(int i=n-1; i>=0; i--){
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

// merge function assists with mergeSort
void merge(int pData[], int l, int m, int r)
{
    int i, j, k, 
		nIdx1 = m - l + 1, 
		nIdx2 = r - m;
	// temp arrays
    int *lArr = malloc(nIdx1*sizeof(int)), 
		*rArr = malloc(nIdx2*sizeof(int));
	// calculates extra memory
	extraMemoryAllocated += nIdx1 * sizeof(int) + nIdx2 * sizeof(int);
	// copies to temp arrays
    for(i=0; i<nIdx1; i++)
        lArr[i] = pData[l+i];
    for(j=0; j<nIdx2; j++)
        rArr[j] = pData[m+1+j];
	// indexes
    i = 0, j = 0, k = l; 
	// merges data back
    while(i<nIdx1&&j<nIdx2){
        if (lArr[i]<=rArr[j]){
            pData[k] = lArr[i];
            i++;
        }else{
            pData[k] = rArr[j];
            j++;
        }
        k++;
    }
	// copies leftover indexes
    while(i<nIdx1){
        pData[k] = lArr[i];
        i++;
        k++;
    }
    while(j<nIdx2){
        pData[k] = rArr[j];
        j++;
        k++;
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if(l<r){
        int m = l + (r - l) / 2;
        mergeSort(pData, l, m);
        mergeSort(pData, m + 1, r);
        merge(pData, l, m, r);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}


int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}