#pragma once

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
		
		int getSize(){return size;};
		int getLength(){return length;}
	
	private:
		char data [N];
		const int size = N;
		int length = 0;
	
};
