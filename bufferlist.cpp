/**********************************************************
 * File     :   bufferlist.cpp
 * Project  :   CSCE 221 project 1, Fall 2024
 * Author   :   Devyani Lenin
 * Date     :   9/22/2024
 * Section  :   599
 * Email    :   devyani@tamu.edu
 * 
 * This file contains the bufferlist.cpp  program for Project 1. 
 * This program contains functions for handling a linked list of buffers. This includes adding new buffers,
 * deleting old buffers and handling memory. 
 * 
 **********************************************************/
#include "bufferlist.h"
BufferList::BufferList(int minBufCapacity){
    // **************************************
    // Implement the alternative constructor
    // **************************************

    //initialize
    m_listSize = 0;

    //if the capacity entered is less than 1 it needs to become the default
    if (minBufCapacity < 1) 
    {
        m_minBufCapacity = DEFAULT_MIN_CAPACITY;
        m_cursor = new Buffer(m_minBufCapacity);

    } else 
    {
        m_minBufCapacity = minBufCapacity;
        m_cursor = new Buffer(m_minBufCapacity);
    }

    // debug segmentation fault: take care of m_next here, m_cursor needs to point to itself for this function
    m_cursor->m_next = m_cursor;
 
    m_listSize++;
}
BufferList::~BufferList(){
    clear();
}

void BufferList::clear(){
    // *****************************
    // Implement clear() function
    // *****************************

    // debug/segmenation error: check if the list is already empty
    if (m_listSize == 0)
    {
        return;
    }

    // start from the first buffer, which is what m_cursor will point to 
    Buffer* currentBuffer = m_cursor->m_next;
    Buffer* nextBuffer;



    // go through the list and get rid of each node
    //debug: you have to clear each node first before you delete it
    for (int i = 0; i < m_listSize; i++) 
    {
        nextBuffer = currentBuffer->m_next;
        currentBuffer->clear();
        delete currentBuffer;
        currentBuffer = nextBuffer;
    }

    m_cursor = nullptr;
    m_listSize = 0;
}

void BufferList::enqueue(const int& data) {
    // ****************************************************************************
    // Implement enqueue(...) function
    // we always insert at the back, that would be the cursor
    // if the current cursor is full we create a new node as cursor then we insert
    // ****************************************************************************

    try 
    {
        // try to insert at the back, as it says above, using the enqueue function from buffer
        m_cursor->enqueue(data);
    }
    catch (const overflow_error& e) 
    {
        // if an error is thrown that means the node is full so make a new node. 

        //the new size of the new node needs to be the current size times the increase factor variable
        int currentSize = m_cursor->capacity();
        int newSize = currentSize * INCREASE_FACTOR;



        // condition: the new node's size cannot be bigger than the minimum capacity times the max factor
        if (newSize > m_minBufCapacity * MAX_FACTOR) 
        {
            newSize = m_minBufCapacity;
        }

       
        Buffer* newBuffer = new Buffer(newSize);

        //the cursor points to the front of the list which is the oldest buffer
        Buffer* oldestBuffer = m_cursor->m_next;

        //the newbuffer that is added on at the end should point to the front which is the oldest buffer
        newBuffer->m_next = oldestBuffer; 

        //the new buffer is added on at the end so it is after the rear of the list
        m_cursor->m_next = newBuffer; 

        //the newest buffer becomes the new rear of the list        
        m_cursor = newBuffer;                 

        m_listSize++;
        m_cursor->enqueue(data);
    }
}

int BufferList::dequeue() {
    // *****************************
    // Implement dequeue() function
    // *****************************

    //the oldest buffer is the front of the list: so m_cursor->m_next
    Buffer* oldestBuffer = m_cursor->m_next;

    try 
    {
        return oldestBuffer->dequeue();
    }
    catch (const underflow_error& e) 
    {
        // if there are no other nodes, then throw an exception
        if (m_listSize == 1) 
        {
            throw underflow_error("No other buffers");
        }

        //move on to the next buffer that is not empty
        Buffer* nextOldest = oldestBuffer->m_next;

        //this is now the buffer you want to dequeue, so the rear needs to point tot his
        m_cursor->m_next = nextOldest;

        //get rid of the empty buffer
        delete oldestBuffer;

        //deleting a node causes the list size to decrease
        m_listSize = m_listSize - 1;

        //try to dequeue from the new oldest buffer
        return nextOldest->dequeue();
    }
}

BufferList::BufferList(const BufferList & rhs){
    // *******************************
    // Implement the copy constructor
    // *******************************

    m_minBufCapacity = rhs.m_minBufCapacity;
    m_listSize = rhs.m_listSize;

    //copy the m_cursor; debug: you need to dereference when copying this. Create a deep copy not a shallow copy like before(m_cursor = rhs.m_cursor)
    m_cursor = new Buffer(*(rhs.m_cursor));

    //create nodes to go through both the left side and the right side of the list
    Buffer* lhsCurrent = m_cursor;
    Buffer* rhsCurrent = rhs.m_cursor->m_next;

    //debug:start at 1 we already got the first node
    for (int i = 1; i < m_listSize; i++) 
    {

        //copy the current right hand side node into the newNode in the lefthand side list, then keep going down the line for both lists
        lhsCurrent->m_next = new Buffer(*rhsCurrent);
        lhsCurrent = lhsCurrent->m_next;
        rhsCurrent = rhsCurrent->m_next;
    }

    // segmenation fault: take care of the circle here, the last node will point to m_cursor
    lhsCurrent->m_next = m_cursor;

    
}

const BufferList & BufferList::operator=(const BufferList & rhs){
    // ******************************
    // Implement assignment operator
    // ******************************

    //debug: do != instead of == condition! bc there is a return *this at the end

    if (this != &rhs) 
    {
        
        clear();

        
        m_minBufCapacity = rhs.m_minBufCapacity;
        m_listSize = rhs.m_listSize;

        //get m_cursor from the right hand side, this is getting a deep copy
        m_cursor = new Buffer(*(rhs.m_cursor));

        //copy over the other nodes into the lefthand side list
        Buffer* lhsCurrent = m_cursor;
        Buffer* rhsCurrent = rhs.m_cursor->m_next;

        for (int i = 1; i < rhs.m_listSize; i++) 
        {

            //make a new node on the lefthand side to copy in the current node from the right hand side, keep going down both lists. 
            lhsCurrent->m_next = new Buffer(*rhsCurrent);
            lhsCurrent = lhsCurrent->m_next;
            rhsCurrent = rhsCurrent->m_next;
        }

        //seg fault: linked the circle
        lhsCurrent->m_next = m_cursor;  
    }

    return *this;
}

void BufferList::dump(){
    Buffer* temp = m_cursor->m_next;
    for(int i=0;i<m_listSize;i++){
        temp->dump();
        temp = temp->m_next;
        cout << endl;
    }
    cout << endl;
}