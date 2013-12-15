#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <functional>
#include <memory>
#include <random>
#include <stdexcept>

namespace tanyatik {

namespace algorithms {

template<typename TKey, typename TPriority = int>
class Treap {
private:
    struct TreapNode;

    typedef std::shared_ptr<TreapNode> TreapNodePointer;

    struct TreapNode {
        TreapNodePointer left_;
        TreapNodePointer right_;
        TKey key_;
        TPriority priority_;
    };

    void split(TreapNodePointer node, 
               const TKey &key, 
               TreapNodePointer *result_left, 
               TreapNodePointer *result_right);
    TreapNodePointer merge(TreapNodePointer left, TreapNodePointer right);
    
    void insert(TreapNodePointer *node, TreapNodePointer new_element);
    TreapNodePointer find(TreapNodePointer node, const TKey &key) const;
    TreapNodePointer findThisOrNext(TreapNodePointer node, const TKey &key) const;
    void erase(TreapNodePointer *node, const TKey &key);

    TPriority countRandomPriority() const;

    TreapNodePointer root_;

public:
    // return true if insertion is successful and there is no such element in the treap,
    // false otherwise
    bool insert(const TKey &key);
    bool erase(const TKey &key);

    bool find(const TKey &key) const;
    bool findThisOrNext(const TKey &key, TKey *result) const;

    // Throws std::exception if some property of binary search tree is unsatisfied
    void checkStructure() const;
    void checkStructure(TreapNodePointer node) const;
};

template<typename TKey, typename TPriority>
bool Treap<TKey, TPriority>::find(const TKey &key) const {
    TreapNodePointer found = findThisOrNext(root_, key);
    return (found != nullptr && found->key_ == key);
}

template<typename TKey, typename TPriority>
bool Treap<TKey, TPriority>::findThisOrNext(const TKey &key, TKey *result) const {
    TreapNodePointer found = findThisOrNext(root_, key);
    if (found == nullptr) {
        return false;
    } else {
        *result = found->key_;
        return true;
    }
}

template<typename TKey, typename TPriority>
typename Treap<TKey, TPriority>::TreapNodePointer Treap<TKey, TPriority>::findThisOrNext
        (TreapNodePointer node, const TKey &key) const {
    if (node == nullptr) {
        return nullptr;
    } else if (node->key_ == key) {
        return node;
    } else if (node->key_ < key) {
        TreapNodePointer right_result = findThisOrNext(node->right_, key);
        return right_result;
    } else {
        TreapNodePointer left_result = findThisOrNext(node->left_, key);
        if (left_result == nullptr) {
            return node;
        } else {
            return left_result;
        }
    }
}

template<typename TKey, typename TPriority>
bool Treap<TKey, TPriority>::insert(const TKey &key) {
    if (find(key)) {
        return false;
    }

    TPriority new_priority = countRandomPriority();
    TreapNode new_node = {nullptr, nullptr, key, new_priority};
    TreapNodePointer new_node_pointer(new TreapNode(new_node));

    insert(&root_, new_node_pointer);
    return true;
}

template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::split(TreapNodePointer node, 
                                   const TKey &key,
                                   TreapNodePointer *result_left,
                                   TreapNodePointer *result_right) {
    if (node == nullptr) {
        *result_left = nullptr;
        *result_right = nullptr;
    } else if (node->key_ < key) {

        split(node->right_, key, &node->right_, result_right);
        *result_left = node;
    } else {
        
        split(node->left_, key, result_left, &node->left_);
        *result_right = node;
    }
}

template<typename TKey, typename TPriority>
typename Treap<TKey, TPriority>::TreapNodePointer Treap<TKey, TPriority>::merge
        (TreapNodePointer left, TreapNodePointer right) {
    if (left != nullptr && right == nullptr) {
        return left;

    } else if (left == nullptr) {
        return right;

    } else if (left->key_ < right->key_) {
        right->left_ = merge(left, right->left_);
        return right;

    } else {
        left->right_ = merge(right, left->right_);
        return left;
    }
}

template<typename TKey, typename TPriority>
bool Treap<TKey, TPriority>::erase(const TKey &key) {
    TreapNodePointer node_to_erase = findThisOrNext(root_, key);

    if (node_to_erase == nullptr || node_to_erase->key_ != key) {
        return false;
    } else {
        erase(&root_, key);
        return true;
    }
}


template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::erase(TreapNodePointer *node, const TKey &key) {
    if ((*node)->key_ == key) {
        *node = merge((*node)->left_, (*node)->right_);
    } else if ((*node)->key_ < key) {
        erase(&(*node)->right_, key);
    } else {
        erase(&(*node)->left_, key);
    }
}

template<typename TKey, typename TPriority>
TPriority Treap<TKey, TPriority>::countRandomPriority() const {
    unsigned seed = 237;
    static std::minstd_rand0 generator (seed);

    TPriority max_priority = std::numeric_limits<TPriority>::max();
    return std::uniform_int_distribution<TPriority>(0, max_priority) (generator);
}

template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::insert(TreapNodePointer *node, TreapNodePointer new_element) {
    if (*node == nullptr) {
        *node = new_element;

    } else if ((*node)->key_ == new_element->key_) {
        throw std::runtime_error("Duplicate key in treap\n");

    } else if ((*node)->priority_ < new_element->priority_) {
        split(*node, new_element->key_, &new_element->left_, &new_element->right_);
        *node = new_element;

    } else {
        if ((*node)->key_ < new_element->key_) {
            insert(&(*node)->right_, new_element);

        } else {
            insert(&(*node)->left_, new_element);
        }
    }
}

template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::checkStructure() const {
    checkStructure(root_);
}

template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::checkStructure(TreapNodePointer node) const {
    if (node == nullptr) {
        return;
    } else {
        checkStructure(node->left_);
        checkStructure(node->right_);
        
        if (node->left_ && node->left_->key_ >= node->key_) {
            std::string description = std::string("Key of left ") + 
                                      std::to_string(node->left_->key_) + 
                                      std::string(" is >= than key of root ") + 
                                      std::to_string(node->key_) + 
                                      std::string("\n");
            throw std::runtime_error(description);
        }
        if (node->right_ && node->right_->key_ < node->key_) {
            std::string description = std::string("Key of right ") + 
                                      std::to_string(node->right_->key_) + 
                                      std::string(" is < than key of root ") + 
                                      std::to_string(node->key_) + 
                                      std::string("\n");
            throw std::runtime_error(description);
        }
    }
}

} // namespace algorithms

class Parking {
private:
    typedef tanyatik::algorithms::Treap<int> TFreePlacesTreap;
    TFreePlacesTreap free_places_;

    static const int NO_FREE_PLACE = -1;
    static const int CORRECT_DEPARTURE = 0;
    static const int INCORRECT_DEPARTURE = -2;

public:
    explicit Parking(int places_count);

    int processArrival(int place_number);
    int processDeparture(int place_number);
};

struct ParkingEvent {
    enum Type {
        ARRIVAL,
        DEPARTURE
    };

    Type type;
    int place_number;
};

std::vector<int> processParkingEvents(std::vector<ParkingEvent> events, int parking_places_count);
int readParkingPlacesNumber();
std::vector<ParkingEvent> readParkingEvents();
void writeEventsResult(std::vector<int> results);

int readParkingPlacesNumber() {
    int parking_places_count;
    std::cin >> parking_places_count;
    return parking_places_count;
}

std::vector<ParkingEvent> readParkingEvents() {
    int events_count;
    std::cin >> events_count;

    std::vector<ParkingEvent> events(events_count);

    char event_type_symbol;
    int place_number;
    for (auto &event : events) {
        std::cin >> event_type_symbol >> place_number;
        ParkingEvent new_event;

        if (event_type_symbol == '-') {
            new_event.type = ParkingEvent::Type::DEPARTURE;
        } else if (event_type_symbol == '+') {
            new_event.type = ParkingEvent::Type::ARRIVAL;
        }
        new_event.place_number = place_number;

        event = new_event;
    }

    return events;
}

std::vector<int> processParkingEvents(std::vector<ParkingEvent> events, int parking_places_count) {
    std::vector<int> events_result;

    Parking parking(parking_places_count);

    for (auto event : events) {
        if (event.type == ParkingEvent::Type::ARRIVAL) {
            events_result.push_back(parking.processArrival(event.place_number));
        } else if (event.type == ParkingEvent::Type::DEPARTURE) {
            events_result.push_back(parking.processDeparture(event.place_number));
        }
    }
    return events_result;
}

void writeEventsResult(std::vector<int> results) {
    for (auto result : results) {
        std::cout << result << std::endl;
    }
}

Parking::Parking(int places_count) {
    for (int place_number = 1; place_number <= places_count; ++place_number) {
        free_places_.insert(place_number);
    }
}

int Parking::processArrival(int place_number) {
    int place = 0;
    bool place_found = free_places_.findThisOrNext(place_number, &place);
    if (!place_found) {
        // cyclic parking -- try to look in the beginning
        place_found = free_places_.findThisOrNext(1, &place);
    }

    if (place_found) {
        free_places_.erase(place);
        return place;
    } else {
        return NO_FREE_PLACE;
    }
}

int Parking::processDeparture(int place_number) {
    bool inserted = free_places_.insert(place_number);

    if (inserted) {
        return CORRECT_DEPARTURE;
    } else {
        return INCORRECT_DEPARTURE;
    }
}

} // namespace tanyatik

int main() {
    using tanyatik::readParkingPlacesNumber;
    using tanyatik::readParkingEvents;
    using tanyatik::ParkingEvent;
    using tanyatik::processParkingEvents;
    using tanyatik::writeEventsResult;

    int parking_places_count = readParkingPlacesNumber();
    std::vector<ParkingEvent> events = readParkingEvents();

    std::vector<int> result = processParkingEvents(events, parking_places_count);
    
    writeEventsResult(result);
    return 0;
}

