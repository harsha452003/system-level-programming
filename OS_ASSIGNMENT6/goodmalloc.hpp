#ifndef GOODMALLOC_H
#define GOODMALLOC_H

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>

using namespace std;
#define PG_SIZE 200000
#define ST_SIZE 300000

typedef struct list{
    int element;
    struct list *next;
    struct list *prev;
}list;

typedef struct memory{
    int *start;
    int *end;
    size_t words;
    size_t free_words;
    int num_free_blocks;
    size_t max_free_block_size;

    int init(size_t size); 
    int *find_free_block(size_t size);
    void allocate_block(int *block, size_t size);
    void free_block(int *block);
    int getoffset(int *block);
    int *getaddress(int offset);
    void print_mem();
    
}memory;

typedef struct page_table_entry{
    unsigned int address : 30;
    unsigned int valid : 1;
    unsigned int scope : 1;

    void init();

    void print();

}page_table_entry;


typedef struct page_table{
    page_table_entry table_entry[PG_SIZE];
    int head;
    int tail;
    size_t size;

    void init();
    int insert(unsigned int address);
    int remove(int index);
    void print();
}page_table;

typedef struct stack_entry{
    char* name;
    int length;
    int index;

}stack_entry;

typedef struct Stack{
    stack_entry stack[ST_SIZE];
    size_t size;

    void init();
    stack_entry top();
    int push(stack_entry index);
    stack_entry pop();
    void print();
    int find(char* name);

}Stack;


//Global page table
extern page_table *global_page_table;
//Global stack
extern Stack *global_stack;
//Global memory
extern memory *global_memory;

void scope_start();                              //Starts a new scope when the function is called which have lists to be created in it
void createMem(size_t size);                        //Creates a memory of size size               
void createList(size_t size, char* list_name);      //Creates a list of size size and name list_name  
int getVal(char* name, int offset);                 //Returns the value at offset offset in the list with name name
void assignVal(char* name, int offset, int value);  //Assigns value value to the offset offset in the list with name name
void freeElem( char* list_name = NULL);             //Frees the list with name list_name or all the lists in the current scope if list_name is NULL

#endif