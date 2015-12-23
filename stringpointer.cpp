#include <iostream>
#include <string>

#include <iostream>
#include <string>

class StringPointer
{
        public:
            StringPointer(std::string* ptr): ptr(ptr), raw(false) {}
            std::string* operator->()
            {
                checkPtr();
                return ptr;
            }

            std::string operator*()
            {
                checkPtr();
                return *ptr; 
            }

            operator std::string*() 
            { 
                checkPtr();
                return ptr; 
            }
            ~StringPointer()
            {
                if(raw)
                {
                    delete ptr;
                }
            }

            private:
                void checkPtr()
                {
                    if(ptr == NULL) 
                    {
                        raw = true;
                        ptr = new std::string();
                    }
                }
                std::string* ptr;
                bool raw;
};

int main()
{
    std::string s1 = "Hello, world";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    std::cout << "sp1->length(): " << sp1->length() << std::endl;
    std::cout << "*sp1: " << *sp1 << std::endl;
    std::cout << "sp2->length: " << sp2->length() << std::endl;
    std::cout << "*sp2: " << (*sp2).length() << std::endl;
    return 0;
}
