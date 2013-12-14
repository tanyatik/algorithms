#include <iostream>
#include <vector>

namespace tanyatik {

class Parking {
private:
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

} // namespace tanyatik
