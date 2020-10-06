#include "Parameter.h"
#include "Util.h"




template<typename T>
Parameter<T>::Parameter(int id) :
	id(id) {

}


template<typename T>
int Parameter<T>::getLength(){
	int idLength = findLongLength(id);
	return idLength + value.getLength();
	// switch(type){
		// case PARAMETER_INT:
			// return idLength + findLongLength(value);
			
		// case PARAMETER_DOUBLE:
			// return idLength + findDoubleLength(value, DIGITS_AFTER_DOT);
			
		// case PARAMETER_USER_DEFINED:
			// return idLength + value.getLength();
			// break;
	// }
}


template<typename T>
void Parameter<T>::handleWritingValue(BaseWriter& writer){
	value.handleWritingValue(writer);
	// switch(type){
		// case PARAMETER_INT:
			// writer.write(value);
			// break;
			
		// case PARAMETER_DOUBLE:
			// writer.write(value, DIGITS_AFTER_DOT);
			// break;
			
		// case PARAMETER_USER_DEFINED:
			// value.handleWritingValue(writer);
			// break;
	// }
}
