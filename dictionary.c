/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#define SIZE 200000

/*define a struct containing a char array (word) with LENGTH plus the null terminator 
and a pointer to the another node.*/

typedef struct node
{
    char word[LENGTH + 1];   
    struct node* next;
} link;

// SDBM Hash Function from http://snipplr.com/view/55639/hashing-function-for-c  
unsigned int hashFunc(const char* str)
{
   unsigned int hash = 0;
   
   for(int i = 0; i < strlen(str); i++)
   {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
   }
    
   return (hash & 0x7FFFFFFF) % SIZE;
}

//Create an array with SIZE
struct node* hashArray[SIZE];

//initialize the number of words loaded.
int count = 0; 

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{   
    //Create a lowercase copy of the word.
    int len = strlen(word);
    char copy[LENGTH + 1];
    for (int i = 0; i < len; i++)
    {
        copy[i] = tolower(word[i]);
    }
    copy[len] = '\0';
    
    //define the hash table index.
    int index = hashFunc(copy);
    
    //create a pointer variable pointing to the 1st node.
    struct node* ptr = hashArray[index];
   
   //compare the lowercase copy with words in the node.
    while (ptr!= NULL)
    {
        if (strcmp(copy, ptr->word) == 0)
        {
            //return true if the copy and word are the same.
            return true;
        }
        
        //increment the pointer variable.
        ptr = ptr-> next;
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    //create a char array.
    char copy[LENGTH + 1];
    
    //open the dictionary file.
    FILE* input = fopen(dictionary, "r");
    
    //if the file doesn't exist, return false.
     if (input == NULL)
    {
        printf("Could not open %s.\n", "dictionary");
        return false;
    }
    
    //Read formatted data from stream.
    while(fscanf(input, "%s", copy) != EOF ){
        
        /*create a new node with a block of size bytes 
        of memory pointed to by a pointer variable*/ 
        struct node *temp  = malloc(sizeof(struct node));
        
        //if the node does not exist, return false.
        if (temp == NULL)
        {
            return false; 
        }
        
        //Copy the string into the node, including the terminating null character. 
        strcpy(temp->word, copy);
        
        //define the hash table index
        int hashIndex = hashFunc(temp->word);
        
        if(hashArray[hashIndex]==NULL)
        {
            //the hash array pointing to the 1st node.
            hashArray[hashIndex] = temp;
            
            //count the number of words loaded.
            count ++;
        }
        else
        {
            //the 2nd node pointing to the 1st node. 
            temp->next = hashArray[hashIndex];
            
            //the hash array pointing to the 2nd node.
            hashArray[hashIndex] = temp;
            
            //count the number of words loaded.
            count ++;  
        }  
    }
    //close the dictionary file.
    fclose(input);

     //if the hash array is not null, return true, else false.
    for (int index = 0; index < SIZE; index++ )
    {  
        if (hashArray[index] != NULL)
        {
            return true; 
        }
    }
    return false;
}
/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    for (int index = 0; index < SIZE; index++)
    { 
        if (hashArray[index] != NULL)
        {
            return count; 
        }
    }
    return 0;
}
/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int index = 0; index < SIZE; index++ )
    {   
        //while the value of the hash array is not null, 
        while (hashArray[index] != NULL)
        {
            //create a pointer variable pointing to the 1st node.
            struct node* temp = hashArray[index];
            
            //points the hash array to the 2nd node.
            hashArray[index] = temp->next;
            
            //deallocate the memory of the 1st node.
            free(temp);
            temp = NULL;
        }
        
        //if the hash array is not null, return false, else true.
        if (hashArray[index] != NULL)
        {
            return false;    
        }   
    }
    return true;
}
