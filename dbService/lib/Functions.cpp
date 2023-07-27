#include "Functions.h"

vector<Flight> Functions::getFlightsByAirportName(string airportName, vector<AirportFlights> &DB)
{
    vector<Flight> res;
    for(const auto& airport : DB)
    {
        if(airport.getAirportName() == airportName)
        {
            pushFlights(airport.getArrivals(), res);
            pushFlights(airport.getDepartures(), res);
            break;
        }
    }

    return res;
}

vector<Flight> Functions::getArrivalsByAirportName(string airportName, vector<AirportFlights> &DB)
{
    vector<Flight> res;
    for(const auto& airport : DB)
    {
        if(airport.getAirportName() == airportName)
        {
            pushFlights(airport.getArrivals(), res);
            break;
        }
    }
    return res;
}

vector<Flight> Functions::getFlightsByCallsign(string callsign, vector<AirportFlights> &DB)
{
    vector<Flight> res, currAirport;
    for(const auto& airport : DB)
    {
        currAirport = airport.getArrivals();
        pushFlightsByCallsign(currAirport,res,callsign);
        currAirport = airport.getDepartures();
        pushFlightsByCallsign(currAirport,res,callsign);
    }
    return res;
}

void Functions::pushFlights(const vector<Flight> &src, vector<Flight> &dst)
{
    for(const auto& flight: src)
    {
        dst.push_back(flight);
    }
}

void Functions::pushFlightsByCallsign(const vector<Flight> &src, vector<Flight> &dst, string callsign)
{
    for(const auto& flight: src)
    {
        if(flight.getCallsign() == callsign)
        {
            dst.push_back(flight);
        }
    }
}

string Functions::updateDatabase(vector<string> airportsNames) {
    path program_path = current_path() / SCRIPT_NAME;
    string result;

    for (const string& airportName : airportsNames) { // Add airports to the path.
        program_path += " " + airportName;
    }

    // Run the script.
    int status = system(program_path.c_str());
    result = "Database update completed.\n";

    int exitCode = WEXITSTATUS(status);
    if(exitCode == 1) // receiving 404 from the api.
    {
        result = "Error: one or more given airports do not exist. Please try again.";
    }
    else if(exitCode != 0) { // Error running the script.
        result = "Error: cannot run the script with the path provided!\n";
    }

    return result;
}
