
#ifndef BUFFER_H
#define BUFFER_H

#include "TableReader.hpp"

// Buffer

template <class T>
class Buffer
{
    struct Fetch : table_fetcher<T>
    {
        Fetch(const T *data, intptr_t size) : table_fetcher<T>(1.0), mData(data), mSize(size) {}
        
        T operator()(intptr_t offset)
        {
            if (offset < 0 || offset >= mSize)
                return 0.0;
            
            return mData[offset];
        }
        
        const T *mData;
        const intptr_t mSize;
    };
    
public:
    
    Buffer() : mBuffer(NULL), mSize(0), mInterpType(kInterpCubicHermite) {}
    
    void read(double* output, double* positions, int numSamps, double amp)
    {
        table_read(Fetch(data(), mSize), output, positions, numSamps, amp, mInterpType);
    }
    
    void setInterpType(InterpType type) { mInterpType = type; }
    void clear() { mSize = 0; }
    T* data() { return mBuffer.data(); }
    
    void resize(int length)
    {
        if (length > mBuffer.size())
            mBuffer.resize(length);
        
        mSize = length;
    }
    
    int getSize() { return mSize; }
    
private:
    
    std::vector<T> mBuffer;
    int mSize;
    InterpType mInterpType;
};

#endif
