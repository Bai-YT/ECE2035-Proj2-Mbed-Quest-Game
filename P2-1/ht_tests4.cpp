// Inform the compiler that this included module is written in C instead of C++.
extern "C" {
	#include "hash_table.h"
}
#include "gtest/gtest.h"


// Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>

// The #define directive defines a constant value that can be accessed throughout
// your code. Here it defines the default number of buckets in the hash table.
// You can change this number, but make sure to update the hash function with
// the right algorithm to compute the indices for buckets.
// For example, if the BUCKET_NUM is set to 5, the hash function should map a
// positive number to an integer between 0 and 4.
#define BUCKET_NUM  3

// Dummy value to store in hash table entry
// Please beware that any type of data (e.g. int, double, struct and etc.) can
// be stored in hash table for testing your hash table. Only the pointer to
// the data will be stored in the HashTableEntry.
struct HTItem {};

// Helper function for creating a lot of dummy values.
// Don't forget to free allocated memory after use
void make_items(HTItem* result[], unsigned n)
{
	// Populate the array with pointers to the dummy values.
	while(n--)
	{
		result[n] = (HTItem*) malloc(sizeof(HTItem));
	}
}

// A simple hash function that maps a positive number to an integer between 0~(BUCKET_NUM-1).
unsigned int hash(unsigned int key) {
  return key%BUCKET_NUM;
}

////////////////////////
// Initialization tests
////////////////////////
TEST(InitTest, CreateDestroyHashTable)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);
	destroyHashTable(ht);
}

////////////////
// Access tests
////////////////
TEST(AccessTest, GetKey_TableEmpty)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Test when table is empty.
  EXPECT_EQ(NULL, getItem(ht, 0));
  EXPECT_EQ(NULL, getItem(ht, 1));
  EXPECT_EQ(NULL, getItem(ht, 2));

  // Test with index greater than the number of buckets.
  EXPECT_EQ(NULL, getItem(ht, 10));

  destroyHashTable(ht);
}

TEST(AccessTest, GetSingleKey)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create a list of items to add to hash table.
	size_t num_items = 5;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	/*for(int i = 0; i< 15; i++ ){
		insertItem(ht, i, m[i%4]);
	}*/
  insertItem(ht, 1, m[0]);
  insertItem(ht, 4, m[2]);
	// Test if the correct value can be retrieved from hash table with its key.
  EXPECT_EQ(NULL, getItem(ht, 7));
  destroyHashTable(ht);
//  free(m[4]);
//  free(m[3]);
//  free(m[2]);
//  free(m[1]);
  //free(m[0]);
}

TEST(AccessTest, GetKey_KeyNotPresent)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create a list of items to add to hash table.
	size_t num_items = 1;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);

	// Test if the hash table returns NULL when the key is not found.
  EXPECT_EQ(NULL, getItem(ht, 1));

  destroyHashTable(ht);
}

////////////////////////////
// Removal and delete tests
////////////////////////////
TEST(RemoveTest, SingleValidRemove)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create a list of items to add to hash table.
	size_t num_items = 3;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);
	insertItem(ht, 3, m[1]);
	insertItem(ht, 6, m[2]);
	// After removing an item with a specific key, the data stored in the
	// corresponding entry should be returned. If the key is not present in the
	// hash table, then NULL should be returned.
  void* data = removeItem(ht, 1);

	// Since the key we want to remove is present in the hash table, the correct
	// data should be returned.
  EXPECT_EQ(NULL, data);

	data = getItem(ht, 0);
	EXPECT_EQ(m[0], data);
    deleteItem(ht, 0);
  data = getItem(ht, 0);
  EXPECT_EQ(NULL, data);

  deleteItem(ht, 6);
  //EXPECT_EQ(m[2], data);
  deleteItem(ht, 6);
 data = getItem(ht, 6);
  EXPECT_EQ(NULL, data);

 deleteItem(ht, 4);
 data = getItem(ht, 4);
  EXPECT_EQ(NULL, data);

 deleteItem(ht, 3);
  //EXPECT_EQ(m[1], data);
	// Free the data
	//free(m[0]);
	//free(m[1]);
	//free(m[2]);
  destroyHashTable(ht);
}

TEST(RemoveTest, SingleInvalidRemove)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// When the hash table is empty, the remove funtion should still work.
  EXPECT_EQ(NULL, removeItem(ht, 1));

  destroyHashTable(ht);
}

///////////////////
// Insersion tests
///////////////////
TEST(InsertTest, InsertAsOverwrite)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create list of items to be added to the hash table.
  size_t num_items = 8;
  HTItem* m[num_items];
	make_items(m, num_items);

	// Only insert one item with key=0 into the hash table.
  EXPECT_EQ(NULL, insertItem(ht, 0, m[0]));
  EXPECT_EQ(NULL, insertItem(ht, 1, m[1]));
  EXPECT_EQ(NULL, insertItem(ht, 2, m[2]));
   EXPECT_EQ(NULL, insertItem(ht, 3, m[3]));
    EXPECT_EQ(NULL, insertItem(ht, 6, m[6]));
  EXPECT_EQ(m[2], insertItem(ht, 2, m[7]));
  EXPECT_EQ(m[1], insertItem(ht, 1, m[4]));
  EXPECT_EQ(m[0], insertItem(ht, 0, m[5]));
   EXPECT_EQ(m[7], getItem(ht, 2));
  EXPECT_EQ(m[4], getItem(ht, 1));
  EXPECT_EQ(m[5], getItem(ht, 0));
	// When we are inserting a different value with the same key=0, the hash table
	// entry should hold the new value instead. In the test case, the hash table entry
	// corresponding to key=0 will hold m[1] and return m[0] as the return value.
  //EXPECT_EQ(m[0], insertItem(ht, 0, m[1]));

	// Now check if the new value m[1] has indeed been stored in hash table with
	// key=0.
  //EXPECT_EQ(m[1], getItem(ht,0));

  destroyHashTable(ht);	// here m[1] is freed
  //free(m[7]);
		// free item 0 here to prevent memory leak
}
