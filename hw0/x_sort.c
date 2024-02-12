#include <stdio.h>

// Function to merge two sorted sub-arrays
void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

// Function to perform merge sort
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Input: An array of integers and its length
// Output: Nothing
// Side Effect: The input array is sorted from least to greatest
void sort(int *arr, int len) {
    mergeSort(arr, 0, len - 1);
}

/* FEEDBACK FUNCTIONS */

// Input: An array of integers and its length
// Output: 1 if the array is sorted least to greatest, 0 otherwise
// Side Effect: None
int is_sorted(int *arr, int len) {
    int i = 0;
    while (i < (len - 1)) {
        if (arr[i] > arr[i+1]) {
            return 0;
        }
        i++;
    }
    return 1;
}

// The special main function

int main() {
    int test_arr[10] = {8,6,4,2,0,1,3,5,7,9};
    sort(test_arr, 10);
    if (is_sorted(test_arr, 10)) {
        printf("Good job!\n");
    } else {
        printf("Keep it up!\n");
    }
    return 0;
}
