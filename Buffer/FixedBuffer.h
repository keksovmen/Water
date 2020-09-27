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
		
		bool startsWith(string str){
			int wordLength = str.length();
			if(wordLength > length)
				return false;
			
			for(int i = 0; i < wordLength; i++){
				if(data[i] != str[i])
					return false;
			}
			return true;
		}
		
		/**
		
		@param str what to find
		@return -1 if there is no coincidence
				index from [0, length) where first letter
					of str located
		*/
		
		int indexOf(string str){
			int wordLength = str.length();
			if(wordLength > length)
				return -1;
			
			#ifdef ARDUINO
				char* find = strstr(data, str);
			#else
				char* find = strstr(data, str.data());
			#endif
			if(find){
				return find - data;
			}
			return -1;
		}
		
		FixedBuffer& trim(){
			if(length == 0)
				return *this;
			
			for(int i = length - 1; i >= 0; i--){
				if(isspace(data[i]) == 0)
					break;
				data[i] = '\0';
				length--;
			}
			
			int count = 0;
			for(int i = 0; i < length; i++){
				if(isspace(data[i]) == 0)
					break;
				count++;
			}
			
			strcpy(data, data + count);
			length -= count;
			return *this;
		}
		
		char* getData(){return data;};
		int getSize(){return size;};
		int getLength(){return length;};
	
	private:
		char data [N];
		const int size = N;
		int length = 0;
	
};
