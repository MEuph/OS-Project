#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void display_memory_usage();

int main(int argc, char** argv) {
	srand(time(NULL));

	if (argc != 4) {
		printf("Usage: ./main <list_size> <max_size> <num_frees>\nMakes a 2D list of integers, with each list having anywhere from 1-<max_size> integers, then frees a specified number of them in a random order, then displays the memory usage of the program\n");
	} else {
		display_memory_usage();
		int num_el = atoi(argv[1]);
		int max_size = atoi(argv[2]);
		printf("Making a list of size %d...\n", num_el);

		int** numbers = (int**)malloc(sizeof(int*) * num_el);

		int p = 0;
		for (int i = 0; i < num_el; i++) {
			int list_size = (rand() % max_size - 1) + 1;
			numbers[i] = (int*)malloc(sizeof(int) * (list_size + 1));
			numbers[i][0] = list_size;
			for (int j = 1; j < list_size + 1; j++) {
				numbers[i][j] = rand();
			}
			if (i % (num_el / 10) == 0) {
				p += 10;
				printf("%d percent done...\n", p);
			}
		}
		p = 0;

		for (int i = 2; i < 10; i++) {
			printf("numbers[%d] = %p | size = %zu bytes\n", num_el / i, (void*)(numbers[num_el / i]), numbers[num_el / i][0] * sizeof(int));
		}

		bool* already_visited = (bool*)malloc(sizeof(bool) * num_el);
		for (int i = 0; i < num_el; i++) {
			already_visited[i] = false;
		}

		display_memory_usage();

		int num_frees = atoi(argv[3]);
		printf("Freeing %d elements...\n", num_frees);

		for (int i = 0; i < num_frees; i++) {
			int to_free = rand() % (num_el - 1);
			while (already_visited[to_free] == true) to_free = rand() % (num_el - 1);
			free(numbers[to_free]);
			already_visited[to_free] = true;
			if (i % (num_frees / 10) == 0) {
				p += 10;
				printf("%d percent done...\n", p);
			}
		}

		printf("Freed %d elements...\n", num_frees);
		display_memory_usage();
		free(numbers);
		free(already_visited);
	}

	return 0;
}

void display_memory_usage() {
	printf("Memory Usage:\n");
	FILE* file = fopen("/proc/self/status", "r");
	if (!file) {
		perror("fopen");
		return;
	}

	char line[256];
	while (fgets(line, sizeof(line), file)) {
		if (strncmp(line, "VmRSS:", 6) == 0) {
			printf("\t%s", line);
			break;
		}
	}
	fclose(file);
}
