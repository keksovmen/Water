#include "Parameter.h"
#include "Util.h"


static const int DIGITS_AFTER_DOT = 2;



template<typename T>
Parameter<T>::Parameter(PARAMETER_TYPES type, int id) :
	id(id), type(type) {

}


template<typename T>
int Parameter<T>::getLength(){
	int idLength = findLongLength(id);
	switch(type){
		case PARAMETER_INT:
			return idLength + findLongLength(value);
			
		case PARAMETER_DOUBLE:
			return idLength + findDoubleLength(value, DIGITS_AFTER_DOT);
			
		case PARAMETER_USER_DEFINED:
			
			break;
	}
}


template<typename T>
void Parameter<T>::handleWritingValue(BaseWriter& writer){
	switch(type){
		case PARAMETER_INT:
			writer.writeInt(value);
			break;
			
		case PARAMETER_DOUBLE:
			writer.writeDouble(value, DIGITS_AFTER_DOT);
			break;
			
		case PARAMETER_USER_DEFINED:
			
			break;
	}
}
