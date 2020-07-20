/*
 Student Name: Yatong Bai
 Date: April 3rd

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {
    
    // Allocate memory for the new item
    HashTableEntry* newItem = (HashTableEntry*)malloc(sizeof(HashTableEntry));
    
    // Initialize the fields and return the new item
    newItem -> key = key;
    newItem -> value = value;
    newItem -> next = NULL;
    return newItem;
    
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {
    
    // Go through each bucket
    int j = 0;
    HashTableEntry* k;
    while (j < (hashTable -> num_buckets)){
        
        // Go to the next bucket if this bucket is empty
        if ((hashTable -> buckets)[j] == NULL){
            j++;
            continue;
        }
        
        // Check if the first entry of the bucket matches the key
        k = (hashTable -> buckets)[j];         // k is the address of the first item of the bucket
        if (key == (k -> key)){
            return k;
        }
        
        // Check if other entries in the linked list matches the key
        while ((k -> next) != NULL){
            if (key == (k -> next -> key)){
                return (k -> next);
            }
                
            k = (k -> next);
        }
        j++;
    }

    return NULL;
}

// freeBucket() recursively free every entry in a bucket
static void freeBucket(HashTableEntry* item){
    
    // Do nothing and return if the memory is null
    if (item == NULL){
        return;
    }
    
    // Free the next entry in the linked list if it exists
    if(item -> next != NULL){
        freeBucket(item -> next);
    }
    
    // Free this item and its value
    free(item -> value);
    free(item);
    
    return;
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket.
  // Exit gracefully if this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets are empty, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable) {
    
    // Free every bucket in the hash table with freeBucket()
    int j = 0;
    while (j < (hashTable -> num_buckets)){
        freeBucket((hashTable -> buckets)[j]);
        j++;
    }
    
    // Delete the hash table itself
    free(hashTable -> buckets);
    free(hashTable);
    return;
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {
    
    // Create the entry and find its hash value
    HashTableEntry* newItem = createHashTableEntry(key, value);
    unsigned int hash = (hashTable -> hash)(key);
    
    // Go through the buckets
    HashTableEntry* v; HashTableEntry* k;
    int j = 0; int a = -1; void* p;
    while (j < (hashTable -> num_buckets)){
        
        // If a bucket is empty, put the new item in the bucket as its first entry
        if ((hashTable -> buckets)[j] == NULL){
            if (a == -1)
                a = j;
            j++;
            continue;
        }
        
        // Find the bucket with the correct hash value
        unsigned int hash1 = (hashTable -> hash)((hashTable -> buckets)[j] -> key);
        if (hash == hash1){
            k = (hashTable -> buckets)[j];         // k is the address of the first item of the bucket
            
            // Check if the first entry in a bucket has the same key
            // If so, replace that entry and update the previous entry
            if (key == (k -> key)){
                newItem -> next = k -> next;
                (hashTable -> buckets)[j] = newItem;
                p = (k -> value);
                free(k);
                return p;
            }
            
            // Check if other entries in the bucket has the same key
            // If so, replace that entry and update the previous entry
            // If not, add the new entry to the end of the linked list and update the previous entry
            while ((k -> next) != NULL){
                if (key == ((k -> next) -> key)){
                    v = k -> next;
                    newItem -> next = v -> next;
                    k -> next = newItem;
                    p = (v -> value);
                    free(v);
                    return p;
                }
                k = (k -> next);
            }
            k -> next = newItem;
            return NULL;
        }
        j++;
    }
    
    (hashTable -> buckets)[a] = newItem;
    return NULL;
}

void* getItem(HashTable* hashTable, unsigned int key) {
    
    // Find the item and return its value
    // If the item does not exist, return null
    HashTableEntry* item = findItem(hashTable, key);
    if (item == NULL){
        return NULL;
    }
    return(item -> value);
}

void* removeItem(HashTable* hashTable, unsigned int key) {

    HashTableEntry* v; HashTableEntry* k; void* val;
    int j = 0;
    while (j < (hashTable -> num_buckets)){
        
        // If this bucket is null, go to the next one
        if ((hashTable -> buckets)[j] == NULL){
            j++;
            continue;
        }
        
        // Check if the first entry in a bucket has the key
        // If so, remove that entry and update the bucket
        k = (hashTable -> buckets)[j];         // k is the address of the first item of the bucket
        if (key == (k -> key)){
            (hashTable -> buckets)[j] = (k -> next);
            val = (k -> value);
            free(k);
            return val;
        }
        
        // Check if other entries in the bucket has the same key
        // If so, remove that entry and update the previous entry
        // If not, go to the next bucket
        while ((k -> next) != NULL){
            if (key == ((k -> next) -> key)){
                v = k -> next;
                k -> next = (k -> next) -> next;
                val = v -> value;
                free(v);
                return val;
            }
            k = (k -> next);
        }
        j++;
    }
    
    // If the key can not be found, return null
    return NULL;
}

void deleteItem(HashTable* hashTable, unsigned int key) {
    
    // Remove the entry
    void* val = removeItem(hashTable, key);
    
    // Free the value
    free(val);
    return;
}
