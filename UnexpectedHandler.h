#pragma once 
#include "Buffer/FixedBufferBase.h"


class UnexpectedHandler{
	public:
	
		/**
			Children supposed to call parent method for
			delegating action deeper
			
			Basically remove unexpected data in buffer
			and do what you want with it
		*/
		
		virtual void handleUnexpected(FixedBufferBase& refBuffer)
		{
			if(pSuccessor){
				pSuccessor->handleUnexpected(refBuffer);
			}
		}
		
		
		void setSuccessor(UnexpectedHandler* successor)
		{
			this->pSuccessor = successor;
		}
		
		
	private:
		UnexpectedHandler* pSuccessor;
	
};