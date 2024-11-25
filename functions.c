#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sfl.h"



int cmpfunc(const void *a, const void *b)
{
	dl_list_t *list_a = *(dl_list_t **)a;
	dl_list_t *list_b = *(dl_list_t **)b;
	if (list_a->list_bytes < list_b->list_bytes)
		return -1;
	else if (list_a->list_bytes > list_b->list_bytes)
		return 1;
	else
		return 0;
}

void dll_create(dl_list_t *list)
{
	list = malloc(sizeof(dl_list_t));
	list->head = NULL;
	list->tail = NULL;
}

dll_node_t *dll_get_nth_node(dl_list_t *list, int n)
{
	// if (n >= list->nodes_count)
	//     n %= list->nodes_count;

	dll_node_t *node = list->head;
	for (int i = 0; i < n; i++)
		node = node->next;

	return node;
}

void dll_add_nth_node(dl_list_t *list, int n, size_t adress)
{
	// if (n > (*list)->nodes_count)
	// {
	//     n = (*list)->nodes_count;
	// }

	dll_node_t *new_node = (dll_node_t *)malloc(sizeof(dll_node_t));
	// new_node->data = malloc(list->data_size);
	// memcpy(new_node->data, data, list->data_size);
	// const void *data,

	if (!list->head) {
		(list)->head = new_node;
		(list)->tail = new_node;
		new_node->prev = NULL;
		new_node->next = NULL;
	} else if (n == 0) {
		new_node->next = NULL;
		new_node->prev = (list)->tail;
		(list)->tail->next = new_node;
		(list)->tail = new_node;
	} else {
		dll_node_t *current = (list)->head;
		for (int i = 0; i < n - 1; ++i)
			current = current->next;
		new_node->next = current->next;
		new_node->prev = current;
		if (current->next)
			current->next->prev = new_node;
		current->next = new_node;
	}
	new_node->adress = adress;
	(list)->nodes_count++;
	new_node->is_written = 0;
}

void add_nth_node_bytes(dl_list_t *list, int n, size_t adress, int bytes_total)
{
	// if (n > (*list)->nodes_count)
	// {
	//     n = (*list)->nodes_count;
	// }

	dll_node_t *new_node = (dll_node_t *)malloc(sizeof(dll_node_t));
	// new_node->data = malloc(list->data_size);
	// memcpy(new_node->data, data, list->data_size);
	// const void *data,

	if (!list->head) {
		(list)->head = new_node;
		(list)->tail = new_node;
		new_node->prev = NULL;
		new_node->next = NULL;
	} else if (n == 0) {
		new_node->next = NULL;
		new_node->prev = (list)->tail;
		(list)->tail->next = new_node;
		(list)->tail = new_node;
	} else {
		dll_node_t *current = (list)->head;
		for (int i = 0; i < n - 1; ++i)
			current = current->next;
		new_node->next = current->next;
		new_node->prev = current;
		if (current->next)
			current->next->prev = new_node;
		current->next = new_node;
	}
	new_node->adress = adress;
	new_node->bytes_total = bytes_total;
	(list)->nodes_count++;
	new_node->is_written = 0;
}

void dll_remove_first_n(dl_list_t **list)
{
	dll_node_t *aux = (*list)->tail;
	if (aux->prev) {
		aux->prev->next = NULL;
		(*list)->tail = aux->prev;
	} else {
		(*list)->tail = NULL;
		(*list)->head = NULL;
	}
	free(aux);
}

void dll_remove_nth_node(dl_list_t **list, int n)
{
	if (n > (*list)->nodes_count - 1)
		n = (*list)->nodes_count - 1;
	dll_node_t *back, *curr;
	curr = (*list)->head;
	back = NULL;
	while (n > 0) {
		back = curr;
		curr = curr->next;
		n--;
	}
	if (!back)
		(*list)->head = curr->next;
	else
		back->next = curr->next;
	(*list)->nodes_count--;
	// free(back);
	free(curr);
}

void dll_free(dl_list_t **pp_list)
{
	dl_list_t *list = *pp_list;
	dll_node_t *current = list->head;
	dll_node_t *next = NULL;

	while (current) {
		next = current->next;
		if (current->is_written == 1)
			free(current->data);
		free(current);
		current = next;
	}

	free(list);
	*pp_list = NULL;
}

void sort_lists_by_bytes_heap(sfl *heap)
{
	qsort(heap->lists, heap->nr_lists, sizeof(dl_list_t *), cmpfunc);
}

sfl *init_heap(size_t start_adress, int nr_lists, int bytes_per_list, int rtype)
{
	sfl *heap = malloc(sizeof(sfl));

	if (!heap) {
		printf("Failed to allocate heap!");
		exit(-1);
	}

	int aux = 8;
	heap->nr_lists = nr_lists;
	heap->bytes_per_list = bytes_per_list;
	heap->reconstruction_type = rtype;
	heap->lists = malloc(nr_lists * sizeof(dl_list_t *));
	if (!heap->lists) {
		printf("Failed to allocate heap!");
		free(heap);
		exit(-1);
	}
	for (int i = 0; i < nr_lists; i++) {
		heap->lists[i] = malloc(sizeof(dl_list_t));
		heap->lists[i]->head = NULL;
		heap->lists[i]->tail = NULL;
		if (!heap->lists[i]) {
			printf("Failed to allocate list");
			for (int j = i - 1; j >= 0; j--)
				free(heap->lists[j]);
			free(heap->lists);
		}
		heap->lists[i]->nodes_count = 0;
		heap->lists[i]->list_bytes = aux;
		for (int j = 0; j < bytes_per_list / aux; j++) {
			dll_node_t *node = (dll_node_t *)malloc(sizeof(dll_node_t));
			if (!node) {
				printf("Failed to allocate node");
				exit(-1);
			}
			node->adress = start_adress;
			node->is_written = 0;
			start_adress = start_adress + aux;
			heap->lists[i]->nodes_count++;
			node->next = heap->lists[i]->head;
			if (j != 0)
				heap->lists[i]->head->prev = node;
			if (j == 0)
				heap->lists[i]->tail = node;
			node->prev = NULL;
			heap->lists[i]->head = node;
		}
		aux = aux * 2;
	}
	heap->free_calls = 0;
	heap->total_memory = heap->nr_lists * heap->bytes_per_list;
	heap->free_memory = heap->nr_lists * heap->bytes_per_list;
	return heap;
}

mem_a_t *initialize_allocated(mem_a_t *mem_a, sfl *heap)
{
	mem_a->all_memory = 0;
	mem_a->mall_calls = 0;
	mem_a->nr_blocks = 0;
	mem_a->nr_lists = heap->nr_lists;
	mem_a->fragmentations = 0;

	mem_a->lists =
						malloc(mem_a->nr_lists *
						sizeof(dl_list_t *));
	if (!(mem_a)->lists) {
		printf("Failed to allocate memory for lists!\n");
		exit(0);
	}

	for (int i = 0; i < mem_a->nr_lists; i++) {
		mem_a->lists[i] = malloc(sizeof(dl_list_t));
		mem_a->lists[i]->head = NULL;
		mem_a->lists[i]->tail = NULL;
		if (!(mem_a)->lists[i]) {
			printf("Failed to allocate memory for list %d!\n", i);
			exit(0);
		}
	}
	for (int i = 0; i < mem_a->nr_lists; i++) {
		mem_a->lists[i]->list_bytes = heap->lists[i]->list_bytes;
		mem_a->lists[i]->nodes_count = 0;
	}
	for (int i = 0; i < mem_a->nr_lists; i++) {
		dll_node_t *tmp = mem_a->lists[i]->head;
		while (tmp) {
			tmp->is_written = 0;
			tmp = tmp->next;
		}
	}
	return mem_a;
}

void sort_lists_by_address(dl_list_t **lists)
{
		// Sortare noduri din lista curentă după adrese
		dll_node_t *current = (*lists)->head;
		// dll_node_t *temp = NULL;
		while (current) {
			dll_node_t *next = current->next;
			while (next) {
				if (current->adress > next->adress) {
					// Schimbare de locuri între noduri
					size_t aux = current->adress;
					current->adress = next->adress;
					next->adress = aux;
					int aux_bytes = current->bytes_total;
					current->bytes_total = next->bytes_total;
					next->bytes_total = aux_bytes;
				}
				next = next->next;
			}
			current = current->next;
		}
	}

void dump_memory(sfl *heap, mem_a_t *mem_a)
{
	printf("+++++DUMP+++++\n");
	printf("Total memory: %d bytes\n", heap->total_memory);
	printf("Total allocated memory: %d bytes\n", mem_a->all_memory);
	printf("Total free memory: %d bytes\n", heap->free_memory);
	int sum = 0;
	for (int i = 0; i < heap->nr_lists; i++)
		sum = sum + heap->lists[i]->nodes_count;

	printf("Free blocks: %d\n", sum);
	printf("Number of allocated blocks: %d\n", mem_a->nr_blocks);
	printf("Number of malloc calls: %d\n", mem_a->mall_calls);
	printf("Number of fragmentations: %d\n", mem_a->fragmentations);
	printf("Number of free calls: %d\n", heap->free_calls);
	sort_lists_by_bytes_heap(heap);
	for (int i = 0; i < heap->nr_lists; i++) {
		dll_node_t *current = heap->lists[i]->head;
		while (current) {
			dll_node_t *nextnode = current->next;
			while (nextnode) {
				if (current->adress < nextnode->adress) {
					size_t temp = current->adress;
					current->adress = nextnode->adress;
					nextnode->adress = temp;
				}
				nextnode = nextnode->next;
			}
			current = current->next;
		}
	}
	for (int i = 0; i < heap->nr_lists; ++i) {
		if (heap->lists[i]->nodes_count != 0) {
			int a = heap->lists[i]->list_bytes;
			int b = heap->lists[i]->nodes_count;
			printf("Blocks with %d bytes - %d free block(s) :", a, b);
			dll_node_t *node;
			node = heap->lists[i]->tail;
			while (node) {
				printf(" 0x%lx", node->adress);
				node = node->prev;
			}
			printf("\n");
		}
	}
	dl_list_t *aux = malloc(sizeof(dl_list_t));
	aux->head = NULL;
	aux->tail = NULL;
	aux->nodes_count = 0;
	for (int i = 0; i < mem_a->nr_lists; i++) {
		dll_node_t *tmp = mem_a->lists[i]->head;
		while (tmp) {
			tmp->bytes_total = mem_a->lists[i]->list_bytes;
			add_nth_node_bytes(aux, 0, tmp->adress, tmp->bytes_total);
			tmp = tmp->next;
		}
	}
	sort_lists_by_address(&aux);
	printf("Allocated blocks :");
		dll_node_t *tmp = aux->head;
		while (tmp) {
			printf(" (0x%lx - %d)", tmp->adress, tmp->bytes_total);
			tmp = tmp->next;
		}
	dll_free(&aux);

	printf("\n-----DUMP-----\n");
}

void sorting_addr(sfl **heap, int index)
{
	
		dll_node_t *current = (*heap)->lists[index]->head;
		while (current) {
			dll_node_t *nextnode = current->next;
			while (nextnode) {
				if (current->adress < nextnode->adress) {
					size_t temp = current->adress;
					current->adress = nextnode->adress;
					nextnode->adress = temp;
				}
				nextnode = nextnode->next;
			}
			current = current->next;
		}
	}

void my_malloc(sfl *heap, mem_a_t *mem_a, int nr_bytes)
{
	mem_a->mall_calls++;
	int i;
	for (i = heap->nr_lists - 1; i > 0; i--) {
		if (heap->lists[i]->nodes_count != 0)
			break;
	}
	if (heap->lists[i]->list_bytes < nr_bytes) {
		printf("Out of memory\n");
		mem_a->mall_calls--;

		return;
	}
	dll_node_t *selected_node = NULL;

	for (int i = 0; i < heap->nr_lists; i++) {
		int a = heap->lists[i]->list_bytes;
		int b = heap->lists[i]->nodes_count;
		if (a == nr_bytes && b != 0) {
			selected_node = heap->lists[i]->tail;
			if (selected_node) {
				// selected_node->data = malloc(nr_bytes);
				selected_node->is_written = 0;
				selected_node->bytes_written = 0;
				int found = 0;
				for (int j = 0; j < mem_a->nr_lists; j++) {
					if (mem_a->lists[j]->list_bytes == nr_bytes) {
						size_t addr_aux = selected_node->adress;
						dll_add_nth_node(mem_a->lists[j], 0, addr_aux);
						found = 1;
						break;
					}
				}
				if (!found) {
					int a = mem_a->nr_lists;
					mem_a->lists =
					realloc(mem_a->lists, (a + 1) * sizeof(dl_list_t *));
					mem_a->nr_lists++;
					a++;
					mem_a->lists[a - 1] = malloc(sizeof(dl_list_t));
					mem_a->lists[a - 1]->head = NULL;
					mem_a->lists[a - 1]->tail = NULL;
					if (!mem_a->lists[a - 1]) {
						printf("Failed to allocate memory for a new list!\n");
						exit(0);
					}
					mem_a->lists[a - 1]->list_bytes = nr_bytes;
					mem_a->lists[a - 1]->nodes_count = 0;
					size_t aux_adr = selected_node->adress;
					dll_add_nth_node(mem_a->lists[a - 1], 0, aux_adr);
					// free(selected_node->data);
				}
			}
			dll_remove_first_n(&heap->lists[i]);
			heap->free_memory -= nr_bytes;
			heap->lists[i]->nodes_count--;
			mem_a->nr_blocks++;
			mem_a->all_memory += nr_bytes;
			return;
		} else if (heap->lists[i]->list_bytes > nr_bytes) {
			selected_node = heap->lists[i]->tail;
			if (selected_node) {
				// selected_node->data = malloc(nr_bytes);
				selected_node->is_written = 0;
				selected_node->bytes_written = 0;
				int found = 0;
				for (int j = 0; j < mem_a->nr_lists; j++) {
					if (mem_a->lists[j]->list_bytes == nr_bytes) {
						size_t addr = selected_node->adress;
						dll_add_nth_node(mem_a->lists[j], 0, addr);
						// free(selected_node->data);
						found = 1;
						break;
					}
				}

				if (!found) {
					int a = mem_a->nr_lists;
					mem_a->lists =
					realloc(mem_a->lists, (a + 1) * sizeof(dl_list_t *));
					a++;
					mem_a->nr_lists++;
					mem_a->lists[a - 1] = malloc(sizeof(dl_list_t));
					mem_a->lists[a - 1]->head = NULL;
					mem_a->lists[a - 1]->tail = NULL;

					if (!mem_a->lists[mem_a->nr_lists - 1]) {
						printf("Failed to allocate memory for a new list!\n");
						exit(0);
					}

					mem_a->lists[mem_a->nr_lists - 1]->list_bytes = nr_bytes;
					mem_a->lists[mem_a->nr_lists - 1]->nodes_count = 0;
					size_t addr = selected_node->adress;
				dll_add_nth_node(mem_a->lists[mem_a->nr_lists - 1], 0, addr);
					// free(selected_node->data);
				}

		found = 0;
		int j;
for (j = 0; j < heap->nr_lists; j++) {
	if (heap->lists[j]->list_bytes == heap->lists[i]->list_bytes - nr_bytes) {
		selected_node->adress += nr_bytes;
		dll_add_nth_node(heap->lists[j], 0, selected_node->adress);
		sorting_addr(&heap, j);
		found = 1;
		break;
		}
	}

				if (!found) {
					a = heap->nr_lists;
					heap->lists =
					realloc(heap->lists, (a + 1) * sizeof(dl_list_t *));
					heap->nr_lists++;
					a++;
					heap->lists[a - 1] = malloc(sizeof(dl_list_t));
					heap->lists[a - 1]->head = NULL;
					heap->lists[a - 1]->tail = NULL;
					if (!heap->lists[a - 1]) {
						printf("Failed to allocate memory for a new list!\n");
						exit(0);
					}
					int b = heap->lists[i]->list_bytes;
					heap->lists[a - 1]->list_bytes = b - nr_bytes;
					selected_node->adress += nr_bytes;
					heap->lists[a - 1]->nodes_count = 0;
					size_t addr = selected_node->adress;
					dll_add_nth_node(heap->lists[a - 1], 0, addr);
					sorting_addr(&heap, a - 1);
				}
				mem_a->fragmentations++;
				heap->free_memory -= nr_bytes;
				heap->lists[i]->nodes_count--;
				mem_a->nr_blocks++;
				mem_a->all_memory += nr_bytes;
				dll_remove_first_n(&heap->lists[i]);
				return;
			}
		}
	}
}

void destroy_heap(sfl *heap, mem_a_t *mem_a)
{
	for (int i = 0; i < heap->nr_lists; i++) {
		dll_node_t *current_node = heap->lists[i]->head;
		dll_node_t *next_node = NULL;
		while (current_node) {
			next_node = current_node->next;
			if (current_node->is_written == 1)
				free(current_node->data);
			free(current_node);
			current_node = next_node;
		}

		free(heap->lists[i]);
	}

	free(heap->lists);
	free(heap);

	for (int i = 0; i < mem_a->nr_lists; i++) {
		dll_node_t *current_node = mem_a->lists[i]->head;
		dll_node_t *next_node = NULL;
		while (current_node) {
			next_node = current_node->next;
			if (current_node->is_written == 1)
				free(current_node->data);
			free(current_node);
			current_node = next_node;
		}

		free(mem_a->lists[i]);
	}

	free(mem_a->lists);
	free(mem_a);
}

int is_allocated(mem_a_t *mem_a, size_t adress)
{
	dll_node_t *tmp;
	int aux = 0;
	for (int i = 0; i < mem_a->nr_lists; i++) {
		tmp = mem_a->lists[i]->head;
		while (tmp) {
			if (tmp->adress == adress) {
				aux = 1;
				break;
			}
			tmp = tmp->next;
		}
	}
	return aux;
}

void my_free(mem_a_t *mem_a, sfl *heap, size_t address)
{
	int is_block_all = is_allocated(mem_a, address);
	if (is_block_all == 0) {
		printf("Invalid free\n");
		heap->free_calls--;
	} else if (is_block_all != 0) {
		dll_node_t *tmp;
		size_t node_to_add_address;
		int nr_bytes;
		int i;
		int ok = 1;
		int aux = 0;

		for (i = 0; i < mem_a->nr_lists && ok; i++) {
			tmp = mem_a->lists[i]->head;
			while (tmp) {
				if (tmp->adress == address) {
					node_to_add_address = tmp->adress;
					nr_bytes = mem_a->lists[i]->list_bytes;
					if (tmp->is_written == 1)
						free(tmp->data);
					dll_remove_nth_node(&mem_a->lists[i], aux);
					ok = 0;
					break;
				}
				tmp = tmp->next;
				aux++;
			}
		}
		i--;
		int found = 0;
		int j;
		for (j = 0; j < heap->nr_lists; j++) {
			if (heap->lists[j]->list_bytes == nr_bytes) {
				dll_add_nth_node(heap->lists[j], 0, node_to_add_address);
				sorting_addr(&heap, j);
				found = 1;
				break;
			}
		}

		if (!found) {
			int a = heap->nr_lists;
			heap->lists = realloc(heap->lists, (a + 1) * sizeof(dl_list_t *));
			a++;
			heap->nr_lists++;
			heap->lists[a - 1] = malloc(sizeof(dl_list_t));
			heap->lists[a - 1]->head = NULL;
			heap->lists[a - 1]->tail = NULL;

			if (!heap->lists[heap->nr_lists - 1]) {
				printf("Failed to allocate memory for a new list!\n");
				exit(0);
			}

			heap->lists[a - 1]->list_bytes = nr_bytes;
			heap->lists[a - 1]->nodes_count = 0;
			dll_add_nth_node(heap->lists[a - 1], 0, node_to_add_address);
			sorting_addr(&heap, a - 1);
		}
		mem_a->nr_blocks--;
		mem_a->all_memory -= nr_bytes;
		heap->free_memory += nr_bytes;

		
}
}

int list_where_addr(mem_a_t *mem_a, size_t addr)
{
	dll_node_t *tmp;
	int i;
	int aux = 0;
	for (i = 0; i < mem_a->nr_lists && aux == 0; i++) {
		tmp = mem_a->lists[i]->head;
		while (tmp) {
			if (tmp->adress == addr) {
				aux = 1;
				break;
			}
			tmp = tmp->next;
		}
	}
	if (aux == 1)
		return i - 1;
	else
		return -1;
}

int sufficient_allocated(mem_a_t *mem_a, int nr_bytes, size_t addr)
{
	int list_with_addr = list_where_addr(mem_a, addr);
	if (list_with_addr == -1)
		return 0;
	int block_size = mem_a->lists[list_with_addr]->list_bytes;
	int space_to_write = block_size;
	size_t next_addr = addr + block_size;
	for (int i = 0; i < mem_a->lists[list_with_addr]->nodes_count; i++) {
		if (list_where_addr(mem_a, next_addr) != -1) {
			space_to_write += block_size;
			next_addr += block_size;
		} else {
			break;
		}
	}
	if (space_to_write < nr_bytes)
		return 1;
	else
		return 2;
}

void my_write(mem_a_t *mem_a, size_t address, char *s, int nr_bytes, sfl *heap)
{
	int tmp = sufficient_allocated(mem_a, nr_bytes, address);
	if (tmp == 0) {
		printf("Segmentation fault (core dumped)\n");
		dump_memory(heap, mem_a);
		destroy_heap(heap, mem_a);
		exit(0);
	} else if (tmp == 1) {
		printf("Segmentation fault (core dumped)\n");
		dump_memory(heap, mem_a);
		destroy_heap(heap, mem_a);
		exit(0);
	}

	char aux[650];
	strcpy(aux, s);
	int a = list_where_addr(mem_a, address);
	int block_size = mem_a->lists[a]->list_bytes;
	size_t next_addr = address;
	for (int i = nr_bytes; i > 0; i -= block_size) {
		dll_node_t *tmp = mem_a->lists[a]->head;
		while (tmp) {
			if (tmp->adress == next_addr)
				break;
			tmp = tmp->next;
		}
		if (block_size <= i) {
			if (tmp->is_written == 0) {
				tmp->data = malloc(block_size + 1);
				tmp->is_written = 1;
			} else {
				free(tmp->data);
				tmp->data = malloc(block_size + 1);
			}

			tmp->bytes_written = block_size;
			// printf("%d\n",tmp->bytes_written);
			strncpy(aux, s, block_size);
			aux[block_size] = '\0';
			memcpy(tmp->data, aux, block_size);
			// printf("%s\n", (char *)tmp->data);
			strcpy(aux, s + block_size);
			strcpy(s, aux);
		} else {
			if (tmp->is_written == 1) {
				char idk[650];
				strncpy(idk, tmp->data, block_size + 1);
				idk[block_size] = '\0';
				strcat(s, idk + nr_bytes);
				free(tmp->data);
			}
			s[strlen(s)] = '\0';
			tmp->data = malloc(strlen(s) + 1);
			tmp->is_written = 1;
			// printf("<%s>\n", s);
			memcpy(tmp->data, s, strlen(s) + 1);
			tmp->bytes_written = strlen(s);
			// printf("%d\n",tmp->bytes_written);
			i = -2;
		}
		next_addr += block_size;
		a = list_where_addr(mem_a, next_addr);
		if (a == -1)
			break;
	}
}

void my_read(mem_a_t *mem_a, size_t address, int nr_bytes, sfl *heap)
{
	int space_to_read = 0;
			int b = list_where_addr(mem_a, address);
			if (b == -1) {
				printf("Segmentation fault (core dumped)\n");
				dump_memory(heap, mem_a);
				destroy_heap(heap, mem_a);
				exit(0);
			}
			int size = mem_a->lists[b]->list_bytes;
			size_t addr_next = address;
			for (int i = 0; i < mem_a->lists[b]->nodes_count; i++) {
				dll_node_t *tmp = mem_a->lists[b]->head;
				while (tmp) {
					int a = tmp->is_written;
					if (list_where_addr(mem_a, addr_next) != -1 && a == 1) {
						space_to_read += tmp->bytes_written;
						break;
					}
					tmp = tmp->next;
				}
				addr_next += size;
				b = list_where_addr(mem_a, addr_next);
				if (b == -1)
					break;
			}
			if (nr_bytes > space_to_read) {
				printf("Segmentation fault (core dumped)\n");
				dump_memory(heap, mem_a);
				destroy_heap(heap, mem_a);
				exit(0);
			}
	int tmp = sufficient_allocated(mem_a, nr_bytes, address);
	if (tmp == 0) {
		printf("Segmentation fault (core dumped)\n");
		dump_memory(heap, mem_a);
		destroy_heap(heap, mem_a);
		exit(0);
	}
	int a = list_where_addr(mem_a, address);
	int block_size = mem_a->lists[a]->list_bytes;
	size_t next_addr = address;
	for (int i = nr_bytes; i > 0; i -= block_size) {
		dll_node_t *tmp = mem_a->lists[a]->head;
		while (tmp) {
			if (tmp->adress == next_addr)
				break;
			tmp = tmp->next;
		}
		if (block_size <= i) {
			char aux[650];
			memcpy(aux, tmp->data, block_size);
			aux[block_size] = '\0';
			printf("%s", aux);
		} else {
			char aux[650];
			memcpy(aux, tmp->data, i);
			aux[i] = '\0';
			printf("%s", aux);
			i = -2;
		}
		next_addr += block_size;
		a = list_where_addr(mem_a, next_addr);
	}
}
