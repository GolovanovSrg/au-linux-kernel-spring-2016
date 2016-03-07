#include "stack.h"
#include <linux/slab.h>
#include <linux/gfp.h>

stack_entry_t* create_stack_entry(void *data)
{
    stack_entry_t* entry = (stack_entry_t*) kmalloc(sizeof(stack_entry_t), GFP_KERNEL);
    
    if (entry != NULL) 
    {
        INIT_LIST_HEAD(&entry->lh);
        entry->data = data;
    }
    
    return entry;  
}

void delete_stack_entry(stack_entry_t *entry)
{
    list_del(&entry->lh);
    kfree(entry);
}

void stack_push(struct list_head *stack, stack_entry_t *entry)
{
   list_add(&entry->lh, stack); 
}

stack_entry_t* stack_pop(struct list_head *stack)
{
    return list_first_entry(stack, stack_entry_t, lh);
}
