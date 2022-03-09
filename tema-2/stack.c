/* CIOBANU Andrei-Bogdan - 312CB */

#include "stack.h"

#include <stdlib.h>
#include <string.h>

/* initializeaza o stiva cu elemente de dimensiune item_size */
struct stack *initialize_stack(size_t item_size)
{
    struct stack *new_stack = malloc(sizeof(struct stack));
    if (new_stack == NULL)
        return NULL;

    new_stack->item_size = item_size;
    new_stack->height = 0;
    new_stack->top = NULL;

    return new_stack;
}

/* elibereaza memoria ocupata de o stiva data */
void free_stack(struct stack **stack, void (*free_data)(void *))
{
    if (*stack == NULL)
        return;

    free_list(&(*stack)->top, free_data);
    free(*stack);

    *stack = NULL;
}

/* insereaza item in stiva stack */
int push(struct stack *stack, void *item)
{
    struct node *new_node = create_node(item);
    if (new_node == NULL)
        return -1;

    new_node->next = stack->top;
    stack->top = new_node;

    ++stack->height;

    return 1;
}

/* extrage elementul din varful stivei */
int pop(struct stack *stack, void *extracted_item, void (*free_data)(void *))
{
    if (stack->top == NULL)
        return 0;

    memcpy(extracted_item, stack->top->data, stack->item_size);

    struct node *old_top = stack->top;
    stack->top = old_top->next;

    free_data(old_top->data);
    free(old_top);

    --stack->height;

    return 1;
}

/* rastoarna stiva sursa in stiva destinatie */
void flip(struct stack *destination, struct stack *source)
{
    if (destination->item_size != source->item_size || source->top == NULL)
        return;

    struct node *source_top = source->top;
    while (source_top != NULL) {
        struct node *new_source_top = source_top->next;
        source_top->next = destination->top;

        destination->top = source_top;
        source_top = new_source_top;
    }

    source->top = NULL;
}

/*
 * identifica elementul din stiva care indeplineste conditia data, fara a-l
 * extrage; returneaza adresa informatiei gasite
 */
void *find_item_in_stack(struct stack *stack, void *item,
                         int (*fcmp)(void *, void *))
{
    void *found_item = NULL;

    struct stack *temp = initialize_stack(stack->item_size);
    if (temp == NULL)
        return NULL;

    flip(temp, stack);

    /* cauta elementul si reformeaza stiva stack in ordinea buna */
    struct node *temp_top = temp->top;
    while (temp_top != NULL) {
        if (fcmp(item, temp_top->data) == 0)
            found_item = temp_top->data;

        struct node *new_temp_top = temp_top->next;
        temp_top->next = stack->top;

        stack->top = temp_top;
        temp_top = new_temp_top;
    }

    free(temp);

    return found_item;
}

/*
 * extrage elementul din stiva care indeplineste conditia data; returneaza
 * informatia extrasa
 */
void *remove_item_from_stack(struct stack *stack, void *item,
                             int (*fcmp)(void *, void *))
{
    void *removed_item = NULL;

    struct stack *temp = initialize_stack(stack->item_size);
    if (temp == NULL)
        return NULL;

    flip(temp, stack);

    /* cauta elementul si reformeaza stiva stack in ordinea buna */
    struct node *temp_top = temp->top;
    while (temp_top != NULL) {
        /*
         * daca s-a gasit elementul care indeplineste conditia data, nu il muta
         * in temp; extrage informatia si elibereaza memoria ocupata de celula
         */
        if (fcmp(item, temp_top->data) == 0) {
            struct node *removed_node = temp_top;
            temp_top = removed_node->next;

            removed_item = removed_node->data;
            free(removed_node);

            --stack->height;
        } else {
            struct node *new_temp_top = temp_top->next;
            temp_top->next = stack->top;

            stack->top = temp_top;
            temp_top = new_temp_top;
        }
    }

    free(temp);

    return removed_item;
}

/* afiseaza elementele din stiva */
void print_stack(FILE *stream, struct stack *stack,
                 void (*print_item)(FILE *, void *))
{
    print_list(stream, stack->top, print_item);
}
