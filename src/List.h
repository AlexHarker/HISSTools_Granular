
#ifndef LIST_H
#define LIST_H

// Voice Queue

template <class T>
class List
{
    
public:
    
    class Listable
    {
        friend List;
        
    public:
        
        Listable() : mOwner(nullptr), mPrev(nullptr), mNext(nullptr) {}
        
    private:
        
        List *mOwner;
        T* mPrev;
        T* mNext;
    };
    
    class Iterator
    {
        friend List;
        
        Iterator(T* obj) : mObj(obj) {}
        
    public:
        
        Iterator operator ++()
        {
            if (mObj)
                mObj = mObj->mNext == mObj->mOwner->mHead ? nullptr : mObj->mNext;
            return *this;
        }
        
        Iterator operator --()
        {
            if (mObj)
                mObj = mObj == mObj->mOwner->mHead ? nullptr : mObj->mPrev;
            return *this;
        }
        
        Iterator operator ++(int n)
        {
            Iterator prev = *this;
            ++*this;
            return prev;
        }
        
        Iterator operator --(int n)
        {
            Iterator prev = *this;
            --*this;
            return prev;
        }
        
        bool operator ==(Iterator cmp) const { return mObj == cmp.mObj; }
        bool operator !=(Iterator cmp) const { return !(*this == cmp); }
        
        T* operator*() const { return mObj; }
        
    private:
        
        T *mObj;
    };
    
    List() : mHead(nullptr), mSize(0) {}
    
    bool empty() const { return !mHead; }
    int size() const { return mSize; }
    
    void add(T *obj)
    {
        assert(((!obj->mOwner && !obj->mPrev && !obj->mNext) || obj->mOwner == this) && "Queued In Another List");
        
        if (obj->mOwner == this)
            return;
        
        obj->mOwner = this;
        
        if (mHead)
        {
            obj->mNext = mHead;
            obj->mPrev = mHead->mPrev;
            mHead->mPrev = obj;
            obj->mPrev->mNext = obj;
        }
        else
        {
            mHead = obj;
            obj->mPrev = obj;
            obj->mNext = obj;
        }
        
        mSize++;
    }
    
    void remove(T *obj)
    {
        if (!obj->mOwner || obj->mOwner != this)
            return;
        
        obj->mNext->mPrev = obj->mPrev;
        obj->mPrev->mNext = obj->mNext;
        
        if (obj == obj->mNext)
        {
            assert(mSize == 1 && "Wrong Size");
            mHead = nullptr;
        }
        
        if (obj == mHead)
            mHead = obj->mNext;
        
        obj->mOwner = nullptr;
        obj->mPrev = nullptr;
        obj->mNext = nullptr;
        mSize--;
    }
    
    void clear()
    {
        for (auto it = begin(); it != end();)
        {
            T* current = *(it++);
            remove(current);
        }
    }
    
    Iterator begin() { return mHead; }
    Iterator end()  { return nullptr; };
    
private:
    
    int mSize;
    T* mHead;
};


#endif

