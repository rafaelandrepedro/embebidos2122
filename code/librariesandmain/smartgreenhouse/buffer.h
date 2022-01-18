#ifndef BUFFER_H
#define BUFFER_H


#define BUFFER_SIZE 8

template <typename BufferType>
class Buffer {
private:
	BufferType* values[BUFFER_SIZE];
	int addPtr, removePtr;
public:
	Buffer(void);
	~Buffer(void);
	bool add(BufferType value);
	bool remove(BufferType* value);
};

#endif
