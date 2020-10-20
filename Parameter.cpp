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
}


template<typename T>
void Parameter<T>::handleWritingValue(BaseWriter& writer){
	value.handleWritingValue(writer);
}
