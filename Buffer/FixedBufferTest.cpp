#include <iostream>
#include <string.h>
#include <assert.h>
#include "FixedBuffer.h"



void checkLength(int divider);

void findString(const char* origin, const char* search, bool expected);

void checkEndWith(const char* origin, const char* ending, bool expected);

void checkStartsWith(const char* origin, const char* start, bool expected);

void checkIndexOf(const char* origin, const char* search, int expected);


template<int N>
void copyIntoBuffer(FixedBuffer<N>& buffer, const char* data);


int main()
{
	using namespace std;
	
	checkLength(1);
	checkLength(2);
	checkLength(4);
	
	findString("123456789", "567", true);
	findString("123456789", "111", false);
	findString("asd dsa qwe fgh", " ds", true);
	
	checkEndWith("qwerty dsa", "dsa", true);
	checkEndWith("qwerty dsa", "sa", true);
	checkEndWith("qwerty dsa", "a", true);
	checkEndWith("qwerty dsa", " ", false);
	checkEndWith("qwerty dsa", "asd", false);
	checkEndWith("a", "asd", false);
	
	checkStartsWith("qwerty", "qwe", true);
	checkStartsWith("  werty", " ", true);
	checkStartsWith("  qwerty", "qwerty", false);
	checkStartsWith("  qwerty", "  q", true);
	checkStartsWith("asd", "asdfghjkl", false);
	
	checkIndexOf("0123456789", "3", 3);
	checkIndexOf("0123456789", "12", 1);
	checkIndexOf("one two three", "four", -1);
	checkIndexOf("one two three", "three", 8);
}

void checkLength(int divider){
	assert (divider > 0);
	
	FixedBuffer<512> buffer;
	for(int i = 0; i < buffer.getSize(); i++){
		buffer+= 'f';
	}
	assert(strlen(buffer.begin()) == buffer.getLength());
}

void findString(const char* origin, const char* search, bool expected){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	
	char* result = strstr(buffer.begin(), search);
	if(result){
		assert(expected == true);
	}else{
		assert(expected == false);
	}
}

void checkEndWith(const char* origin, const char* ending, bool expected){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	
	if(buffer.endWith(ending)){
		assert(expected == true);
	}else{
		assert(expected == false);
	}
}

void checkStartsWith(const char* origin, const char* start, bool expected){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	
	if(buffer.startsWith(start)){
		assert(expected == true);
	}else{
		assert(expected == false);
	}
}

void checkIndexOf(const char* origin, const char* search, int expected){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	int index = buffer.indexOf(search);
	
	if(index){
		assert(expected == index);
	}else{
		assert(expected == -1);
	}
}


template<int N>
void copyIntoBuffer(FixedBuffer<N>& buffer, const char* data){
	int size = strlen(data);
	assert(size < buffer.getSize());
	
	for(int i = 0; i < size; i++){
		buffer += data[i];
	}
}