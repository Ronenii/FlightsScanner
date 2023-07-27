#include "Flight.h"

const string &Flight::getIcao24() const {
    return icao24;
}

void Flight::setIcao24(const string &_icao24) {
    icao24 = _icao24;
}

int Flight::getFirstSeen() const {
    return firstSeen;
}

void Flight::setFirstSeen(int _firstSeen) {
    firstSeen = _firstSeen;
}

int Flight::getLastSeen() const {
    return lastSeen;
}

void Flight::setLastSeen(int _lastSeen) {
    lastSeen = _lastSeen;
}

const string &Flight::getEstDepartureAirport() const {
    return estDepartureAirport;
}

void Flight::setEstDepartureAirport(const string &_estDepartureAirport) {
    estDepartureAirport = _estDepartureAirport;
}

const string &Flight::getEstArrivalAirport() const {
    return estArrivalAirport;
}

void Flight::setEstArrivalAirport(const string &_estArrivalAirport) {
    estArrivalAirport = _estArrivalAirport;
}

const string &Flight::getCallsign() const {
    return callsign;
}

void Flight::setCallsign(const string &_callsign) {
    callsign = _callsign;
}

/**
 * Get the time of arrival/departure to/from a given airport.
 */
int Flight::getTime(const string &airport_name) {
    if(estArrivalAirport == airport_name)
        return lastSeen;
    else
        return firstSeen;
}

/**
 * Get if the flight
 */
bool Flight::isArriving(const string &airport_name) {
    return estArrivalAirport == airport_name;
}


