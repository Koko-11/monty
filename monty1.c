#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define EXIT_FAILURE 1

/**
 * struct stack_s - Structure representing a stack node.
 * @n: Integer data stored in the node.
 * @next: Pointer to the next node in the stack.
 */
typedef struct stack_s
{
    int n;
    struct stack_s *next;
} stack_t;

stack_t *stack = NULL; // Global stack

/**
 * is_integer - Check if a string represents a valid integer.
 * @str: The string to check.
 * Return: 1 if it's a valid integer, 0 otherwise.
 */
int is_integer(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0;

    if (*str == '-' || *str == '+')  // Check for negative or positive sign
        str++;

    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }

    return 1;
}

/**
 * push - Pushes an integer onto the stack.
 * @line_number: The current line number in the file (for error reporting).
 * @value: The value to push onto the stack.
 */
void push(int line_number, char *value)
{
    if (!is_integer(value))
    {
        fprintf(stderr, "L%d: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    int num = atoi(value);
    stack_t *new_node = malloc(sizeof(stack_t));

    if (new_node == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->n = num;
    new_node->next = stack; // Insert the new node at the top
    stack = new_node; // Update the stack to point to the new top
}

/**
 * pall - Prints all values in the stack from top to bottom.
 */
void pall(void)
{
    stack_t *current = stack;

    while (current != NULL)
    {
        printf("%d\n", current->n);
        current = current->next;
    }
}

/**
 * free_stack - Frees the memory used by the stack.
 */
void free_stack(void)
{
    stack_t *current = stack;

    while (current != NULL)
    {
        stack_t *next_node = current->next;
        free(current);
        current = next_node;
    }
}

/**
 * main - Entry point of the program.
 * @argc: The number of arguments passed to the program.
 * @argv: Array of argument strings.
 * Return: 0 on success, EXIT_FAILURE on error.
 */
int main(int argc, char *argv[])
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number = 0;

    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    while ((read = getline(&line, &len, file)) != -1)
    {
        line_number++;
        char *opcode = strtok(line, " \n\t");

        if (opcode == NULL || opcode[0] == '#')  // Ignore empty lines or comments
            continue;

        if (strcmp(opcode, "push") == 0)
        {
            char *arg = strtok(NULL, " \n\t");
            push(line_number, arg);
        }
        else if (strcmp(opcode, "pall") == 0)
        {
            pall();
        }
        else
        {
            fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
            free(line);
            fclose(file);
            free_stack();
            return EXIT_FAILURE;
        }
    }

    free(line);
    fclose(file);
    free_stack();
    return 0;
}
