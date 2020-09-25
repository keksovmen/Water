#include <iostream>
#include <string.h>
#include <assert.h>
#include "FixedBuffer.h"



void checkLength(int divider);

void findString(const char* origin, const char* search, bool expected);

int main()
{
	using namespace std;
	
	checkLength(1);
	checkLength(2);
	checkLength(4);
	
	findString("123456789", "567", true);
	findString("123456789", "111", false);
	findString("asd dsa qwe fgh", " ds", true);
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
	
	int size = strlen(origin);
	assert(size < buffer.getSize());
	
	for(int i = 0; i < size; i++){
		buffer += origin[i];
	}
	
	char* result = strstr(buffer.begin(), search);
	if(result){
		assert(expected == true);
	}else{
		assert(expected == false);
	}
}