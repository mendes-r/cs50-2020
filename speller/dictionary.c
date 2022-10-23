// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 250000; //105000 ~ 73% aof all words in the large dictionary

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    // Convert string to lowercase only
    char str[LENGTH + 1];
    strcpy(str, word);

    for (int j = 0; str[j]; j++)
    {
        str[j] = tolower(str[j]);
    }

    // Create a paralelle pointer to the hash table
    node *trv = table[hash(str)];

    if (trv == NULL)
    {
        return false;
    }

    // Check if the word exists in that index

    // If just one element in this slot of the array
    if (trv->next == NULL)
    {
        if (strcmp(str, trv->word) == 0)
        {
            return true;
        }

        return false;
    }

    // If a linked-list
    while (trv != NULL)
    {
        if (strcmp(str, trv->word) == 0)
        {
            return true;
        }

        trv = trv->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // djb2 by dan bernstein
    unsigned int hash = 5381;
    int c;
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; //hash * 33 + c
    }
    return hash % N;
}

int word_count = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    char *word = malloc(sizeof(char) * (LENGTH + 1));

    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a node for the word
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);
        n->next = NULL;

        // Find a hash number for the word
        unsigned int hash_numb = hash(n->word);

        // Insert word in hash table
        if (table[hash_numb] != NULL)
        {
            // Insert word at the beginning of the linked list
            n->next = table[hash_numb];
            table[hash_numb] = n;
            word_count ++;

        }
        else 
        {
            table[hash_numb] = n;
            word_count ++;
        }

    }
    free(word);
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (long i = 0; i < N + 1; i ++)
    {
        node *trv = table[i];

        // tranvers the linked list

        while (trv != NULL)
        {
            node *tmp = trv;
            trv = trv->next;
            free(tmp);
        }

    }

    return true;
}