#ifndef PROGRAMS_H
#define PROGRAMS_H

#include <iostream>
#include "../lib/AirportFlights.h"
#include <string>
#include "../lib/Functions.h"
#include <vector>
#include <map>
#include <exception>
#include <sys/types.h>
#include <unistd.h>

const int ERROR_CREATING = -1;
const int CHILD_PROCESS_ID = 0;
const int READ_END = 0;
const int WRITE_END = 1;

using namespace std;

string fetchAirportsIncomingFlights(vector<string> airports);

string fetchAirportsFullFlightSchedule(vector<string> flightNames);

string fetchAircraftsFullFlightSchedule(vector<string> aircrafts);

string incomingFLightsToString(const vector<Flight> &flights);

string updateDatabase(vector<string> airportsNames);

string zipDatabaseFiles();

bool compareFlights(const Flight &f1, const Flight &f2, const string &airport_name);

string printFlightsWithICAO24(vector<AirportFlights> &DB, const string &icao24);

string printFlight(const Flight &flight);

string runOnFlightsList(vector<Flight> &flights, const string &icao24);

#endif // PROGRAMS_H