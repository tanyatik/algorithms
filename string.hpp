#include <iostream>
#include <memory>

namespace tanyatik {

class cow_string_t {
private:
    std::shared_ptr<char> data_;
    size_t length_;
    
private:
    template<typename iter_t>
    void clone_data(iter_t begin, iter_t end) {
        size_t length = std::distance(begin, end);

        data_ = std::shared_ptr<char> (new char[length], [] (char *obj) { delete [] obj; });

        std::copy(begin, end, data_.get());

        length_ = length;
        data_.get()[length_ - 1] = '\0';
    }

public:
    cow_string_t() : 
        data_(nullptr),
        length_(0)
        {}

    cow_string_t(const cow_string_t &other) :
        data_(other.data_),
        length_(other.length_)
        {}

    cow_string_t(const char *data) :
        data_(nullptr),
        length_(0) {

        length_ = strlen(data) + 1;
        clone_data(data, data + strlen(data) + 1);
    }

    cow_string_t operator=(const cow_string_t &other) {
        if (this == &other) {
            return *this;
        } 
        data_.reset();

        clone_data(other.data_.get(), other.data_.get() + other.length_);

        return *this;
    } 

    char operator[] (size_t index) const {
        std::cout << "const operator\n";
        return ((const char *) data_.get())[index];
    }  

    char &operator[] (size_t index) {
        std::cout << "non const operator\n";
        auto new_data = std::shared_ptr<char> (new char[length_], [] (char *obj) { delete [] obj; } ); 
        std::copy(data_.get(), data_.get() + length_, new_data.get());

        data_ = new_data;        

        return data_.get()[index];
    }
 
    friend bool operator_equal_impl(const cow_string_t &string, const char *data);
    bool operator==(const char *data) {
        return operator_equal_impl(*this, data);
    }

    const char *get() const {
        return data_.get();
    }

    ~cow_string_t() = default;
};


bool operator_equal_impl(const cow_string_t &string, const char *data) {
    return strcmp(string.data_.get(), data);
}

bool operator==(const char *data, const cow_string_t &string) {
    return operator_equal_impl(string, data);
}

} // namespace tanyatik

