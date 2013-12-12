#pragma once

namespace tanyatik {

template <class TPointer>
class ReferenceCounter {
private:
    unsigned int *counter_;

public:
    ReferenceCounter() :
        counter_(new unsigned int(1)) {}

    ReferenceCounter(const ReferenceCounter &other) :
        counter_(other.counter_) {}

    TPointer clone(const TPointer &object) {
        ++(*counter_);
        return object;
    }

    bool release(const TPointer &) {
        if (!--(*counter_)) {
            delete counter_;
            counter_ = nullptr;
            return true; 
        }
        return false;
    }
protected:
    ~ReferenceCounter() {}
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
    explicit SmartPointer(typename TStorage::TPointer pointed_object_) :
        TOwnership(),
        TStorage(pointed_object_) {
    }

    SmartPointer (const SmartPointer &other) :
          TOwnership(other), 
          TStorage(TOwnership::clone(getReference(other))) {
    }

    SmartPointer &operator= (SmartPointer &other) {
        bool unique = TOwnership::release(getReference(*this));
        if (unique) {
            TStorage::destroy();
        }

        TOwnership::operator =(other);
        TStorage::operator = (other.TOwnership::clone(getReference(other)));

        return *this;
    }

    ~SmartPointer() {
        bool unique = TOwnership::release(get(*this));
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
    bool operator ==(const SmartPointer<TOtherObject> &other) const {
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
    bool operator !=(const SmartPointer<TOtherObject> &other) const {
        return get(*this) != get(other);
    }
}; 


} // namespace tanyatik
