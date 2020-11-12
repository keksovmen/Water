#pragma once


class FixedBufferBase
{
	public:
		
		virtual FixedBufferBase& operator=(const char* str) = 0;
		virtual ~FixedBufferBase(){};
		
		virtual char& operator[](int index) = 0;
		virtual FixedBufferBase& operator+=(char c) = 0;
		virtual FixedBufferBase& operator--(int) = 0;
		
		virtual char* begin() = 0;
		virtual char* end() = 0;
		
		virtual bool isFull() = 0;
		virtual int remains() = 0;
		
		virtual FixedBufferBase& clear() = 0;
		
		virtual bool endsWith(const char* str) = 0;
		virtual bool startsWith(const char* str) = 0;
		
		virtual int indexOf(const char* str) = 0;
		virtual int indexOfFrom(int from, const char* str) = 0;
		virtual int indexOfEnd(const char* str) = 0;
		
		virtual FixedBufferBase& trim() = 0;
		virtual FixedBufferBase& remove(int index, int amount) = 0;
		virtual bool remove(const char* str) = 0;
		virtual FixedBufferBase& substring(int from, int to = -1) = 0;
		
		virtual char* getData() = 0;
		virtual int getSize() = 0;
		virtual int getLength() = 0;
};