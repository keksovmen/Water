#include <iostream>
#include <string.h>
#include <assert.h>
#include "FixedBuffer.h"



void checkLength(int divider);

int main()
{
	using namespace std;
	
	checkLength(1);
	checkLength(2);
	checkLength(4);
}

void checkLength(int divider){
	assert (divider > 0);
	
	FixedBuffer<512> buffer;
	for(int i = 0; i < buffer.getSize(); i++){
		buffer+= 'f';
	}
	assert(strlen(buffer.begin()) == buffer.getLength());
}