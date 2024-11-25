#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dll_node_t {
	void *data;
	size_t adress;
	struct dll_node_t *prev, *next;
	int is_written;
	int bytes_written;
	int bytes_total;
} dll_node_t;

typedef struct dl_list_t {
	dll_node_t *head;
	int data_size;
	int nodes_count;
	int list_bytes;
	dll_node_t *tail;

} dl_list_t;

typedef struct {
	dl_list_t **lists;
	int nr_lists;
	int bytes_per_list;
	int reconstruction_type;
	int free_memory;
	int total_memory;
	int free_calls;
} sfl;

typedef struct {
	dl_list_t **lists;
	int mall_calls;
	int nr_blocks;
	int nr_lists;
	int all_memory;
	int fragmentations;
} mem_a_t;

int cmpfunc(const void *a, const void *b);

void dll_create(dl_list_t *list);

dll_node_t *dll_get_nth_node(dl_list_t *list, int n);

void dll_add_nth_node(dl_list_t *list, int n, size_t adress);


void add_nth_node_bytes(dl_list_t *list, int n, size_t adress, int bytes_total);

void dll_remove_first_n(dl_list_t **list);

void dll_remove_nth_node(dl_list_t **list, int n);

void dll_free(dl_list_t **pp_list);

void sort_lists_by_bytes_heap(sfl *heap);
sfl *init_heap(size_t start_adress, int nr_lists, int bytes_per_list, int rtype);

mem_a_t *initialize_allocated(mem_a_t *mem_a, sfl *heap);


void dump_memory(sfl *heap, mem_a_t *mem_a);

void sorting_addr(sfl **heap, int index);

void my_malloc(sfl *heap, mem_a_t *mem_a, int nr_bytes);

void destroy_heap(sfl *heap, mem_a_t *mem_a);

int is_allocated(mem_a_t *mem_a, size_t adress);

void my_free(mem_a_t *mem_a, sfl *heap, size_t address);

int list_where_addr(mem_a_t *mem_a, size_t addr);

int sufficient_allocated(mem_a_t *mem_a, int nr_bytes, size_t addr);

void my_write(mem_a_t *mem_a, size_t address, char *s, int nr_bytes, sfl *heap);

void my_read(mem_a_t *mem_a, size_t address, int nr_bytes, sfl *heap);