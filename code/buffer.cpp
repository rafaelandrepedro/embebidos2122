#include <buffer.h>

template <typename BufferType>
Buffer<BufferType>::Buffer(void) {
	addPtr = 0;
	removePtr = 0;
}

template <typename BufferType>
Buffer<BufferType>::~Buffer(void) {}

template <typename BufferType>
Buffer<BufferType>::add(BufferType value) {
	if ((addPtr + 1 == removePtr) || (addPtr == BUFFER_SIZE && removePtr == 0))
		return 0;
	values[addPtr] = value;
	addPtr++;
	if (addPtr == BUFFER_SIZE)
		addPtr = 0;
	return 1;
}

template <typename BufferType>
Buffer<BufferType>::remove(BufferType* value) {
	if (addPtr == removePtr)
		return 0;
	*value = values[removePtr];
	removePtr++;
	if (removePtr == BUFFER_SIZE)
		removePtr = 0;
	return 1;
}