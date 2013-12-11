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
};

template<typename TObject>
class DefaultStorage {

public:
    typedef TObject *TStored;
    typedef TObject *TPointer;
    typedef TObject &TReference;

public:
    DefaultStorage() :
        pointed_object_(getDefaultValue()) {}

    DefaultStorage(const TStored &pointer) :
        pointed_object_(pointer) {}

    friend  inline TPointer get(const DefaultStorage &storage) {
        return storage.pointed_object_;
    }
    friend inline const TStored &getReference(const DefaultStorage &storage) {
        return storage.pointed_object_;
    }
    friend inline const TStored &getReference(DefaultStorage &storage) {
        return storage.pointed_object_;
    }

    TPointer operator ->() const {
        return pointed_object_;
    }

    TReference operator *() const {
        return pointed_object_;
    }

protected:
    void destroy() {
        delete pointed_object_;
    }
    static TStored getDefaultValue() {
        return nullptr;
    }

private:
    TStored pointed_object_;
};

template<typename TObject, 
         template<class> class TOwnershipPolicy = ReferenceCounter,
         template<class> class TStoragePolicy = DefaultStorage>
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
        TOwnership::operator =
            (getImlementation(other));
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
}; 


} // namespace tanyatik
