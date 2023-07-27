#include "AirportFlights.h"

/**
 * Get the flights for each airport in the 'flightsDB' folder.
 * (from arrival and departure files of each airport)
 */
vector<AirportFlights> AirportFlights::getDatabase()
{
    vector<AirportFlights> res;

    path db_path = "/tmp/flightsDB";

    string curr_path;
    string curr_name;

    if(!is_directory(db_path)) {
        throw runtime_error("No DB exists, please run option 1 to create DB.");
    }

    // Run through all the directories in the airports database.
    for (const auto& entry : directory_iterator(db_path)) {
        if (entry.is_directory()) {
            curr_path = entry.path();

            // Get the last 4 characters of the current path (the name of the airport).
            curr_name = curr_path;
            curr_name = curr_name.substr(curr_name.size() - 4);

            // Get the path to the arrival and departure files.
            string file_arrival_path = curr_path + "/" + curr_name + ".arv";
            string file_departure_path = curr_path + "/" + curr_name + ".dpt";

            // Open the files to receive data.
            ifstream file_arrival(file_arrival_path);
            ifstream file_departure(file_departure_path);

            if(!file_arrival.is_open()) {
                throw runtime_error("No DB exists, please run option 1 to create DB.");
            }

            // Get the flights of the current airport.
            AirportFlights currAirport(curr_name);
            getFlightsFromFile(currAirport, file_arrival, true);
            getFlightsFromFile(currAirport, file_departure, false);

            // Done receiving data from files. closing them...
            file_arrival.close();
            file_departure.close();

            res.push_back(currAirport);
        }
    }

    return res;
}

/*flightStatus: true value stands for arrivals flights, false value for departures. */
void AirportFlights::getFlightsFromFile(AirportFlights &airport, ifstream &file, bool flightStatus)
{
    Flight currFlight;
    string currLine;

    getline(file, currLine); // Read the first line to skip the header.

    while(getline(file, currLine)) {
        // create a stringstream from the current line.
        stringstream ss(currLine);
        string currField;

        // Go through each field separated by commas.
        getline(ss, currField, ','); // Get ICAO24.
        currFlight.setIcao24(currField);

        getline(ss, currField, ','); // Get firstSeen.
        currFlight.setFirstSeen(stoi(currField));

        getline(ss, currField, ','); // Get estDepartureAirport.
        currFlight.setEstDepartureAirport(currField);

        getline(ss, currField, ','); // Get lastSeen.
        currFlight.setLastSeen(stoi(currField));

        getline(ss, currField, ','); // Get estArrivalAirport.
        currFlight.setEstArrivalAirport(currField);

        getline(ss, currField, ','); // Get callsign.
        currFlight.setCallsign(currField);

        // add currFlight into the airport's flights.
        if(flightStatus)
        {
            airport.addArrivalFlight(currFlight);
        }
        else
        {
            airport.addDepartureFlight(currFlight);
        }
    }
}
