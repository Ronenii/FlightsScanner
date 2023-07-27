#ifndef SHARED_LIBRARY_FLIGHT_H
#define SHARED_LIBRARY_FLIGHT_H

#include <string>
#include <iostream>

using namespace std;

class Flight {
    string icao24;
    int firstSeen;
    int lastSeen;
    string estDepartureAirport;
    string estArrivalAirport;
    string callsign;

public:
    const string &getIcao24() const;

    void setIcao24(const string &icao24);

    int getFirstSeen() const;

    void setFirstSeen(int _firstSeen);

    int getLastSeen() const;

    void setLastSeen(int _lastSeen);

    const string &getEstDepartureAirport() const;

    void setEstDepartureAirport(const string &_estDepartureAirport);

    const string &getEstArrivalAirport() const;

    void setEstArrivalAirport(const string &_estArrivalAirport);

    const string &getCallsign() const;

    void setCallsign(const string &_callsign);

    void print() const {
        cout << icao24 << ", " << firstSeen << ", " << estDepartureAirport << ", " << lastSeen << ", "
             << estArrivalAirport << ", " << callsign << endl;
    }

    int getTime(const string &airport_name);

    bool isArriving(const string &airport_name);
};

#endif //SHARED_LIBRARY_FLIGHT_H
