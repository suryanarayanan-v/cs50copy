// Implements a dictionary's functionality

//#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

//Defining constants for universal hash
#define k 7919
#define a 321
#define Hsize 1009
#define b 43112

unsigned int no_of_words = 0;
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1500000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //hasing word
    char lower[LENGTH + 1];
    strcpy(lower, word);
    for (int i = 0; lower[i] != '\0'; i++)
    {
        lower[i] = tolower(lower[i]);
    }
    int hash_no = hash(lower);
    //check for word in the hash table
    for (node *tmp = table[hash_no]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
// citation: djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    //Universal hash process
    unsigned int hash = 5381;
    int c;
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //Opening dictionary file
    FILE *dic = fopen(dictionary, "r");
    if (dic == NULL)
    {
        return false;
    }
    
    // Declaring values "word" and "hash_no" for reading each word and storing the hash value of the word
    char word[LENGTH + 1];
    unsigned int hash_no;
    
    // Setting all values of the table array to null
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    
    //Iterating through all the words in the dictionary
    while (fscanf(dic, "%s", word) != EOF)
    {
        //hashing word
        hash_no = hash(word);
        if (hash_no > N)
        {
            hash_no = hash_no % N;
        }
        no_of_words++;
        
        // Creating node for given word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        
        //copying word and setting the next pointer to null
        strcpy(n->word, word);
        n->next = NULL;
        
        //Checking to see if the hash table already has a node
        if (table[hash_no] == NULL)
        {
            table[hash_no] = n;
        }
        else
        {
            // Iterating through each node in hash table and assigning new node to the end of current node
            node *tmp = table[hash_no];
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
            tmp->next = n;
        }
    }
    fclose(dic);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return no_of_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Declaring erase to free up each node and Declaring cursor so that i dont lose track of the next nodes
    node *cursor = NULL;
    node *erase = NULL;
    for (int i = 0; i < N; i++)
    {
        // Checking if a hash table is empty
        if (table[i] == NULL)
        {
            continue;
        }
        
        //Erasing every node in hash table
        erase = table[i];
        cursor = erase->next;
        while (cursor != NULL)
        {
            free(erase);
            erase = cursor;
            cursor = cursor->next;
        }
        free(erase);
    }
    return true;
}
