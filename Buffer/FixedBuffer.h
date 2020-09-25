#pragma once
#ifndef ARDUINO
	#include <string>
	typedef std::string string;
#endif


template<int N>
class FixedBuffer
{
	public:
		// FixedBuffer();
		
		char& operator[] (int index){return data[index];}
		
		FixedBuffer& operator+= (char c){
			data[length] = c;
			length++;
			data[length] = '\0';
			return *this;
		}
		
		char* begin(){return data;}
		char* end(){return &data[length];}
		
		bool isFull(){return length >= size;}
		
		bool endWith(string str){
			int index = length - str.length();
			if(index < 0)
				return false;
			
			for(int i = index, j = 0; i < length; i++, j++){
				if(data[i] != str[j])
					return false;
			}
			return true;
		}
		
		int getSize(){return size;};
		int getLength(){return length;}
	
	private:
		char data [N];
		const int size = N;
		int length = 0;
	
};
