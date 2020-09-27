#include <iostream>
#include <string.h>
#include <assert.h>
#include "FixedBuffer.h"



bool checkLength(int divider);

bool findString(const char* origin, const char* search);

bool checkEndWith(const char* origin, const char* ending);

bool checkStartsWith(const char* origin, const char* start);

bool checkIndexOf(const char* origin, const char* search, int expected);

bool checkTrim(const char* origin, const char* expected);

bool checkRemove(const char* origin, int start, int amount, const char* expected);

bool checkSubstring(const char* origin, int from, int to, const char* expected);


template<int N>
void copyIntoBuffer(FixedBuffer<N>& buffer, const char* data);


int main()
{
	using namespace std;
	
	assert (checkLength(1));
	assert (checkLength(2));
	assert (checkLength(4));
	
	assert (findString("123456789", "567") == true);
	assert (findString("123456789", "111") == false);
	assert (findString("asd dsa qwe fgh", " ds") == true);
	
	assert (checkEndWith("qwerty dsa", "dsa") == true);
	assert (checkEndWith("qwerty dsa", "sa") == true);
	assert (checkEndWith("qwerty dsa", "a") == true);
	assert (checkEndWith("qwerty dsa", " ") == false);
	assert (checkEndWith("qwerty dsa", "asd") == false);
	assert (checkEndWith("a", "asd") == false);
	
	assert (checkStartsWith("qwerty", "qwe") == true);
	assert (checkStartsWith("  werty", " ") == true);
	assert (checkStartsWith("  qwerty", "qwerty") == false);
	assert (checkStartsWith("  qwerty", "  q") == true);
	assert (checkStartsWith("asd", "asdfghjkl") == false);
	
	assert (checkIndexOf("0123456789", "3", 3));
	assert (checkIndexOf("0123456789", "12", 1));
	assert (checkIndexOf("one two three", "four", -1));
	assert (checkIndexOf("one two three", "three", 8));
	
	assert (checkTrim(" ", ""));
	assert (checkTrim("\t", ""));
	assert (checkTrim("\r\f\t", ""));
	assert (checkTrim("\r\f\t", ""));
	assert (checkTrim("\n123\n", "123"));
	assert (checkTrim("\r\n 1 2 3 \r\n", "1 2 3"));
	assert (checkTrim("\n123", "123"));
	
	assert (checkRemove("0123456789", 0, 3, "3456789"));
	assert (checkRemove("0123456789", 3, 3, "0126789"));
	assert (checkRemove("0123456789", 5, 5, "01234"));
	assert (checkRemove("0123456789", 9, 1, "012345678"));
	assert (checkRemove("0123456789", 9, 2, "0123456789"));
	assert (checkRemove("0123456789", 15, 0, "0123456789"));
	assert (checkRemove("0123456789", -15, 0, "0123456789"));
	assert (checkRemove("0123456789", -15, -4, "0123456789"));
	assert (checkRemove("0123456789", 1, -4, "0123456789"));
	
	assert (checkSubstring("0123456789", 0, 1, "0"));
	assert (checkSubstring("0123456789", 1, 2, "1"));
	assert (checkSubstring("0123456789", 3, 5, "34"));
	assert (checkSubstring("0123456789", 1, 1, "0123456789"));
	assert (checkSubstring("0123456789", -1, -1, "0123456789"));
	assert (checkSubstring("0123456789", 10, -1, "0123456789"));
	assert (checkSubstring("0123456789", 5, 1, "0123456789"));
	assert (checkSubstring("0123456789", 0, 10, "0123456789"));
}

bool checkLength(int divider){
	assert (divider > 0);
	
	FixedBuffer<512> buffer;
	for(int i = 0; i < buffer.getSize(); i++){
		buffer+= 'f';
	}
	return strlen(buffer.begin()) == buffer.getLength();
}

bool findString(const char* origin, const char* search){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	
	char* result = strstr(buffer.begin(), search);
	if(result){
		return true;
	}else{
		return false;
	}
}

bool checkEndWith(const char* origin, const char* ending){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	
	if(buffer.endWith(ending)){
		return true;
	}else{
		return false;
	}
}

bool checkStartsWith(const char* origin, const char* start){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	
	if(buffer.startsWith(start)){
		return true;
	}else{
		return false;
	}
}

bool checkIndexOf(const char* origin, const char* search, int expected){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	int index = buffer.indexOf(search);
	
	return index == expected;
}

bool checkTrim(const char* origin, const char* expected){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	buffer.trim();
	assert (buffer.getLength() == strlen(expected));
	
	return strcmp(buffer.getData(), expected) == 0;
}

bool checkRemove(const char* origin, int start, int amount, const char* expected){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	buffer.remove(start, amount);
	assert (buffer.getLength() == strlen(expected));
	
	return strcmp(buffer.getData(), expected) == 0;
}

bool checkSubstring(const char* origin, int from, int to, const char* expected){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	buffer.substring(from, to);
	assert (buffer.getLength() == strlen(expected));
	
	return strcmp(buffer.getData(), expected) == 0;
}

template<int N>
void copyIntoBuffer(FixedBuffer<N>& buffer, const char* data){
	int size = strlen(data);
	assert(size < buffer.getSize());
	
	for(int i = 0; i < size; i++){
		buffer += data[i];
	}
}