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

bool checkIndexOfEnd(const char* origin, const char* search, int expected);

bool checkClear(const char* origin);

bool checkTest();

bool testRemove(const char* origin, const char* del, const char* expected);

bool testRemains();

bool testNull();

bool testCopy();


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
	
	assert (checkClear("123456678"));
	
	assert (checkIndexOfEnd("0", "0", 0));
	assert (checkIndexOfEnd("0123456789", "789", 7));
	assert (checkIndexOfEnd("0123456789", "45", 4));
	assert (checkIndexOfEnd("0123456789", "54", -1));
	assert (checkIndexOfEnd("0123456789", "45", 4));
	assert (checkIndexOfEnd("01234567890123", "45", 4));
	assert (checkIndexOfEnd("01234567890123456", "45", 14));
	assert (checkIndexOfEnd("-1.0\r\nOK\r\n\r\nNO CARRIER\r\n", "\r\nOK\r\n", 4));
	// assert (checkIndexOfEnd("\r\nOK\r\n\r\nNO CARRIER\r\n\r\nOK\r\n", "\r\nOK\r\n", ));
	assert (checkIndexOfEnd("\r\nOK\r\n", "\r\nOK\r\n", 0));
	assert (checkIndexOfEnd("\r\nNO CARIER\r\n\r\nOK\r\n", "\r\nOK\r\n", 13));
	assert (checkIndexOfEnd("\r\nOK\r\n\r\nNO CARIER\r\n\r\nOK\r\n", "\r\nOK\r\n", 19));
	assert (checkIndexOfEnd("\r\nOK\r\n\r\nNO CARIER\r\n", "\r\nOK\r\n", 0));
	assert (checkIndexOfEnd("\r\nOK\r\n\r\nOK\r\n", "\r\nOK\r\n", 6));
	assert (checkIndexOfEnd("\r\nOK\r\n\r\nOK\r\n\r\nOK\r\n", "\r\nOK\r\n", 12));
	assert (checkIndexOfEnd("\r\nOK\r\n\r\nOK\r\n\r\nOK\r\n", "\r\nOK\r\n", 12));
	
	assert (checkTest());
	
	assert(testRemove("Temperature - C: 23.60	Pressure - \r\nRING\r\n\r\n+HTTPREAD: 64\r\nmb: 1017.66	Time: 08:10:20:19:01:25\nTemperature - C: 24.22	Pres\r\nOK\r\n", "\r\nRING\r\n", "Temperature - C: 23.60	Pressure - \r\n+HTTPREAD: 64\r\nmb: 1017.66	Time: 08:10:20:19:01:25\nTemperature - C: 24.22	Pres\r\nOK\r\n"));
	assert(testRemove("123_456_789", "_456_", "123789"));
	assert(testRemove("123_456_789", "321", "123_456_789"));
	assert(testRemove("\r\nREAD\r\n\r\n+HTTPREAD: 64\r\n", "\r\nREAD\r\n", "\r\n+HTTPREAD: 64\r\n"));
	assert(testRemove("Temp - C: Pres - \r\nRING\r\n\r\n+HTTPREAD: 64\r\nmb: 10", "\r\nRING\r\n", "Temp - C: Pres - \r\n+HTTPREAD: 64\r\nmb: 10"));
	
	assert(testRemains());
	
	assert(testNull());
	
	assert(testCopy());
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
	
	if(buffer.endsWith(ending)){
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

bool checkClear(const char* origin){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	buffer.clear();
	
	return strlen(buffer.getData()) == buffer.getLength();
}

bool checkIndexOfEnd(const char* origin, const char* search, int expected){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, origin);
	
	return buffer.indexOfEnd(search) == expected;
}

bool checkTest(){
	FixedBuffer<512> buffer;
	
	copyIntoBuffer(buffer, "Temperature - C: 01	Pressure - mb: 1001\nTemperature - C: 02	Pressure - mb: 1002\nTemperature - C: 03	Pressure - mb: 1003\nTemperature - C: 04	Pressure - mb: 1004\n");
	for(int i = 0; i < 4; i++){
		int index = buffer.indexOf("\n");
		index++;
		buffer.substring(index);		
		// std::cout << buffer.begin() << std::endl;
		// std::cout << "INDEX = " << index << "\tLENGTH = "<< buffer.getLength() << std::endl;
	}
	assert (buffer.getLength() == 40);
	return strcmp(buffer.begin(), "") > 0;
}

bool testRemove(const char* origin, const char* del, const char* expected){
	FixedBuffer<130> buffer;
	
	copyIntoBuffer(buffer, origin);
	
	buffer.remove(del);
	
	std::cout << "\nORIGIN:\n" << buffer.begin() << "\nEXPECTED:\n" << expected << std::endl;
	
	int length = strlen(expected);
	std::cout << "ORIGIN LENGTH: " << buffer.getLength() << "\tEXPECTED LENGTH: " << length << std::endl;
	assert (buffer.getLength() == length);
	
	return strcmp(buffer.begin(), expected) == 0;
}


bool testRemains(){
	FixedBuffer<128> buffer;
	
	for(int i = 0; i < 6; i++){
		buffer += 'a';
	}
	
	std::cout << "REMAINS: " << buffer.remains() << std::endl;
	std::cout << "Length: " << buffer.getLength() << std::endl;
	std::cout << "Size: " << buffer.getSize() << std::endl;
	assert(buffer.remains() == 121);
	
	buffer.remove(0, 6);
	assert(buffer.remains() == 127);
	
	std::cout << "REMAINS: " << buffer.remains() << std::endl;
	std::cout << "Length: " << buffer.getLength() << std::endl;
	std::cout << "Size: " << buffer.getSize() << std::endl;
	
	for(int i = 0; i < 127; i++){
		buffer += 'a';
	}
	
	std::cout << "REMAINS: " << buffer.remains() << std::endl;
	std::cout << "Length: " << buffer.getLength() << std::endl;
	std::cout << "Size: " << buffer.getSize() << std::endl;
	assert(buffer.remains() == 0);
	
	buffer.clear();
	assert(buffer.remains() == 127);
	
	copyIntoBuffer(buffer, "\r\nOK\r\n");
	buffer.remove(0, 6);
	assert(buffer.remains() == 127);
	
	return true;
}


bool testNull(){
	FixedBuffer<128> buffer;
	
	copyIntoBuffer(buffer, "#8=25$#9=20$");
	
	int begin = buffer.indexOf("#");
	int end = buffer.indexOf("$");
	assert(begin != -1);
	assert(end != -1);
	int amount = (end - begin + 1);
	std::cout << "END = " << end << "\tAMOUNT = " << amount << std::endl;
	assert(amount == 6);
	buffer.remove(begin, amount);
	// assert();
	assert(buffer.indexOf("#9=20\0") != -1);
	
	return true;
	
}

bool testCopy(){
	FixedBuffer<10> buffer;
	buffer.copy("12345");
	assert(buffer.getLength() == 5);
	assert(buffer.indexOf("3") == 2);
	
	buffer.copy("123");
	assert(buffer.getLength() == 3);
	assert(buffer.indexOf("3") == 2);
	
	buffer.copy("01234567891234567890");
	assert(buffer.getLength() == 9);
	assert(buffer.indexOf("9") == -1);
}


template<int N>
void copyIntoBuffer(FixedBuffer<N>& buffer, const char* data){
	int size = strlen(data);
	std::cout << "BUFFER SIZE: " << buffer.getSize() << "\tDATA SIZE: " << size << std::endl;
	assert(size <= buffer.getSize());
	
	for(int i = 0; i < size; i++){
		buffer += data[i];
	}
}