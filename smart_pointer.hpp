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
        std::cout << "refcounter = " << *counter_ << std::endl;
        return object;
    }

    bool release(const TPointer &) {
        if (!--(*counter_)) {
            delete counter_;
            std::cout << "refcounter = 0\n";
            return true;
        }
        std::cout << "refcounter = " << *counter_ << std::endl;
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

protected:
    SimpleStorage() :
        pointed_object_(getDefaultValue()) {}

    SimpleStorage(const TStored &pointer) :
        pointed_object_(pointer) {}

    friend  inline TPointer get(const SimpleStorage &storage) {
        return storage.pointed_object_;
    }
    friend inline const TStored &getReference(const SimpleStorage &storage) {
        return storage.pointed_object_;
    }
    friend inline const TStored &getReference(SimpleStorage &storage) {
        return storage.pointed_object_;
    }

    TPointer operator ->() const {
        return pointed_object_;
    }

    TReference operator *() const {
        return pointed_object_;
    }

protected:
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
                            TStoragePolicy<TObject> {
private:

    typedef TStoragePolicy<TObject> TStorage;
    typedef TOwnershipPolicy<typename TStorage::TPointer> TOwnership;
    
public:
    explicit SmartPointer(typename TStorage::TPointer pointed_object_) :
        TOwnership(),
        TStorage(pointed_object_) {
        std::cout << "SmartPointer dtor\n";              
    }

    SmartPointer (const SmartPointer &other) :
          TOwnership(other), 
          TStorage(TOwnership::clone(getReference(other))) {
        std::cout << "SmartPointer copy\n";          
    }

    SmartPointer &operator= (const SmartPointer &other) {
        TOwnership::operator =(other);
        TStorage::operator =(TOwnership::clone(getReference(other)));
        std::cout << "SmartPointer =\n";          
    }

    ~SmartPointer() {
        bool unique = TOwnership::release(get(*this));
        if (unique) {
            TStorage::destroy();
        }
        std::cout << "SmartPointer dtor\n";          
    }

    typename TStorage::TReference operator *() const {
        return TStorage::operator *();
    }

    typename TStorage::TReference operator ->() const {
        return TStorage::operator->();
    }
private:
    template<typename TAnyPointer>
    TAnyPointer operator &();

    template<typename TAnyPointer>
    operator TAnyPointer *();
}; 


} // namespace tanyatik
