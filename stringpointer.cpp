#include <iostream>
#include <string>

class StringPointer
{
    public:
        StringPointer() : ptr(0), homeMade(false) {}
        StringPointer(std::string* ptr): ptr(ptr), homeMade(false) {}

        std::string* operator->()
        {
            if(ptr == 0) 
            {
                homeMade = true;
                ptr = new std::string;
            }

            return ptr;
        }

        operator std::string*() { return ptr; }
        ~StringPointer()
        {
            if(homeMade) delete ptr;
        }

    private:
        std::string* ptr;
        bool homeMade;
};

int main()
{
    std::string s1 = "Hello, world";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    std::cout << sp1->length() << std::endl;
    std::cout << *sp1 << std::endl;
    std::cout << sp2->length() << std::endl;
    std::cout << *sp2 << std::endl;
    return 0;
}
