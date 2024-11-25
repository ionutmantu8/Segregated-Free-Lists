#include "sfl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	sfl *heap = NULL;
	mem_a_t *mem_a = malloc(sizeof(mem_a_t));
	if (!mem_a) {
		printf("Failed to allocate memory!\n");
		exit(0);
	}
	while (1) {
		char com[1000];
		scanf("%s", com);
		if (strcmp(com, "INIT_HEAP") == 0) {
			size_t start_adress;
			int nr_lists, bytes_per_list, reconstruction_type;
			scanf("%lx", &start_adress);
			scanf("%d", &nr_lists);
			scanf("%d", &bytes_per_list);
			scanf("%d", &reconstruction_type);
			heap =
		init_heap(start_adress, nr_lists, bytes_per_list, reconstruction_type);
			mem_a = initialize_allocated(mem_a, heap);
		} else if (strcmp(com, "MALLOC") == 0) {
			int nr_bytes;
			scanf("%d", &nr_bytes);
			my_malloc(heap, mem_a, nr_bytes);
			sort_lists_by_bytes_heap(heap);
		} else if (strcmp(com, "FREE") == 0) {
			heap->free_calls++;
			size_t address;
			scanf("%lx", &address);
			my_free(mem_a, heap, address);
			sort_lists_by_bytes_heap(heap);
		} else if (strcmp(com, "READ") == 0) {
			size_t address;
			scanf("%lx", &address);
			int nr_bytes;
			scanf("%d", &nr_bytes);
			my_read(mem_a, address, nr_bytes, heap);
			printf("\n");
		} else if (strcmp(com, "WRITE") == 0) {
			size_t address;
			scanf("%lx", &address);
			char s[650];
			int nr_bytes;
			scanf(" \"%[^\"]\" %d", s, &nr_bytes);
			s[strlen(s)] = '\0';
			if (nr_bytes > (int)strlen(s))
				nr_bytes = strlen(s);
			my_write(mem_a, address, s, nr_bytes, heap);
		} else if (strcmp(com, "DUMP_MEMORY") == 0) {
			dump_memory(heap, mem_a);
		} else if (strcmp(com, "DESTROY_HEAP") == 0) {
			destroy_heap(heap, mem_a);
			exit(0);
		}
	}
	return 0;
}
