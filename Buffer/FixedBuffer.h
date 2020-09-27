#pragma once
#ifdef ARDUINO
	#include <Arduino.h>
#else
	#include <string>
	typedef std::string String;
#endif

//TODO: add function that will return 
//string [from - to) and not modify itself
//for parsing purpuses

/**
	Same as String but with fixed length
*/

template<int N>
class FixedBuffer
{
	public:
		char& operator[] (int index){return data[index];}
		
		
		/**
			Instead of append function
			Will put null terminator at the end
		*/
		
		FixedBuffer& operator+= (char c){
			data[length] = c;
			length++;
			data[length] = '\0';
			return *this;
		}
		
		
		//Iterators
		char* begin(){return data;}
		char* end(){return &data[length];}
		
		
		/**
			@return true if there is no more space
		*/
		
		bool isFull(){return length > size;}
		
		
		/**
			Made buffer empty again
			
			@return as builder pattern
		*/
		
		FixedBuffer& clear(){
			length = 0;
			data[0] = '\0';
			
			return *this;
		}
		
		
		/**
			@param str to find at the end
			@return true only if there is match
		*/
		
		bool endsWith(String str){
			int index = length - str.length();
			if(index < 0)
				return false;
			
			for(int i = index, j = 0; i < length; i++, j++){
				if(data[i] != str[j])
					return false;
			}
			return true;
		}
		
		
		/**
			@param str to find at the begining
			@return true only if there is match
		*/
		
		bool startsWith(String str){
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
		
		int indexOf(String str){
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
		
		
		/**
			Deletes all space characters \t\r\f\n\v
			from both ends
			
			Modifies this buffer
			
			@return as builder pattern
		*/
		
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
		
		
		/**
			Modifies this buffer
			
			@param index inclusive
			@param amount how much characters to delete
			
			@return as builder pattern
		*/
		
		FixedBuffer& remove(int index, int amount){
			if(index < 0 || amount < 0)
				return *this;
			
			int sum = index + amount;
			if(index >= length || sum > length || sum == 0)
				return *this;
			
			strcpy(data + index, data + sum);
			length -= amount;
			
			return *this;
		}
		
		
		/**
			Modifies this buffer
			
			@param from inclusive
			@param to exclusive, may not present so 
					end of buffer will be selected 
					
			@return as builder pattern
		*/
		
		FixedBuffer& substring(int from, int to = -1){
			if(from == to || 
					from < 0 || 
					to < -1 ||
					from >= length ||
					to > length)
				return *this;
				
			if(to == -1)
				to = length;
			
			if(from > to)
				return *this;
			
			remove(to, length - to);
			remove(0, from);
			
			return *this;
		}
		
		
		//Getters
		
		char* getData(){return data;};
		int getSize(){return size;};
		int getLength(){return length;};
	
	private:
		//represents max size minus zero terminator
		const int size = N - 1;
		
		//buffer
		char data [N];
		
		//represent current length, data[length] = '\0'
		int length = 0;
	
};
