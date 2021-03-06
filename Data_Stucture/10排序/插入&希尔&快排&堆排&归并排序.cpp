﻿#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

//declaration
void swap(int &a, int &b);
void show_array(int* arr, int n);
int get_time(time_t t_start, time_t t_end);
template <class T>
int getArrayLen(T& array);


void InsertSort(int* arr, int n);		//直接插入排序
void BinInsertSort(int* arr, int n);	//二分插入排序
void TWayInsertSort(int* arr, int* temp, int n);	//2路直接插入排序
void TWayBinInsertSort(int* arr, int size);		//2路二分插入排序
void ShellSort(int* arr, int n);				//Shell排序算法
void QuickSort(int* arr, int i, int j);			//快速排序算法
void HeapSort(int *a, int size);			    //堆排序 

int getPartition(int* arr, int i, int j);			//快速排序数组划分算法			
void HeapAdjust(int *a, int i, int size);		//堆调整算法
void BuildHeap(int *a, int size);				//最小堆排序

void insertMergeSort(int* arr, int left, int right);		//归并辅助插入排序
void TWayMerge(int* arr, int left, int mid, int right);	//双向监视哨归并
void doSort(int* arr, int left, int right);				//递归归并排序
void mergeSort(int* arr, int left, int right);			//归并函数主接口



int main()
{
	return 0;
}

/* 每次将一个待排序的记录，按大小插入到前面已经排好序的子序列中，直到全部记录插入完成为止。
 * 设数组为a[0…n-1]
 * 1.初始时，a[0]自成1个有序区，无序区为a[1..n-1]。
 * 2.令i=1，将a[i]并入当前的有序区a[0…i-1]中形成a[0…i]的有序区间。
 * 3.i++并重复第二步直到i==n-1。排序完成。
 */
/**
 * @brief 直接插入排序
 * @brief 时间复杂度O(n^2)
 * @param a
 * @param n
 */
void InsertSort(int* arr, int n)  
{  
	for(int i = 1; i < n; i++)
	{
		for(int j = i-1; j >= 0 && arr[j] > arr[j + 1]; j--)
		{
			swap(arr[j], arr[j + 1]);
		}
	}
}

/**
 * @brief 二分插入排序
 * @brief 元素查找时间复杂度O(n·log2(n))
 * @brief 元素移动时间耗费仍为O(n^2)
 * @brief 故算法时间复杂度仍为O(n^2)
 * @param arr
 * @param n
 */
void BinInsertSort(int* arr, int n)
{
	int low, high, mid;
	int i, j;
	for(i = 1; i < n; i++)
	{
		low = 0;
		high = i - 1;
		while(low <= high)		//二分法查找插入位置
		{
			mid = (low + high) / 2; 	//指向已排序好的中间位置
			if(arr[i] < arr[mid])		//即将插入的元素应当在在左区间
				high = mid - 1;
			else                    //即将插入的元素应当在右区间
				low = mid + 1;
		}
		/* 每次查找完毕后，low总比high大一
		 * a[low]总是存放第一个比temp大的数，因此应从此处开始，每个元素右移一位，并将temp存入a[low]中
		 * 这样就保证了a[0...i]是排好序的
		 */
		for(j = i-  1; j >= low; j--	)
		{	//后移排序码大于R[i]的记录
           swap(arr[j+1], arr[j]);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//		final						first
//---------------------------------------------
//|	11 | 13 |    |    |    |    | 3  | 7  | 9  |
//---------------------------------------------
/**
 * @brief 2路插入排序算法
 * @param arr
 * @param temp
 * @param n
 */
 void TWayInsertSort(int* arr, int* temp, int n)
{
	int i, first, final, k;
	first = final = 0;
	temp[0] = arr[0];

	for(i = 1; i < n; i++)
	{
		if(arr[i] < temp[first])
		{	
			first = (first - 1 + n) % n;		//待插入元素比最小的元素小
			temp[first] = arr[i];
		}
		else if(arr[i] > temp[final])		//待插入元素比最大的元素大
		{ 
			final = (final + 1 + n) % n;
			temp[final] = arr[i];
		}
		else	//插入元素比最小大，比最大小
		{ 
			k = (final + 1 + n) % n;
			while(temp[(k - 1 + n) % n] > arr[i])
			{
				temp[(k + n) % n] = temp[(k - 1 + n) % n];
				k = (k - 1 + n) % n;
			}
			temp[(k + n) % n] = arr[i];
			final = (final + 1 + n) % n;
		}
	}
	
	// 将排序记录复制到原来的顺序表里
	for(k = 0; k < n; k++)
	{
		arr[k] = temp[(first + k) % n];
	}
}

/**
 * @brief 2路插入二分算法
 * @param arr
 * @param size
 */
void TWayInsertSort(int *arr, int size)  
{
	int first, final, mid;
	int temp[size];
	first = final = 0;
	temp[0] = arr[0];
	for(int i = 1; i < size; i++)
	{
		if(arr[i] >= temp[final])
		{
			temp[++final] = arr[i];
		}
		else if(arr[i] <= temp[first])
		{
			first = (first - 1 + size) % size;
			temp[first] = arr[i];
		}
		else		//进行二分插入排序，在网上看了很多人的这部分是直接插入的。
		{
			int low, high;
			if(arr[i] < temp[0])
			{
				low = first;
				high = size-1;
			} 
			else
			{
				low = 0;
				high = final;
			} 
			while(low <= high)
			{
				mid = (low + high) / 2;
				if(arr[i] < temp[mid])
					high = mid - 1;
				else 
					low = mid + 1;
			}
			for(int j = final; j != high; j = (j - 1 +size) % size)
			{
				temp[(j+1)%size] = temp[j];
			}
			temp[(high+1)%size] = arr[i]; 
			final++;   
		}//end else
	}//end for
	for(int i = 0, j = first; i < size; j = (j + 1) % size, ++i)
	{
		arr[i]=temp[j];
	}    
}

/**
 * @brief Shell排序算法
 * @param arr
 * @param n
 * @return 
 */
void ShellSort(int* arr, int n)  
{
    int i, j, gap;  
    for(gap = n / 2; gap > 0; gap = gap / 2)
	{  
        for(i = gap; i < n; i++)
		{
			
            for(j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j = j - gap) 
			{
                swap(arr[j], arr[j + gap]); 
			}
		}
	}
}

/**
 * @brief 快速排序数组划分算法
 * @param arr
 * @param i
 * @param j
 * @return 
 */
int getPartition(int* arr, int i, int j)  
{  
	int temp = arr[i]; 
	while(i < j)  
	{  
		//从右向左找小于x的数来填s[i]  
		while(i < j && arr[j] >= temp)   
			j--;    
		if(i < j) 	
			arr[i++] = arr[j]; 		//将s[j]填到s[i]中，s[j]就形成了一个新的坑  
		//从左向右找大于或等于x的数来填s[j]  
		while(i < j && arr[i] <= temp)  
			i++;    
		if(i < j)  
			arr[j--] = arr[i];
	}  
    //退出时，i等于j。将x填到这个坑中。  
	arr[i] = temp;  
	return i;  
}  

/**
 * @brief 快速排序算法
 * @param arr
 * @param i
 * @param j
 */
void QuickSort(int* arr, int i, int j)
{
	if(i < j)
	{
		int pivot = getPartition(arr, i, j);
		QuickSort(arr, i, pivot - 1);
		QuickSort(arr, pivot + 1, j);
	}
}

////////////////////////////////堆结构存储///////////////////////////////
/* 一般用数组来表示堆，i结点的父结点下标为(i – 1)/2
 * 它的左右子结点下标分别为2*i+1和2*i+2
 * 如第0个结点左右子结点下标分别为1和2
 *
 *				 10				
 *		  	   /   \
 *		  	  /     \				-------------------------------
 *	  	    15      56			| 10 | 15 | 56 | 25 | 30 | 70 |
 *	       / \      /				-------------------------------
 * 	      /   \    /
 *	     25   30  70
 *		   逻辑结构							存储结构
 */
/////////////////////////////////////////////////////////////////////////
/**
 * @brief 把二叉树中以i为根的子树变成最大堆。
 * @brief 注意: 使用的前提条件是i节点的左右子树(如果存在的话)都是最大堆
 * @param a
 * @param i
 * @param size
 */
void HeapAdjust(int *a, int i, int size)  //调整堆 
{
    int lchild = 2 * i;       	//i的左孩子节点序号 
    int rchild = 2 * i + 1;   	//i的右孩子节点序号 
    int max = i;         		//临时变量 
    if(i <= size / 2)        	//如果i是叶节点就不用进行调整 
    {
        if(lchild < size && a[lchild] > a[max])
        {
            max = lchild;
        }    
        if(rchild < size && a[rchild] > a[max])
        {
            max = rchild;
        }
        if(max != i)
        {
            swap(a[i], a[max]);
            HeapAdjust(a, max, size);    //避免调整之后以max为父节点的子树不是堆 
        }
    }        
}

void BuildHeap(int *a, int size)    //建立堆 
{
    int i;
    for(i = size / 2; i >= 0; i--)    //非叶节点最大序号值为size/2 
    {
        HeapAdjust(a, i, size);    
    }    
} 

void HeapSort(int *a, int size)    //堆排序 
{
    int i;
    BuildHeap(a, size);
    for(i = size - 1; i > 0; i--)
    {
        swap(a[0], a[i]);           //交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到最后面 
		//BuildHeap(a, i-1);        //将余下元素重新建立为大顶堆 
		HeapAdjust(a, 0, i);  		//重新调整堆顶节点成为大顶堆
    }
} 

/**
 * @brief 归并辅助插入排序
 * @param arr
 * @param left
 * @param right
 */
void insertMergeSort(int* arr, int left, int right)
{  
	int i,j;
	int temp;  
	for(i = left + 1 ; i <= right; i++)	//外层循环是从第二个元素开始的  
	{
		if(arr[i] < arr[i-1])
		{  
			temp = arr[i];  
			j = i - 1;  
			while(j >= left && temp < arr[j])
			{
				arr[j+1] = arr[j];
				j--;
			}
			arr[j+1] = temp;  
		}  
	}  
}  
  
/**
 * @brief 双向监视哨归并
 * @param arr
 * @param left
 * @param mid
 * @param right
 */
void TWayMerge(int* arr, int left, int mid, int right)
{  
	int s1 = left;//s1,s2 是检测指针，t是存放指针  
	int s2 = right;  
	int t = left,k;  
	int* b = new int[right];  
	for(k = left ; k <= mid; k++)		//正向复制  
		b[k] = arr[k];  
	for(k = mid + 1; k <= right; k++)	//反向复制  
		b[right+mid+1-k] = arr[k];  
    while(t <= right)						//归并过程
	{  
		if(b[s1] <= b[s2])
			arr[t++] = b[s1++];  
		else
			arr[t++] = b[s2--];  
	}  
}  
  
/**
 * @brief 递归归并排序
 * @param a
 * @param left
 * @param right
 */
void doSort(int* arr, int left, int right)
{  
	if(left >=right)
		return;  
	if(right - left + 1 < getArrayLen(arr))	//序列长度小于M时候，跳出循环 
		return; 
	int mid = (left + right )/2;			//从中间划分两个子序列  
	doSort(arr, left, mid);					//从左侧子序列进行递归排序  
	doSort(arr, mid + 1, right);			//从右侧子序列进行递归排序  
	TWayMerge(arr, left, mid, right);		//合并  
}  

/**
 * @brief 归并函数主接口
 * @param arr
 * @param left
 * @param right
 */
void mergeSort(int* arr, int left, int right)
{  
    doSort(arr, left, right);				//对序列进行归并排序  
    insertMergeSort(arr, left, right);	//对排序结果在做插入排序  
}  


void swap(int &a, int &b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void show_array(int* arr, int n)
{
	for(int i = 0; i < n; ++i)
	{
		cout << arr[i] << '\t';
		if((i+1) % 15 == 0)	cout << endl;
	}
	cout << endl;
}

int get_time(time_t t_start, time_t t_end)
{
	return (t_end - t_start)*1000/CLOCKS_PER_SEC;
}


/**
 * @brief 使用模板定义一 个函数getArrayLen
 * @brief 返回数组array的长度
 * @param array
 * @return 
 */
template <class T>
int getArrayLen(T& array)
{
	return (sizeof(array) / sizeof(array[0]));
}