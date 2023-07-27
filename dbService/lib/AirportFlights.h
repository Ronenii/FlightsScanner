#ifndef SHARED_LIBRARY_AIRPORTFLIGHTS_H
#define SHARED_LIBRARY_AIRPORTFLIGHTS_H

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "Flight.h"

using namespace std;
using namespace filesystem;

class AirportFlights {
private:
    const string name;
    vector<Flight> arrivals;
    vector<Flight> departures;
public:
    const vector<Flight> &getArrivals() const {return arrivals;}
    void setArrivals(const vector<Flight> &_arrivals) {arrivals = _arrivals;}
    const vector<Flight> &getDepartures() const {return departures;}
    void setDepartures(const vector<Flight> &_departures) {departures = _departures;}
    const string getAirportName() const {return name;}
    explicit AirportFlights(const string &_name) : name(_name) {}
    void addArrivalFlight(const Flight &_flight) { arrivals.push_back(_flight); }
    void addDepartureFlight(const Flight &_flight) { departures.push_back(_flight); }
    static vector<AirportFlights> getDatabase();
    static void getFlightsFromFile(AirportFlights &airport, ifstream &file, bool flightStatus);
};


#endif //SHARED_LIBRARY_AIRPORTFLIGHTS_H
