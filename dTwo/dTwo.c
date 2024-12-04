#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void *checkSafety(void *rank);
int abs(int num);

pthread_mutex_t lock;
int **vals;
int safeCount = 0;

int main() {
	pthread_t *threads = malloc(sizeof(pthread_t) * 10);
	FILE *file = fopen("input.txt", "r");
	char line[256];
	vals = malloc(sizeof(int*) * 1000);

	int i = -1;
	while (fgets(line, sizeof(line), file) && ++i < 1000) {
		vals[i] = malloc(sizeof(int) * 10);
		int numbers = 0;
		char *token = strtok(line, " ");
		while (token != NULL) {
			numbers++;
			vals[i][numbers] = atoi(token);
			token = strtok(NULL, " ");
		}
		vals[i][0] = numbers;
		// printf("%d\n", vals[i][0]);
	}
	int thread = -1;
	// printf("%d\n", vals[0][0]);
	while (++thread < 10)
		pthread_create(&threads[thread], NULL, checkSafety, &thread);
	thread = -1;
	while (++thread < 10)
		pthread_join(threads[thread], NULL);
	i = -1;
	while (++i < 1000)
		free(vals[i]);
	free(vals);
	free(threads);
	printf("%d\n", safeCount);
	return 0;
}

void *checkSafety(void *rank) {
	int i = -1;
	printf("Rank %d reporting for duty\n", *(int *)rank);
	while (++i < 100) {
		printf("Row %d\n", *(int *)rank * 100 + i);
		int* current = vals[*(int *)rank * 100 + i];
		int numCount = current[0];
		int validity = 1;
		int j = 1;
		int previousNum = current[1];
		while (++j <= numCount) {
			int difference = current[j] - previousNum;
			// Any two adjacent levels differ by at least one and at most three.
			if (difference == 0 || abs(difference) > 3) {
				validity = 0;
				break;
			}
			int sign = 0;
			// The levels are either all increasing or all decreasing
			if (difference > 0) {
				if (sign == 0)
					sign = 1;
				else if (sign == -1) {
					validity = 0;
					break;
				}
			} else if (difference < 0){
				if (sign == 0)
					sign = -1;
				else if (sign == 1) {
					validity = 0;
					break;
				}
			previousNum = current[j];
			}
		}
		printf("Row %d validity: %d\n", *(int *)rank, validity);
		if (validity) {
			pthread_mutex_lock(&lock);
			safeCount++;
			pthread_mutex_unlock(&lock);
		}
	}
	return NULL;
}

int abs(int num) {
	if (num < 0)
		num *= -1;
	return num;
}
