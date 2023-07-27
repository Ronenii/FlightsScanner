#ifndef SHARED_LIBRARY_FUNCTIONS_H
#define SHARED_LIBRARY_FUNCTIONS_H

#include "AirportFlights.h"
#include "Flight.h"
#include <unistd.h>
#define SCRIPT_NAME "flight_scanner.sh"

class Functions {
public:
    static vector<Flight> getFlightsByAirportName(string airportName, vector<AirportFlights> &DB);
    static void pushFlights(const vector<Flight> &src, vector<Flight> &dst);
    static vector<Flight> getArrivalsByAirportName(string airportName, vector<AirportFlights> &DB);
    static vector<Flight> getFlightsByCallsign(string callsign, vector<AirportFlights> &DB);
    static void pushFlightsByCallsign(const vector<Flight> &src, vector<Flight> &dst, string callsign);
    static string updateDatabase(vector<string> airportsNames);
};



#endif //SHARED_LIBRARY_FUNCTIONS_H
