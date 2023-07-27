#include "programs.h"

// PROGRAM 1
// ---------------------------------------------------------------------------------------

// Gets an airport list from the parent process, the child process then returns a string of all incoming flights to airports.
string fetchAirportsIncomingFlights(vector<string> airports) {
    vector<AirportFlights> DB = AirportFlights::getDatabase();
    vector<Flight> arrivals;
    string output, input;
    bool foundAirport;
    int nofAirports;

    // For every airport received:
    nofAirports = airports.size();
    for (int i = 0; i < nofAirports; ++i) {
        foundAirport = false;
        for (auto &airport: DB) {            // Find that airport in the database.
            if (airports[i] == airport.getAirportName()) {// Found the airport
                foundAirport = true;
                arrivals = airport.getArrivals();// Get the arrival flights for the airport.
                output += "Flights arriving for " + airports[i] + ":" + "\n";
                output += incomingFLightsToString(arrivals);
                output += "End of flights arriving for " + airports[i] + "\n";
                break;
            }
        }
        output += "\n";
        if (!foundAirport) {
            output += "Airport with the name " + airports[i] + " not found" + "\n";
        }
    }

    return output;
}


string incomingFLightsToString(const vector<Flight> &flights) {
    string ret = "";
    for (auto &flight: flights) {
        ret += "Flight " + flight.getCallsign() + " arriving from " + flight.getEstDepartureAirport() +
               ", tookoff at " + to_string(flight.getFirstSeen()) + " landed at " + to_string(flight.getLastSeen()) + "\n";
    }

    return ret;
}

// PROGRAM 2
// ---------------------------------------------------------------------------------------

/**
 * Compare two flights by the time of arrival/departure to/from a given airport.
 */
bool compareFlights(const Flight &f1, const Flight &f2, const string &airport_name) {
    int time_f1, time_f2;

    if (f1.getEstArrivalAirport() == airport_name)
        time_f1 = f1.getLastSeen();
    else
        time_f1 = f1.getFirstSeen();

    if (f2.getEstArrivalAirport() == airport_name)
        time_f2 = f2.getLastSeen();
    else
        time_f2 = f2.getFirstSeen();

    return time_f1 < time_f2;
}


// Send the airport name in a vector with 1 name
string fetchAirportsFullFlightSchedule(vector<string> airportNames) {
    string result;

    // Get the database from the files:
    vector<AirportFlights> database = AirportFlights::getDatabase();

    // Get all flights of received airport:
    vector<Flight> airportFlights = Functions::getFlightsByAirportName(airportNames[0], database);

    if(airportFlights.empty())
    {
        result += "Airport with the name " + airportNames[0] + " not found" + "\n";
    }
    else
    {
        // Bubble sort the vector using the compareFlights function:
        for (int i = 0; i < airportFlights.size() - 1; ++i) {
            for (int j = i + 1; j < airportFlights.size(); ++j) {
                if (!compareFlights(airportFlights[i], airportFlights[j], airportNames[0]))
                    swap(airportFlights[i], airportFlights[j]);
            }
        }

        // Print sorted flights details:
        for (Flight &curr: airportFlights) {
            if (curr.isArriving(airportNames[0]))  // Arriving to received airport.
                result += "Flight " + curr.getCallsign() + " arriving from " + curr.getEstDepartureAirport() + " at "
                          + to_string(curr.getTime(airportNames[0])) + "\n";
            else                                   // Departing from received airport.
                result += "Flight " + curr.getCallsign() + " departing to " + curr.getEstArrivalAirport() + " at "
                          + to_string(curr.getTime(airportNames[0])) + "\n";
        }
    }

    return result;
}

// PROGRAM 3
// ---------------------------------------------------------------------------------------
string fetchAircraftsFullFlightSchedule(vector<string> aircrafts) {
    vector<AirportFlights> DB = AirportFlights::getDatabase();
    string ret, midCheck;

    for (int i = 0; i < aircrafts.size(); i++) {
        midCheck = printFlightsWithICAO24(DB, aircrafts[i]);
        if(midCheck.empty())
        {
            ret += "No flights for aircraft " + aircrafts[i] + " found\n";
        }
        else
        {
            ret += midCheck;
        }
    }

    return ret;
}

string printFlightsWithICAO24(vector<AirportFlights> &DB, const string &icao24) {
    string ret, midCheck;
    vector<Flight> flights;

    ret += "\nFlights for aircraft " + icao24 + ":\n";
    for (auto &airport: DB) {
        flights = airport.getDepartures();
        midCheck += runOnFlightsList(flights, icao24);
        flights = airport.getArrivals();
        midCheck += runOnFlightsList(flights, icao24);
    }
    if(midCheck.empty())
    {
        ret.clear();
    }
    else
    {
        ret += midCheck;
    }

    return ret;
}

string runOnFlightsList(vector<Flight> &flights, const string &icao24) {
    string ret;

    for (auto &flight: flights) {
        if (flight.getIcao24() == icao24)
           ret += printFlight(flight);
    }

    return ret;
}

string printFlight(const Flight &flight) {
    string ret;

    ret += flight.getIcao24() + " departed from " + flight.getEstDepartureAirport() + " at " + to_string(flight.getFirstSeen())
            + " arrived in " + flight.getEstArrivalAirport() + " at " + to_string(flight.getLastSeen()) +"\n";

    return ret;
}

// PROGRAM 4
// ---------------------------------------------------------------------------------------
string updateDatabase(vector<string> airportsNames) {
    string result = Functions::updateDatabase(airportsNames);

    return result;
}


// PROGRAM 5
// ---------------------------------------------------------------------------------------
string zipDatabaseFiles() {
    string output;

    // Creates zip of flightsDB folder content
    int exitStatus = system("(cd /tmp && zip -rD /tmp/DB.zip flightsDB)");

    if (exitStatus == 0) {
        output += "Zip command executed successfully.\n";
    } else {
        output += "Error encountered during zip command execution.\n";
    }

    return output;
}

