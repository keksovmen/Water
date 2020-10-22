#include "Parameter.h"
#include "Util.h"




template<typename T>
Parameter<T>::Parameter(int id) :
	id(id) {

}


template<typename T>
int Parameter<T>::getLength(){
	int idLength = findLongLength(id);
	//+ 1 from = sign between them
	return idLength + value.getLength() + 1;
}


template<typename T>
void Parameter<T>::handleWritingValue(BaseWriter& writer){
	writer.write(id);
	writer.write('=');
	value.handleWritingValue(writer);
}
