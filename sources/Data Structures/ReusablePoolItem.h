//
// Created by Hindrik  Stegenga on 30/05/2018.
//

#ifndef VKRENDERER_REUSABLEPOOLITEM_H
#define VKRENDERER_REUSABLEPOOLITEM_H

#include <cstdint>
#include <utility>
#include "Handle.h"

template<typename T, typename HandleType = Handle<T, uint16_t>>
class ReusablePoolItem final {
public:
    ReusablePoolItem()  = default;
    ~ReusablePoolItem() = default;

    ReusablePoolItem(const ReusablePoolItem&)                   = delete;
    ReusablePoolItem(ReusablePoolItem&&) noexcept;

    ReusablePoolItem& operator=(const ReusablePoolItem&)        = delete;
    ReusablePoolItem& operator=(ReusablePoolItem&&) noexcept;
public:
    union Data
    {
    public:
        T object;
        typename HandleType::HandleSizeType nextIndex;
    public:
        //Implement constructor/destructor as no-op, because they NEED to be defined in case of T having a non trivial constructor
        Data() { (void)0; };
        ~Data(){ (void)0; };

        Data(const Data&)       = delete;
        Data(Data&&) noexcept   = delete;

        Data& operator=(const Data&)     = delete;
        Data& operator=(Data&&) noexcept = delete;
    };

    bool isUsed = false;
    alignas(T) Data data;
public:
    template<typename ... Args>
    void reset(Args&& ... args);
    void cleanUp();
};

template<typename T, typename HandleType>
ReusablePoolItem<T, HandleType>& ReusablePoolItem<T, HandleType>::operator=(ReusablePoolItem&& rhs) noexcept {
    cleanUp();
    if(!rhs.isUsed)
    {
        isUsed = false;
        data.nextIndex = rhs.data.nextIndex;
        rhs.data.nextIndex = HandleType::invalidMaxValue;
    }
    else
    {
        isUsed = true;
        data.object = std::move(rhs.data.object);
    }
    return *this;
}

template<typename T, typename HandleType>
ReusablePoolItem<T, HandleType>::ReusablePoolItem(ReusablePoolItem && rhs) noexcept {
    cleanUp();
    if(!rhs.isUsed)
    {
        isUsed = false;
        data.nextIndex = rhs.data.nextIndex;
        rhs.data.nextIndex = HandleType::invalidMaxValue;
    }
    else
    {
        isUsed = true;
        data.object = std::move(rhs.data.object);
    }
}

template<typename T, typename HandleType>
template<typename... Args>
void ReusablePoolItem<T, HandleType>::reset(Args&&... args) {
    cleanUp();
    void* tVoid = &data;
    new (tVoid) T(std::forward<Args>(args)...);
    isUsed = true;
}

template<typename T, typename HandleType>
void ReusablePoolItem<T, HandleType>::cleanUp() {
    if (isUsed) {
        data.object.~T();
        isUsed = false;
    }
}


#endif //VKRENDERER_REUSABLEPOOLITEM_H
