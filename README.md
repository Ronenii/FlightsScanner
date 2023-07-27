# FlightsScanner

## Technologies used

* IDE: Clion
* Programming language: C++ 
* OS: Linux - Ubuntu 22.10
* Flights API: OpenSky REST API
* Containers: Docker

  ## Overview

 The FlightScanner is a C++ terminal application developed in a Linux environment. It allows users to access and manage information about departing and arriving 
 flights from airports worldwide. The application consists of two programs, each running within separate Docker containers:
  * `Flight-Service:` This container hosts the UI program responsible for receiving and managing user data.
  * `DB-Service:` Within this container, the program manages the program's database and its associated functions.

  To facilitate communication between the two containers, a named pipe is used. This setup ensures that both containers are aware of any potential issues or crashes
  in the other container and are capable of managing such situations effectively to maintain smooth operations.

  ## Usage

  After initialize the whole setup, you will be presented with the following menu in the terminal window:
 <p align="center">
  <img src="https://github.com/RoyToledano/ReverseTicTacToe/assets/102805117/a37136ca-3db7-4b8e-99e4-0f7b8eab46e1" width="350">
  </p>

  First, by selecting option 1 you wiil fetch the Database with some initial data, populating it with relevant flight information. The airport names in the Database will be represented by their respective ICAO airport codes, such as "LLBG" for Ben Gurion Airport.

 <p align="center">
  <img src="https://github.com/RoyToledano/ReverseTicTacToe/assets/102805117/35cf4ca7-a9c7-457e-8767-20b4bf571c52" width="350">
  </p>
  
After fetching the Database with some initial data (using function number 1), you are free to utilize any other function provided by the FlightScanner application:
  * `Print airports incoming flights:` Choosing option 2 will prompt you to enter a list of airports' ICAO codes. After entering the codes, the application will display each airport's incoming flights.
  * `Print airport full flight schedule:` Option 3 allows you to input a single airport's ICAO code. Once entered, the application will display the full flight schedule for that airport, ordered by time of arrival/departure.
  * `Print aircrafts full flight schedule:` Option 4 enables you to input an aircraft's ICAO24 code. Upon entering the code, the application will display the full flight schedule for that specific aircraft.
  * `Zip Database files:` Selecting option 5 will trigger the zipping of the current Database files. This process will also occur during the shutdown of both containers, ensuring that the updated Database is preserved for future use.

 <p align="center">
  <img src="https://github.com/RoyToledano/ReverseTicTacToe/assets/102805117/08e190db-6f11-44d5-968f-79f5fc99e889" width="450">
  </p>

 After executing each function, pressing "Enter" will clear the screen, and the menu will reappear, allowing you to perform additional actions as needed.
 By choosing option 6, the application will gracefully shut down both containers after zipping the Database files.

 ## Release

In the release window on the right-hand side of the FlightScanner program, you will have two options to download different setups for the program:

### Setup with DockerHub
 * This setup provides a convenient way to run the FlightScanner application by utilizing DockerHub.
 * You will receive a bash script named "FlightScannerProgram.sh".
 * To start the application, open the terminal and navigate to the directory where the "FlightScannerProgram.sh" is located.
 * Run the following command:
 ```
 ./FlightScannerProgram.sh
 ```
 * This script will pull the required container images from DockerHub and then initiate the two containers necessary for the FlightScanner application.

### Setup manually

 * If the images on DockerHub are unavailable or you prefer more control over the setup process, you can choose the manual setup option.
 * After downloading this setup, you will receive a bash script and two directorys, one for the Flight-Service program and another one for the DB-Service program.
 * First, navigate into the Flight-Service directory using the terminal and execute the bash script to build the corresponding Docker container for the UI program: 
   ```
   ./BuildFlightService.sh
   ```
 * Next, navigate into the DB-Service directory and run its bash script to build the Docker container for the program handling the Database and functions:
   ```
   ./BuildDbService.sh
   ```
 * Once both containers are built, return to the main directory and run the provided bash script to initiate the containers manually:
   ```
    ./FlightScannerProgram.sh
   ```
   

Both setups will require sudo operations as Docker requires administrative privileges to build and run containers. Choose the setup that suits your preferences and system configuration. After successfully setting up the FlightScanner application, you will be able to use the functions mentioned earlier and enjoy the features it provides.

