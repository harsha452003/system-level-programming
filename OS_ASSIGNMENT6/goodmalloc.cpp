#include "goodmalloc.hpp"

FILE *fp;

int memory::init(size_t size){      //size in bytes total required

    size = ((size + 3) / 4) * 4; //round up to nearest multiple of 4
    start = (int *)malloc(size);
    if(start == NULL){
        return -1;
    }
    end = start + (size/4);
    words = size/4;
    free_words = size/4;
    num_free_blocks = 1;
    max_free_block_size = size/4;

    *start = (size/4) << 1;                     //storing no of words available in first 31 bits in 1st word
    *(start + (size/4)-1) = (size/4) << 1;     //storing no of words available in last 31 bits in last word

    printf("Memory Segment Created and initialized with %lu words\n", words);
    printf("Start Address: %p\n", start);

    fp = fopen("memory_footprint.txt", "w");

    return 0;
}

int * memory::find_free_block(size_t size){      //size in words  

    printf("Finding free block of size %lu words\n", size);

    int *curr = start;
    while(curr < end){
        if((long unsigned int)(*curr & 1) == 0 && (long unsigned int)(*curr >> 1) >= (size+2))
        {
            printf("Found free block of size %lu words at address %p\n", (long unsigned int)(*curr >> 1), curr);
            return curr;
        }
        curr += (*curr >> 1);
    }
    return NULL;
}
void memory::allocate_block(int *block, size_t size){      //size in words and block is address of free block

    printf("Allocating block of size %lu words at address %p\n", size, block);

    size+=2;

    unsigned long int old_size = *block >> 1;
    *block = (size << 1) | 1;       //storing size of block in 31 bits and 1 in LSB
    *(block + size - 1) = (size << 1) | 1;     //storing size of block in 31 bits and 1 in LSB

    if(old_size > size)
    {
        int *new_block = block + size;
        *new_block = (old_size - size) << 1;
        *(new_block + (old_size - size) - 1) = (old_size - size) << 1;
    }

    free_words -= size;

    if(size == old_size)
        num_free_blocks--;
    
    if(old_size == max_free_block_size)
        max_free_block_size -= size;

    max_free_block_size = max(max_free_block_size, free_words/(num_free_blocks+1));

    fprintf(fp, "Unallocated Memory Right Now: %ld\n", free_words);
    printf("Block allocated\n");
}


void memory::free_block(int *block){        //block is address of allocated block

    printf("Freeing block at address %p\n", block);

    int size = *block >> 1;
    *block = *block & ~1;       //storing size of block in 31 bits and 0 in LSB
    *(block + size - 1) = *(block + size - 1) & ~1;     //storing size of block in 31 bits and 0 in LSB

    free_words += size;
    num_free_blocks++;

    int *next_block = block + size;
    if(next_block < end && (*next_block & 1) == 0){
        printf("Coalescing with next block at address %p\n", next_block);
        size += *next_block >> 1;
        *block = size << 1;
        *(block + size - 1) = size << 1;
        num_free_blocks--;
    }

    //if previous block is free
    if ((block != start) && (*(block-1) & 1) == 0)
    {
        int prev_size = *(block-1) >> 1;
        printf("Coalescing with previous block at address %p\n", (block - prev_size));
        *(block - prev_size) = (prev_size + size) << 1;
        *(block + size - 1) = (prev_size + size) << 1;
        num_free_blocks--;
        size += prev_size;
    }

    max_free_block_size = max(max_free_block_size, (size_t)size);
    max_free_block_size = max(max_free_block_size, free_words/(num_free_blocks+1));

    fprintf(fp, "Unallocated Memory Right Now: %ld\n", free_words);
    printf("Block freed\n");
}


int memory::getoffset(int *block){
    return (int)(block - start);
}


int * memory::getaddress(int offset){
    return start + offset;
}


void memory::print_mem(){

    printf("Printing Memory Segment\n");
    printf("Start Address: %p\n", start);
    printf("End Address: %p\n", end);
    printf("Total Words: %lu\n", words);
    printf("Free Words: %lu\n", free_words);
    printf("Number of Free Blocks: %d\n", num_free_blocks);
    printf("Max Free Block Size: %lu\n", max_free_block_size);

    int *curr = start;
    while(curr < end){
        printf("Address: %ld\tSize: %d\tAllocated: %d\n", (curr-start), *curr >> 1, *curr & 1);
        curr += (*curr >> 1);
    }
}


void page_table_entry::init(){
    address = 0;
    valid = 0;
    scope = 0;
}


void page_table_entry::print()
{
    printf("Address: %u\tValid: %d\tScope: %d\n", address, valid, scope);
}


void page_table::init()
{
    head = 0;
    tail = PG_SIZE-1;
    size = 0;
    for(int i=0; i<PG_SIZE; i++)
    {
        table_entry[i].init();
        table_entry[i].address = i+1;
    }

    printf("Page Table Initialized\n");
}
int page_table::insert(unsigned int address)
{
    if(size == PG_SIZE)
    {
        printf("Page Table Full\n");
        return -1;
    }

    int index = head;
    int next_free = table_entry[head].address;
    table_entry[index].address = address;
    table_entry[index].valid = 1;
    table_entry[index].scope = 1;
    size++;

    if(size < PG_SIZE)
    {
        head = next_free;
    }
    else
    {
        head = PG_SIZE;
        tail = PG_SIZE;
    }
    printf("Page Table Entry Inserted\n");
    return index;
}
int page_table::remove(int index)
{
    if(size == 0)
    {
        printf("Page Table Empty\n");
        return -1;
    }
    else if(table_entry[index].valid == 0)
    {
        printf("Page Table Entry Invalid\n");
        return -1;
    }

    table_entry[index].valid = 0;
    table_entry[tail].address = index;
    tail = index;
    size--;

    if(size == PG_SIZE-1)
    {
        head = tail;
    }
    printf("Page Table Entry Removed\n");
    return 0;
}
void page_table::print()
    {
        printf("\nPrinting Page Table\n");

        printf("Head: %d\tTail: %d\tSize: %lu\n", head, tail, size);
        for(int i=0; i<PG_SIZE; i++)
        {
            if(table_entry[i].valid == 1)
            {
                printf("Index: %d\t", i);
                table_entry[i].print();
            }
        }
    }
//Global stack structure



void Stack::init(){
    size = 0;
    for(int i=0;i<ST_SIZE;i++){
        stack[i].name=(char *)malloc(10*sizeof(char));
        stack[i].index=-2;
        stack[i].length=0;
    }
    printf("Stack Initialized\n");
}

stack_entry Stack::top(){
    if(size == 0)
    {
        printf("Stack Empty\n");
        return (stack_entry){0};
    }
    return stack[size-1];
}
int Stack::push(stack_entry index){
    if(size == ST_SIZE)
    {
        printf("Stack Full\n");
        return -1;
    }
    stack[size] = index;
    size++;
    printf("Pushed %d to stack\n", index.index);
    return 0;
}
stack_entry Stack::pop(){
    if(size == 0)
    {
        printf("Stack Empty\n");
        return (stack_entry){0};
    }
    size--;
    printf("Popped %s from stack\n", stack[size].name);
    return stack[size];
}

void Stack::print()
{
    printf("Printing Global Stack\n");
    printf("Current Size: %lu\n", size);
    for(long unsigned int i=0; i<size; i++)
    {
        printf("%d\t", stack[i].index);
    }
    printf("\n");
}

int Stack::find(char *name){
    for(int i=size-1;i>=0;i--){
        if(stack[i].index == -3)
            return 0;
        if(strcmp(stack[i].name,name)==0)
            return stack[i].index;
    }
    return -2;
}

//Global page table
page_table *global_page_table;
//Global stack
Stack *global_stack;
//Global memory
memory *global_memory;

void scope_start(){
    stack_entry *temp = (stack_entry *)malloc(sizeof(stack_entry));
    temp->index = -3;
    temp->length = 0;
    temp->name = (char *)malloc(100*sizeof(char));
    temp->name = NULL;
    global_stack->push(*temp);
}

void createMem(size_t size)     //size in bytes
{
    printf("Creating Memory Segment\n");

    size = (size_t)(size*1.25);
    size = ((size + 3) >> 2) << 2;

    global_memory = (memory*)malloc(sizeof(memory));
    if(global_memory->init(size) == -1)
    {
        printf("Error in initializing memory\n");
        return;
    }

    global_page_table = (page_table*)malloc(sizeof(page_table));
    global_page_table->init();

    global_stack = (Stack*)malloc(sizeof(Stack));
    global_stack->init();

}


void createList(size_t size, char *list_name)
{
    printf("Creating List of size %lu for list name %s\n", size, list_name);
    unsigned int required_size = (unsigned int)(size*6);    //required size in words
    int *block = global_memory->find_free_block(required_size);
    printf("Block found at %p\n", block);

    if(block == NULL)
    {
        printf("Error in finding free block\n");
        return;
    }
    global_memory->allocate_block(block, required_size);
    int address = global_memory->getoffset(block);
    int index = global_page_table->insert(address);
    if(index == -1)
    {
        printf("Error in inserting page table entry\n");
        return;
    }
    //int idx = 4*index;
    block=block+1;
    list * temp = (list*)block;

    temp[size-1].next=NULL;
    temp[0].prev=NULL; 

    for(long unsigned int i=0; i<size-1; i++)
    {
        temp[i].element=i;
        temp[i].next = &temp[i+1];
        
    }
    for(long unsigned int i=1; i<size; i++)
    {
        
        temp[i].prev = &temp[i-1];
        
    }
    
    // temp[0].next = 0;

    stack_entry idx;
    idx.name=(char *)malloc(10*sizeof(char));
    strcpy(idx.name,list_name);
    idx.length=strlen(list_name);
    idx.index=index;
    global_stack->push(idx);            //pushing the list name and index to the stack
    
}

int getVal(char* name,int offset)
{
    printf("Getting val for list name %s and offset %d\n", name, offset);

    int index=global_stack->find(name);
    int address = global_page_table->table_entry[index].address;
    list *block = (list*)(global_memory->getaddress(address)+1);

    return block[offset].element;
}

void assignVal(char* name,int offset, int value)
{

    printf("Assigning val %d for list name %s and offset %d\n", value, name, offset);
    int index=global_stack->find(name);
    int address = global_page_table->table_entry[index].address;
    list *block = (list*)(global_memory->getaddress(address)+1);
    block[offset].element = value;
    printf("Assigned value %d to index %d in page table\n", value, index);
}


void freeElem(char* list_name)
{
    if(list_name != NULL)
    {
        int index = global_stack->find(list_name);
        printf("Freeing List at index %d in page table\n", index);
        int status = global_page_table->remove(index);
        if(status == -1)
        {
            printf("Error in removing page table entry\n");
            return;
        }
        global_memory->free_block(global_memory->getaddress(global_page_table->table_entry[index].address));

        Stack *temp = (Stack*)malloc(sizeof(Stack));
        temp->init();
        while(global_stack->top().index != -3)
        {
            stack_entry x;
            x.name=(char *)malloc(100*sizeof(char));
            x = global_stack->pop();
            if(strcmp(x.name,list_name)!=0)
                temp->push(x);
            else{
                printf("Found and Removed from Stack %s\n",x.name);
                break;
            }
        }
        while (temp->size != 0)
        {
            global_stack->push(temp->pop());
        }
    }
    else
    {
        printf("Freeing all indexes present in global stack\n");
        stack_entry x;
        x.name=(char *)malloc(10*sizeof(char));
        do
        {
            x=global_stack->pop();
            printf("Freeing element %d\n",x.index);
        }
        while(x.index != -3);
        
    }
}