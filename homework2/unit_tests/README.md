# Unit Tests

These unit tests can be implemented by copying the contents of this directory in to the root folder of cmocka.

Once you add the contents of this directory. Run the following command to run the unit tests on the doubly linked data structures library.

    # $ sudo make test

The unit tests implemented here are as follows

# test_insert_at_beginning(void **state) -

In this unit test a head pointer whose next pointer is null i.e. an empty linked list is chosen. Then we call the insert_at_beginning function which then makes the length of the list to 1. Then i fetched data at position 1 and verified if the data read from position 1 and data added are the same.

# test_insert_at_end(void **state)

In this unit test a head pointer whose next pointer is null i.e. an empty linked list is chosen. Then we call the insert_at_beginning function and then call the insert_at_end functions. Then fetch data at the 2nd position and see if the data inserted in insert_at_end is same as the data fetched from 2nd position 

# test_insert_at_position(void **state)

In this unit test a head pointer whose next pointer is null i.e. an empty linked list is chosen. Then we insert 3 elements in the linked list. Then we cal insert_at_position function and insert the element at 3. I then fetch data at position 3 of the linked list and check if the fetched data is equal to the data inserted through insert_at_position.

# test_remove_at_beginning(void **state)

In this unit test a head pointer whose next pointer is null i.e. an empty linked list is chosen. Then we insert 3 element in to the list and then remove the element from beginning and see if the head pointer now points the element inserted 2nd.

# test_remove_at_end(void **state)

In this unit test a head pointer whose next pointer is null i.e. an empty linked list is chosen. Then we insert 3 elements in to the linked list and then call remove_at_end and see if the element removed is 3 and the data fetched at position 1 and 2 are the data inserted 1st and 2nd.

# test_remove_at_position(void **state)

n this unit test a head pointer whose next pointer is null i.e. an empty linked list is chosen. Then we insert 3 elements in to the linked list and then call remove_at_position function to remove 2nd element in the list. Now then we check if the data fetched from position 2 is the 3rd element that has been inserted.

# test_size(void **state)

n this unit test a head pointer whose next pointer is null i.e. an empty linked list is chosen.Then I make 3 inserts and size function gets called before and after every insert. I then checked if the 4 sizes returned are 0, 1, 2, 3 correspondingly. 

# test_destroy(void **state)

n this unit test a head pointer whose next pointer is null i.e. an empty linked list is chosen.Then I make 3 inserts and then call teh destroy function. I then verify if the destroy function returns a head pointer whose next pointer points to null.

