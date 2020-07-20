This project is a game that can be played using the mbed microcontroller.
A self-implemented version of hash table was created in c to support the data structure required by the game.
This was an individual class project.

How to play:
Tilt the microcontroller to make the main charactor move in the desired direction and push buttons to do desired operations.
The characters should complete some tasks to win the game.
Hardware libraries were provided, but my code interfaced with those libraries.

Hash table:
Different objects that can help or harm the main charactor are placed in the map using a hash table.
hash_table.c implements a hash table in c, including the functionality of creating the hash table and the hash function, and inserting, fetching, removing and deleting items.
Many test cases were created and used to test the robustness of this hash table. 
Test cases are in ht_tests.cpp.