/**********************************************************
 * File     :   buffer.cpp
 * Project  :   CSCE 221 project 1, Fall 2024
 * Author   :   Devyani Lenin
 * Date     :   9/22/2024
 * Section  :   599
 * Email    :   devyani@tamu.edu
 * 
 * This file contains the buffer.cpp program for Project 1. 
 * This program contains the functions for handling a circular buffer. This include adding and removing
 * items from the dynamic array(buffer).
 * 
 **********************************************************/
#include "buffer.h"

Buffer::Buffer(int capacity){
    // **************************************
    // Implement the alternative constructor
    // No need to set m_next to nullptr, 
    // it will be handled by linked list,
    // because it is used by linked list
    // **************************************

    //condition: if capacity is less than 1, it is automatically set to 0, and the pointer is null
    if (capacity < 1) 
    {
        m_capacity = 0;
        m_buffer = nullptr;
    } else 
    {
        m_capacity = capacity;
        m_buffer = new int[m_capacity];
    }


    //instructions state to not initialize m_next
    m_count = 0;
    m_start = 0;
    m_end = 0;
}

void Buffer::clear(){
    // ***********************************
    // Implement clear() function
    // No need to set m_next to nullptr, 
    // it will be handled by linked list,
    // because it is used by linked list
    // ***********************************

    //deallocate the memory for the buffer pointer first
    delete[] m_buffer;
    m_buffer = nullptr;

    //make everyother int value equal to 0
    m_capacity = 0;
    m_count = 0;
    m_start = 0;
    m_end = 0;
}

Buffer::~Buffer(){
    clear();
}

int Buffer::count(){return m_count;}

int Buffer::capacity(){return m_capacity;}

bool Buffer::full(){
    // **************************
    // Implement full() function
    // **************************

    //in a full buffer the # of elements will be equal to the capacity it can have
    if (m_count == m_capacity)
    {
        return true;
    }

    
    return false;


 
}

bool Buffer::empty(){
    // **************************
    // Implement empty() function
    // **************************

    //if there is nothing in the buffer then the number of items is 0
    if( m_count == 0) 
    {
        return true;
    }


    return false;

}

void Buffer::enqueue(int data){
    // ********************************
    // Implement enqueue(...) function
    // ********************************

    //full buffer condition
    if (full()) 
    {
        throw overflow_error("buffer is full!");
    }

    //set the last index to have the info, and increase the number of elements
    m_buffer[m_end] = data;

    //circle buffer method that was shown in dump() function
    m_end = (m_end + 1) % m_capacity; 
    m_count++;
}

int Buffer::dequeue(){
    // *****************************
    // Implement dequeue() function
    // *****************************

    if (empty()) 
    {
        throw underflow_error("buffer is empty!");
    }

    //return the information in the first index, you have to take it from the oldest one which is m_start, this will take away an element
    int dataPiece = m_buffer[m_start];
    m_start = (m_start + 1) % m_capacity; 
    m_count--;
    return dataPiece;
}

Buffer::Buffer(const Buffer & rhs){
    // *******************************
    // Implement the copy constructor
    // *******************************

    //debugging: if there is a negative buffer size, you can't put anything in the data, so it would have a size 0, you took care of this in the constructor so don't change anything!!

    m_capacity = rhs.m_capacity;
    m_count = rhs.m_count;
    m_start = rhs.m_start;
    m_end = rhs.m_end;
    m_buffer = new int[m_capacity];

    // copy each element of the right hand side buffer into m_buffer for a deep copy
    for (int i = 0; i < m_capacity; i++) 
    {
        m_buffer[i] = rhs.m_buffer[i];
    }

    
}

const Buffer & Buffer::operator=(const Buffer & rhs){
    // ******************************
    // Implement assignment operator
    // ******************************

    //debug: check != instead of == condition! bc there is a return *this at the end

    if (this != &rhs) 
    {

        //if they aren't the same get rid of the lefthand side
        clear(); 

        
        m_capacity = rhs.m_capacity;
        m_start = rhs.m_start;
        m_end = rhs.m_end;
        m_count = rhs.m_count;
        

        if (rhs.m_buffer != nullptr) 
        {
            //do a deep copy to copy the element at each index
            m_buffer = new int[m_capacity];
            for (int i = 0; i < m_capacity; ++i) {
                m_buffer[i] = rhs.m_buffer[i];
            }
        } else 
        {
            //debug: seg fault fix
            m_buffer = nullptr;
        }
    }
    return *this;
}

void Buffer::dump(){
    int start = m_start;
    // int end = m_end;
    int counter = 0;
    cout << "Buffer size: " << m_capacity << " : ";
    if (!empty()){
        while(counter < m_count){
            cout << m_buffer[start] << "[" << start << "]" << " ";
            start = (start + 1) % m_capacity;
            counter++;
        }
        cout << endl;
    }
    else cout << "Buffer is empty!" << endl;
}