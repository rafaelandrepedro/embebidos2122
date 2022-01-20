#ifndef BUFFER_H
#define BUFFER_H


#define BUFFER_SIZE 8

template <class BufferType>
class Buffer {
private:
	BufferType values[BUFFER_SIZE];
	int addPtr, removePtr;
public:
	Buffer();
	~Buffer();
	bool add(BufferType value);
	bool remove(BufferType* value);
	bool check(BufferType* value);
};

template <class BufferType>
Buffer<BufferType>::Buffer() {
	addPtr = 0;
	removePtr = 0;
}

template <class BufferType>
Buffer<BufferType>::~Buffer() {}

template <class BufferType>
bool Buffer<BufferType>::add(BufferType value) {
    if ((addPtr + 1 == removePtr) || (addPtr == BUFFER_SIZE-1 && removePtr == 0))
		return 0;
	values[addPtr] = value;
	addPtr++;
	if (addPtr == BUFFER_SIZE)
		addPtr = 0;
	return 1;
}

template <class BufferType>
bool Buffer<BufferType>::remove(BufferType* value) {
	if (addPtr == removePtr)
		return 0;
	*value = values[removePtr];
	removePtr++;
	if (removePtr == BUFFER_SIZE)
		removePtr = 0;
	return 1;
}

template <class BufferType>
bool Buffer<BufferType>::check(BufferType* value) {
	int checkPtr=addPtr;
	checkPtr--;
	if(checkPtr==-1)
		checkPtr=BUFFER_SIZE-1;
	*value = values[checkPtr];
	return 1;
}

#endif
