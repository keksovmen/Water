#include "BaseParameter.h"
#include "Util.h"


static const int DIGITS_AFTER_DOT = 2;


template<typename T>
int PrimitivIntParameter<T>::getLength(){
	return findLongLength(value);
}


template<typename T>
void PrimitivIntParameter<T>::handleWritingValue(BaseWriter& writer){
	writer.write(value);
}


template<typename T>
int PrimitivFloatParameter<T>::getLength(){
	return findDoubleLength(value, DIGITS_AFTER_DOT);
}


template<typename T>
void PrimitivFloatParameter<T>::handleWritingValue(BaseWriter& writer){
	writer.write(value, DIGITS_AFTER_DOT);
}
