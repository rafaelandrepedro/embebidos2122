#include <iostream>
#include "buffer.h"
#include <unistd.h>

int main()
{
	Buffer<int> buf;
	int a=1;
	int b=2;
    std::cout << "Hello World!" << std::endl;
    while(1){
	buf.add(a);
	buf.add(b);
	buf.remove(&b);
	buf.remove(&a);
        std::cout << a << b << std::endl;
        sleep(1);
    }
    return 0;
}
