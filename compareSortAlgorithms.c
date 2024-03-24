#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/*
	Author: Nicholas Bonilla
	Purpose: Implementing Different Sorting Algorithms, as well as Parse Data
	Date: 03/20/2024
	*/


int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}


//Heapify Function for Heap Sort
void heapify(int arr[], int n, int i)
{
	//Making variables for heapify
	int max, l, r, temp;

		//Setting Initial Variables
		max = i;
		l = 2 * i + 1;
		r = 2 * i + 2;

		if(l < n && arr[l] > arr[max])
		{
			max = l;
		}

		if(r < n && arr[r] > arr[max])
		{
			max = r;
		}

		if(max != i)
		{
			temp = arr[i];
			arr[i] = arr[max];
			arr[max] = temp;
			heapify(arr, n, max);
		}
	
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	//Calling Heapify
	for(int i = n / 2 - 1; i >= 0; i--)
	{
		heapify(arr, n, i);
	}

	//Heap Sort
	int temp;

	for(int i = n - 1; i >= 0; i--)
	{
		temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;

		//Calling Heapify again
		heapify(arr, i, 0);
	}


}


//Merge function for merge sort
void merge(int pData[], int l, int mid, int r)
{
		//Making Variables
		int i, j, k;
		int n1 = mid - l + 1;
		int n2 = r - mid;

		//Making Temp Arrays Using Alloc Function
		int *lArray = Alloc(sizeof(int) * n1);
		int *rArray = Alloc(sizeof(int) * n2);

		//Copying Data into Arrays

		//Left Array
		for(i = 0; i < n1; i++)
		{
			lArray[i] = pData[l + i];
		}

		//Right Array
		for(i = 0; i < n2; i++)
		{
			rArray[i] = pData[mid + 1 + i];
		}

		//Setting and Resetting Indexes
		i = 0; //Index for left array
		j = 0; //Index for right array
		k = l; //Index for merged array

		//Comparing Arrays
		while(i < n1 && j < n2)
		{
			//If Left subarray is smaller
			if(lArray[i] <= rArray[j])
			{
				pData[k] = lArray[i];
				i++;
			}

			//If Right subarray is smaller
			else
			{
				pData[k] = rArray[j];
				j++;
			}

			//Increasing index of merged array
			k++;
		}

		//Copying any remaining values in arrays
		while(i < n1)
		{
			pData[k] = lArray[i];
			i++;
			k++;
		}

		while(j < n2)
		{
			pData[k] = rArray[i];
			j++;
			k++;
		}


		//Using Dealloc Function on Temp Arrays
		DeAlloc(lArray);
		DeAlloc(rArray);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	//Checking that indexes aren't equal
	if(l < r)
	{
		//Splitting array in half
		int mid = (l + r) / 2;

		//Recursion to split down
		mergeSort(pData, l, mid);
		mergeSort(pData, mid + 1, r);

		//Calling Merge Function
		merge(pData, l, mid, r);

	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	//Making Variables
	int i, j, temp;
	
	//Looping through data
	for(i = 1; i < n; i++)
	{
		//Setting j
		j = i;

		//Comparing and going down array
		while(j > 0 && pData[j-1] > pData[j])
		{
			//Swapping
			temp = pData[j-1];
			pData[j-1] = pData[j];
			pData[j] = temp;

			//Moving back
			j--;
		}
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	//Temp variable
    int temp;

    //Looping through length # of times
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - 1 - i; j++)
        {

            if(pData[j] > pData[j+1])
            {
                temp = pData[j+1];
                pData[j+1] = pData[j];
                pData[j] = temp;
            }
        }
    }
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	//Making Variables
	int min, mIndex, temp;

	//Looping through array
	for(int i = 0; i < n - 1; i++)
	{
		//Setting values
		min = pData[i];
		mIndex = i;
		for(int j = i+1; j < n; j++)
		{
			//Comparing values
			if(min > pData[j])
			{
				min = pData[j];
				mIndex = j;
			}
		}

		//Swapping
		temp = pData[mIndex];
		pData[mIndex] = pData[i];
		pData[i] = temp;
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		//Checking if pointer is null
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (int i=0;i<dataSz;++i)
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
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
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

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
