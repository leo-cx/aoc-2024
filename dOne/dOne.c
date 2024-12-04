#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    int lines = 1000; 
    int *left = malloc(sizeof(int) * lines);
    int *right = malloc(sizeof(int) * lines);

    int currLine = 0;
    while (currLine < lines) {
	scanf("%d   %d", &left[currLine], &right[currLine]);
	currLine++;
    }

    qsort(left, lines, sizeof(int), compare);
    qsort(right, lines, sizeof(int), compare);

    int similaritySum = 0;
    int repeatSum = 0;
    int i = -1;
    while (++i < lines) {
	int result = left[i] - right[i];
	if (result < 0) result *= -1;
	similaritySum += result;
	int j = -1;
	while (++j < lines) {
	    if (left[i] == right[j])
		repeatSum += left[i];
	}
    }
    printf("%d\n", similaritySum);
    printf("%d\n", repeatSum);
    return 0;
}
