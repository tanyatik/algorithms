#pragma once

namespace tanyatik {

template<typename, 
         template<class> class,
         template<class> class>
class WeakPointer;

template <class TPointer>
class ReferenceCounter {
private:
    unsigned int *shared_pointer_counter_;
    unsigned int *weak_pointer_counter_;

public:
    ReferenceCounter(int shared_pointer_counter, int weak_pointer_counter) :
        shared_pointer_counter_(new unsigned int(shared_pointer_counter)),
        weak_pointer_counter_(new unsigned int(weak_pointer_counter)) {}

    ReferenceCounter(const ReferenceCounter &other) :
        shared_pointer_counter_(other.shared_pointer_counter_),
        weak_pointer_counter_(other.weak_pointer_counter_){}

    TPointer cloneShared(const TPointer &object) {
        ++(*shared_pointer_counter_);
        return object;
    }

    TPointer cloneWeak(const TPointer &object) {
        ++(*weak_pointer_counter_);
        return object;
    }

    bool releaseShared(const TPointer &) {
        if (!--(*shared_pointer_counter_)) {
            if (!*weak_pointer_counter_) {
                release();
            }
            return true; 
        }
        return false;
    }

    bool releaseWeak(const TPointer &) {
        if(!--(*weak_pointer_counter_)) {
            if (!*shared_pointer_counter_) {
                release();
            }
            return true;
        }
        return false;
    }

    int getSharedCount() const {
        if (shared_pointer_counter_ == nullptr) {
            return 0;
        }
        return *shared_pointer_counter_;
    }
protected:
    ~ReferenceCounter() {}
private:
    void release() {
        delete shared_pointer_counter_;
        shared_pointer_counter_ = nullptr;

        delete weak_pointer_counter_;
        weak_pointer_counter_ = nullptr;
    }
};

template<typename TObject>
class SimpleStorage {
public:
    typedef TObject *TStored;
    typedef TObject *TPointer;
    typedef TObject &TReference;

    SimpleStorage() :
        pointed_object_(getDefaultValue()) {}

    SimpleStorage(const TStored &pointer) :
        pointed_object_(pointer) {}
    friend inline TPointer get(const SimpleStorage &storage) {
        return storage.pointed_object_;
    }
    friend inline const TStored &getReference(const SimpleStorage &storage) {
        return storage.pointed_object_;
    }
    friend inline const TStored &getReference(SimpleStorage &storage) {
        return storage.pointed_object_;
    }
protected:
    TPointer operator ->() const {
        return pointed_object_;
    }

    TReference operator *() const {
        return pointed_object_;
    }

    static TStored getDefaultValue() {
        return nullptr;
    }

    ~SimpleStorage() {}

    TStored pointed_object_;
};

template<typename TObject>
class ArrayStorage : public SimpleStorage<TObject> {
public:

    ArrayStorage(const typename SimpleStorage<TObject>::TStored &pointer) :
        SimpleStorage<TObject>(pointer) {}

protected:
    void destroy() {
        delete [] SimpleStorage<TObject>::pointed_object_;
    }
};

template<typename TObject>
class DefaultStorage : public SimpleStorage<TObject> {

public:
    DefaultStorage(const typename SimpleStorage<TObject>::TStored &pointer) :
        SimpleStorage<TObject>(pointer) {}

protected:
    void destroy() {
        delete SimpleStorage<TObject>::pointed_object_;
    }
};

template<typename TObject, 
         template<class> class TStoragePolicy = DefaultStorage,
         template<class> class TOwnershipPolicy = ReferenceCounter>
class SmartPointer : public TOwnershipPolicy<typename TStoragePolicy<TObject>::TPointer>,
                     public TStoragePolicy<TObject> {
private:

    typedef TStoragePolicy<TObject> TStorage;
    typedef TOwnershipPolicy<typename TStorage::TPointer> TOwnership;
    
    class BoolChecker {
        void operator delete(void *);
    };
public:
    SmartPointer() :
        TOwnership(1, 0),
        TStorage(nullptr) {
    }

    explicit SmartPointer(typename TStorage::TPointer pointed_object_) :
        TOwnership(1, 0),
        TStorage(pointed_object_) {
    }

    SmartPointer (const SmartPointer &other) :
        TOwnership(other), 
        TStorage(TOwnership::cloneShared(getReference(other))) {
    }

    SmartPointer (const WeakPointer<TObject, TStoragePolicy, TOwnershipPolicy> &other) :
        TOwnership(other), 
        TStorage(TOwnership::cloneShared(getReference(other))) {
    }

    SmartPointer &operator= (SmartPointer &other) {
        if (*this != other) {

            bool unique = TOwnership::releaseShared(getReference(*this));
            if (unique) {
                TStorage::destroy();
            }

            TOwnership::operator =(other);
            TStorage::operator = (other.TOwnership::cloneShared(getReference(other)));
        }

        return *this;
    }

    ~SmartPointer() {
        bool unique = TOwnership::releaseShared(get(*this));
        if (unique) {
            TStorage::destroy();
        }
    }

    typename TStorage::TReference operator *() const {
        return TStorage::operator *();
    }

    typename TStorage::TPointer operator ->() const {
        return TStorage::operator->();
    }

    operator BoolChecker *() const {
        if (get(*this) == nullptr) {
            return nullptr;
        }
        static BoolChecker checker;
        return &checker;
    }

    bool operator!() const {
        return get(*this) == nullptr;
    }

    inline friend bool operator == (const SmartPointer &lhs, const TObject *rhs) {
        return get(lhs) == rhs;
    }

    inline friend bool operator == (const TObject *lhs, const SmartPointer &rhs) {
        return lhs == get(rhs);
    }

    inline friend bool operator == (const SmartPointer &lhs, const SmartPointer &rhs) {
        return get(lhs) == get(rhs);
    }

    template<typename TOtherObject>
    inline friend bool operator == (const TOtherObject *lhs, const SmartPointer &rhs) {
        return lhs == get(rhs);
    }

    template<typename TOtherObject>
    inline friend bool operator == (const SmartPointer &lhs, const TOtherObject *rhs) {
        return get(lhs) == rhs;
    }

    template<typename TOtherObject>
    bool operator == (const SmartPointer<TOtherObject> &other) const {
        return get(*this) == get(other);
    }

    inline friend bool operator != (const SmartPointer &lhs, const TObject *rhs) {
        return get(lhs) != rhs;
    }

    inline friend bool operator != (const TObject *lhs, const SmartPointer &rhs) {
        return lhs != get(rhs);
    }

    inline friend bool operator != (const SmartPointer &lhs, const SmartPointer &rhs) {
        return get(lhs) != get(rhs);
    }

    template<typename TOtherObject>
    inline friend bool operator != (const TOtherObject *lhs, const SmartPointer &rhs) {
        return lhs != get(rhs);
    }

    template<typename TOtherObject>
    inline friend bool operator != (const SmartPointer &lhs, const TOtherObject *rhs) {
        return get(lhs) != rhs;
    }

    template<typename TOtherObject>
    bool operator != (const SmartPointer<TOtherObject> &other) const {
        return get(*this) != get(other);
    }

    inline friend bool operator < (const SmartPointer &lhs, const TObject *rhs) {
        return get(lhs) < rhs;
    }

    inline friend bool operator < (const TObject *lhs, const SmartPointer &rhs) {
        return lhs < get(rhs);
    }

    inline friend bool operator < (const SmartPointer &lhs, const SmartPointer &rhs) {
        return get(lhs) < get(rhs);
    }

    template<typename TOtherObject>
    inline friend bool operator < (const TOtherObject *lhs, const SmartPointer &rhs) {
        return lhs < get(rhs);
    }

    template<typename TOtherObject>
    inline friend bool operator < (const SmartPointer &lhs, const TOtherObject *rhs) {
        return get(lhs) < rhs;
    }

    template<typename TOtherObject>
    bool operator < (const SmartPointer<TOtherObject> &other) const {
        return get(*this) == get(other);
    }

}; 

template<typename TObject, 
         template<class> class TStoragePolicy = DefaultStorage,
         template<class> class TOwnershipPolicy = ReferenceCounter>
class WeakPointer : public TOwnershipPolicy<typename TStoragePolicy<TObject>::TPointer>,
                    public TStoragePolicy<TObject> {
private:

    typedef TStoragePolicy<TObject> TStorage;
    typedef TOwnershipPolicy<typename TStorage::TPointer> TOwnership;
    
public:
    WeakPointer() :
        TOwnership(0, 1),
        TStorage(nullptr) {
    }

    explicit WeakPointer(typename TStorage::TPointer pointed_object_) :
        TOwnership(0, 1),
        TStorage(pointed_object_) {
    }

    WeakPointer (const WeakPointer &other) :
          TOwnership(other), 
          TStorage(TOwnership::cloneWeak(getReference(other))) {
    }

    WeakPointer (const SmartPointer<TObject, TStoragePolicy, TOwnershipPolicy> &other) :
          TOwnership(other), 
          TStorage(TOwnership::cloneWeak(getReference(other))) {
    }

    WeakPointer &operator =(WeakPointer &other) {
        if (*this != other) {
            bool unique = TOwnership::releaseWeak(getReference(*this));
            if (unique) {
                TStorage::destroy();
            }
            TOwnership::operator =(other);
            TStorage::operator =(other.TOwnership::cloneWeak(getReference(other)));
        }
        return *this;
    }

    WeakPointer &operator =(SmartPointer<TObject, TStoragePolicy, TOwnershipPolicy> &other) {
        bool unique = TOwnership::releaseWeak(getReference(*this));
        if (unique) {
            TStorage::destroy();
        }
        TOwnership::operator =(other);
        TStorage::operator =(other.TOwnership::cloneWeak(getReference(other)));
        return *this;
    }

    bool expired() const {
        return TOwnership::getSharedCount() == 0;
    }

    SmartPointer<TObject, TStoragePolicy, TOwnershipPolicy> lock() const {
        if (expired()) {
            return SmartPointer<TObject, TStoragePolicy, TOwnershipPolicy>();
        } else {
            return SmartPointer<TObject, TStoragePolicy, TOwnershipPolicy>(*this);
        }
    }

    ~WeakPointer() {
        TOwnership::releaseWeak(get(*this));
    }
};


} // namespace tanyatik
