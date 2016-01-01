#include <iostream>

class SmallAllocator {
    private:
        struct PointerInfo
        {
            unsigned int size;
            char* pointer;
            PointerInfo* next;
        } *Pointers; 

        char Memory[1048576];

        char* CurrentPointer;
    public:
        SmallAllocator()
        {
            CurrentPointer = &Memory[0];
            Pointers = NULL;
        }

        void *Alloc(unsigned int Size);
        void *ReAlloc(void *Pointer, unsigned int Size);
        void Free(void *Pointer);
};

void* SmallAllocator::ReAlloc(void *Pointer, unsigned int Size)
{
    if(Pointer == NULL) return Alloc(Size);

    PointerInfo* PointerTmp = Pointers;
    while(PointerTmp != NULL && PointerTmp->pointer != Pointer) 
    {
        PointerTmp = PointerTmp->next;
    }

    if(PointerTmp->size >= Size) {
        PointerTmp->size = Size;
        return Pointer;
    }

    //copy algorithm
    Pointer = Alloc(Size);
    unsigned int diff = PointerTmp->size;

    while(diff > 0) *((char*)Pointer + diff) = *(PointerTmp->pointer + diff), --diff;
    *((char*)Pointer + diff) = *(PointerTmp->pointer + diff);

    Free(PointerTmp->pointer);
    return Pointer;

}

void* SmallAllocator::Alloc(unsigned int Size)
{
    PointerInfo* pi = new PointerInfo;

    pi->next = Pointers;
    pi->size = Size;
    pi->pointer = CurrentPointer;

    Pointers = pi;
    CurrentPointer += Size;

    return pi->pointer;
}

void SmallAllocator::Free(void *Pointer)
{
    if(Pointer == NULL) return;
    if(Pointers == NULL) return;

    PointerInfo* PointersTmp = Pointers;
    if(Pointers->pointer == Pointer)
    {
        Pointers = Pointers->next;
        delete PointersTmp;
        return;
    }

    PointerInfo* PointersPrecedent = Pointers;
    PointersTmp = Pointers->next;

    while(PointersTmp != NULL)
    {
        if(PointersTmp->pointer == Pointer)
        {
            PointersPrecedent->next = PointersTmp->next;
            delete PointersTmp;
            return;
        }

        PointersPrecedent = PointersTmp;
        PointersTmp = PointersTmp->next;
    }
}

int main()
{
    SmallAllocator A1;
    int * A1_P1 = (int *) A1.Alloc(sizeof(int));
    A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
    A1.Free(A1_P1);
    SmallAllocator A2;
    int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
    int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
    A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
    for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 20; i++) if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
    A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
    for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
    A2.Free(A2_P1);
    A2.Free(A2_P2);

    return 0;
}
