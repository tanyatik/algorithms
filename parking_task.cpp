#include <iostream>
#include <vector>

#include "treap.hpp"

namespace tanyatik {

class Parking {
private:
    typedef tanyatik::Treap<int> TFreePlacesTreap;
    TFreePlacesTreap free_places_;

    static const int NO_FREE_PLACE = -1;
    static const int CORRECT_DEPARTURE = 0;
    static const int INCORRECT_DEPARTURE = -2;

public:
    Parking(int places_count);

    int processArrival(int place_number);
    int processDeparture(int place_number);
};

struct ParkingEvent{
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

int main() {
    int parking_places_count = readParkingPlacesNumber();
    std::vector<ParkingEvent> events = readParkingEvents();

    std::vector<int> result = processParkingEvents(events, parking_places_count);
    
    writeEventsResult(result);
    return 0;
}

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
    for (auto event : events) {
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
    bool place_found = free_places_.find([place_number] (int place) { return place >= place_number; },
                                         &place);
    if (!place_found) {
        // cyclic parking -- try to look in the beginning
        place_found = free_places_.find([place_number] (int place) { return place >= 1; },
                                        &place);
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
