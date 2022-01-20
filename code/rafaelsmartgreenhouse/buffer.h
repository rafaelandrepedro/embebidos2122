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



#endif
