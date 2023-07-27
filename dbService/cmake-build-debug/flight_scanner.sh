#!/bin/bash

BASE_URL="https://opensky-network.org/api/flights/"   # url from which the flights data will be taken.

NOF_PARAMS=$#                     # get the number of input parameters received.
CURR_TIME=$(date +%s)             # current time since epoch.
TIME_24H=$(($CURR_TIME - 86400))  # time since epoch- 24 hours before the current time.

cd /tmp

mkdir -p flightsDB    # create directory where the flights data will be stored.
cd flightsDB          # enter the created directory.

# get each airport's arrival and departure flights:
for param in "$@"; do
  URL_ARRIVAL=$BASE_URL"arrival?airport="$param"&begin="$TIME_24H"&end="$CURR_TIME
  URL_DEPARTURE=$BASE_URL"departure?airport="$param"&begin="$TIME_24H"&end="$CURR_TIME

  # Use wget with --spider and --server-response in order to receive the HTTP response before receiving the data:
  wget --spider --server-response "$URL_ARRIVAL" 2>&1 | grep -q "HTTP/1.1 404"

  # Check if the HTTP response code is 404, means that this airport doesn't exist:
  if [ $? -eq 0 ]; then
    echo "API returned 404"
    exit 1
  fi

  mkdir -p $param  # create directory for the current airport.
  cd $param     # enter the created directory.

  ARRIVAL_FILE=$param.arv
  DEPARTURE_FILE=$param.dpt

  # a title of each colum in the result file.
  TITLE="icao24,firstSeen,estDepartureAirport,lastSeen,estArrivalAirport,callsign"
  echo $TITLE > $param.arv  # save the title into the result file for arrivals and departures.
  echo $TITLE > $param.dpt  # if the file already exists- overwrites it.

  wget -O tmp1.tmp $URL_ARRIVAL   # get arrivals from url.
  RECEIVED_CODE_ARRIVAL=$?
  wget -O tmp2.tmp $URL_DEPARTURE # get departures from url.
  RECEIVED_CODE_DEPARTURE=$?

  if [[ 0 -ne $RECEIVED_CODE_ARRIVAL || 0 -ne $RECEIVED_CODE_DEPARTURE ]]; then   # check for errors in the received message.
    echo "Received error code from URL: arrival- "$RECEIVED_CODE_ARRIVAL", departure- "$RECEIVED_CODE_DEPARTURE

    cd ..         # go back one directory.
    rm -r $param     # delete folder for current airport that got an error.

  else
    # parse the received arrivals' and departures' data in a CSV format.
    grep -o '[^{]*{\([^{}]*\)}' tmp1.tmp | sed 's/{\(.*\)}/{\1}/' | tr -d '[:space:]' | sed -e 's/[{}]/''/g' | tr ',' '\n' | awk -F: '{print $2}' | tr -d '"' | awk '{printf "%s%s", $0, (NR%6 ? "," : "\n")}' | awk 'NR%2==1' > tmp3.tmp
    cat tmp3.tmp >> $param.arv
    grep -o '[^{]*{\([^{}]*\)}' tmp2.tmp | sed 's/{\(.*\)}/{\1}/' | tr -d '[:space:]' | sed -e 's/[{}]/''/g' | tr ',' '\n' | awk -F: '{print $2}' | tr -d '"' | awk '{printf "%s%s", $0, (NR%6 ? "," : "\n")}' | awk 'NR%2==1' > tmp4.tmp
    cat tmp4.tmp >> $param.dpt

    # delete temporary files created.
    rm tmp1.tmp
    rm tmp2.tmp
    rm tmp3.tmp
    rm tmp4.tmp

    cd .. # go back one directory.
  fi
done
