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
        std::cout << "next" << std::endl;
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
    int* A1_P1 = (int*) A1.Alloc(sizeof(int));
    A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));

    *A1_P1 = 5;
    std::cout << *A1_P1 << std::endl;

    int* A1_P2 = (int*) A1.Alloc(sizeof(int));
    *A1_P2 = 6;
    A1_P2 = (int *) A1.ReAlloc(A1_P2, 2 * sizeof(int));

    std::cout << *A1_P2 << std::endl;

    A1.Free(A1_P2);

    return 0;
}
