
#ifndef BUFFER_H
#define BUFFER_H

#include "HISSTools_Library/TableReader.hpp"
#include "IPlugStructs.h"

// Buffer

template <class T>
class Buffer
{
    struct Fetch : table_fetcher<T>
    {
        Fetch(const T *data, intptr_t size) : table_fetcher<T>(size, 1.0), mData(data), mSize(size) {}
        
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
    
    Buffer() : mBuffer(NULL), mSize(0), mInterpType(InterpType::CubicHermite) {}
    
    void read(double* output, double* positions, int numSamps, double amp)
    {
        table_read(Fetch(data(), mSize), output, positions, numSamps, amp, mInterpType);
    }
    
    void setInterpType(InterpType type) { mInterpType = type; }
    void clear() { mSize = 0; }
    T* data() { return mBuffer.data(); }
    const T* data() const { return mBuffer.data(); }
    
    void resize(int length)
    {
        if (length > mBuffer.size())
            mBuffer.resize(length);
        
        mSize = length;
    }
    
    int getSize() const { return mSize; }
    
    void save(iplug::IByteChunk &storage) const
    {
        storage.Put(&mSize);
        storage.PutBytes(data(), mSize * sizeof(float));
    }
    
    int recall(const iplug::IByteChunk &storage, int pos)
    {
        int size;
        
        pos = storage.Get(&size, pos);
        resize(size);
        return storage.GetBytes(data(), size * sizeof(T), pos);
    }
    
private:
    
    std::vector<T> mBuffer;
    int mSize;
    InterpType mInterpType;
};

#endif

