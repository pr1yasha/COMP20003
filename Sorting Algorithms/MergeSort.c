#include <stdio.h>

void mergeSsort(int a[], int length);
void mergeSortRecursion(int a[], int l, int r);
void mergeSortedArrays(int a[], int l, int m, int r);

int main(){
  int array[] = { 9, 4, 8, 1, 7, 0, 3, 2, 5, 6};
  int length = 10;
  
  mergeSort(array, length);
  
  for (int i = 0; i < length; i++)
    printf("%d ", array[i]);
  printf("\n");
  return 0;
}

void mergeSort(int a[], int length){
  mergeSortRecursion(a, 0, length - 1);
}

void mergeSortRecursion(int a[], int left, int right){

    // only call recursion when left is less than right
    if (left < right){
        int middle = left + (right - left) / 2;
        mergeSortRecursion(a, left, middle);
        mergeSortRecursion(a, middle+1, right);
        mergeSortRecursion(a, left, middle, right);
    }
}

void mergeSortedArrays(int a[], int left, int middle, int right){
    int firstLen = middle - left + 1;
    int secondLen = right - middle;
    
    // create two temporary sub arrays
    int temp_left[firstLen];
    int temp_right[secondLen];

    int i, j, k;

    // copy the left part of the array to temp array
    for (int i = 0; i < left_length; i++){
        temp_left[i] = a[left + i];
    }

    // copy the right part of the array to temp array
    for (int i = 0; i < right_length; i++){
        temp_right[i] = a[middle + 1 + i];
    }

    for (i=0, j=0, k=left; k<= right; k++){
        // compare the values of each list and merge
        if ((i < firstLen) && (j >= secondLen || temp_left[i] <= temp_right[j])){
            a[k] = temp_left[i];
            i++;
        }
        else{
            a[k] = temp_right[j];
            j++;
        }

    }

}
