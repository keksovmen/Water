#pragma once
#include <SoftwareSerial.h>
#include "Buffer/FixedBuffer.h"
#include "BaseWriter.h"

/**
	Handles the lowest IO operations
	Can write and read data directly from port
	Reads data into underlying FixedBuffer
	Try to find your buffer size
	UART has 64 bytes buffer
*/


template<int N>
class SimIOWrapper : public BaseWriter
{
	public:
	//Test instead of link use pointer???
		explicit SimIOWrapper(SoftwareSerial& refSerial);
		
		
		/**
			Sends a command in to the serial port
			Sends termination sequence
			
			@param cmd full comand
			@param clearBuffer define need to clear buffer or not
					flase used only in HTTPREAD to store data some time
		*/
		
		void writeCommand(const char* cmd, bool clearBuffer = true);
		
		
		/**
			Writes a string into the serial port
			Won't send termination sequence
			
			@param str to send
		*/
		
		void write(const char* str) override;
		
		
		/**
			Writes a char into the serial port
			Won't send termination sequence
			
			@param c to send
		*/
		
		void write(char c) override;
		
		
		/**
			Writes an integer into the serial port
			Won't send termination sequence
			
			@param i to send
		*/
		
		virtual void write(int i) override;
		
		/**
			Writes n long into the serial port
			Won't send termination sequence
			
			@param i to send
		*/
		
		void write(long l) override;
		
		
		/**
			Writes a double into the serial port
			Won't send termination sequence
			
			@param d to send
			@param amountAfterDot how much digits after dot
		*/
		
		void write(double d, int amountAfterDot) override;
		
		
		/**
			Writes a termination sequence into the serial port
			
			@param clearBuffer define need to clear buffer or not
					flase used only in HTTPREAD to store data some time
		*/
		
		void writeEndOfCommand(bool clearBuffer = true);
		
		
		/**
			Tries read to the buffer from UART port
			Consumes some little time ~ 25 millis
			
			@return true if at least 1 character was read
					false if timed out
		*/
		
		bool readToBuffer();
		
		
		/**
			Same as readToBuffer() but will wait
			requried time if there is no data,
			and return as soon as possible if there is
			at least 1 character read
			
			@return true if at least 1 character was read
					false if timed out
		*/
		
		bool readToBufferTimeout(int millis);
		
		
		//Getter
		FixedBuffer<N>& getBuffer(){return buffer;};
		
	private:
		
		
		/**
			Tris to read the buffer and return instanteniusly
			
			@return true if at least 1 character was read
					false if timed out
		*/
		
		bool tryReadToBuffer();
		
		
		//sim module here
		SoftwareSerial& refPort;
		
		FixedBuffer<N> buffer;
		
};

template class SimIOWrapper<128>;