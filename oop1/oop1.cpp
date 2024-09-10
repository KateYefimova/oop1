#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Airplane {
private:
    int totalSeats;
    string flightNumber;
    string flightDate;
    vector<bool> seatAvailability;
public:
    Airplane(int seats, const string& flightNo, const string& date)
        : totalSeats(seats), flightNumber(flightNo), flightDate(date), seatAvailability(seats, true) {}

    bool isSeatAvailable(int seatNumber) const {
        if (seatNumber < 1 || seatNumber > totalSeats) {
            return false;
        }
        return seatAvailability[seatNumber - 1];
    }

    bool bookSeat(int seatNumber) {
        if (isSeatAvailable(seatNumber)) {
            seatAvailability[seatNumber - 1] = false;
            return true;
        }
        return false;
    }

    int getTotalSeats() const {
        return totalSeats;
    }

    int availableSeats() const {
        int count = 0;
        for (bool available : seatAvailability) {
            if (available) {
                count++;
            }
        }
        return count;
    }
    
    string getFlightNumber() const {
        return flightNumber;
    }

    string getFlightDate() const {
        return flightDate;
    }
};

class Ticket {
private:
    string passengerName;
    int seatNumber;
    string flightInfo;
    string flightDate;
    int ticketID;
    bool isBooked;

public:
    Ticket(string name, int seat, string flight, string date, int id, bool booked = false)
        : passengerName(name), seatNumber(seat), flightInfo(flight), flightDate(date), ticketID(id), isBooked(booked) {}


    void setBookingStatus(bool status) {
        isBooked = status;
    }

    string getPassengerName() const {
        return passengerName;
    }

    int getSeatNumber() const {
        return seatNumber;
    }

    string getFlightNumber() const {
        return flightNumber;
    }

    string getFlightDate() const {
        return flightDate;
    }

    int getTicketID() const {
        return ticketID;
    }

    void printTicketInfo() const {
        cout << "Passenger Name: " << passengerName << endl;
        cout << "Seat Number: " << seatNumber << endl;
        cout << "Flight Information: " << flightInfo << endl;
        cout << "Booking Status: " << (isBooked ? "Confirmed" : "Pending") << endl;
    }
    void returnTicket() {
        isBooked = false;
    }
};

class Flight {
private:
    string flightNumber;
    string flightDate;
    int seatsPerRow;
    vector<pair<int, int>> seatPriceRanges;
    vector<int> prices;

public:
    
    Flight(const string& flightNo, const string& date, int seatsRow)
        : flightNumber(flightNo), flightDate(date), seatsPerRow(seatsRow) {}

    void addPriceRange(int startRow, int endRow, int price) {
        seatPriceRanges.push_back(make_pair(startRow, endRow));
        prices.push_back(price);
    }

    void display() const {
        cout << "Flight Number: " << flightNumber << ", Date: " << flightDate << ", Seats per row: " << seatsPerRow << endl;
        for (size_t i = 0; i < seatPriceRanges.size(); ++i) {
            cout << "Row " << seatPriceRanges[i].first << "-" << seatPriceRanges[i].second
                << " Price: " << prices[i] << "$" << endl;
        }
    }
};

class FlightConfiguration {
private:
    vector<Flight> flights;

public:
    void readConfig(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error: Could not open file: " << filename << endl;
            return;
        }

        string line;
        int numRecords;
        file >> numRecords;
        file.ignore();

        while (getline(file, line)) {
            istringstream iss(line);
            string date, flightNumber;
            int seatsPerRow;
            string range1, range2, price1, price2;

            iss >> date >> flightNumber >> seatsPerRow >> range1 >> price1 >> range2 >> price2;

            Flight flight(date, flightNumber, seatsPerRow);

            int start1, end1, start2, end2, pr1, pr2;
            sscanf_s(range1.c_str(), "%d-%d", &start1, &end1);
            sscanf_s(range2.c_str(), "%d-%d", &start2, &end2);
            pr1 = stoi(price1.substr(0, price1.size() - 1));
            pr2 = stoi(price2.substr(0, price2.size() - 1));

            flight.seatPriceRanges.push_back(make_pair(start1, end1));
            flight.prices.push_back(pr1);
            flight.seatPriceRanges.push_back(make_pair(start2, end2));
            flight.prices.push_back(pr2);

            flights.push_back(flight);
        }

        file.close();
    }

    void displayConfig() const {
        for (const auto& flight : flights) {
            flight.display();
            cout << "-------------------" << endl;
        }
    }
};
class TicketSystem {
private:
    vector<Ticket> tickets; 

public:
    
    void checkFlight(const Flight& flight, const Airplane& airplane) {
        cout << "Available seats for Flight " << flight.getFlightNumber() << " on " << flight.getFlightDate() << ":" << endl;
        cout << "Available Seats: " << airplane.availableSeats() << endl; 
        flight.display(); 
    }

    void bookTicket(Airplane& airplane, const Flight& flight) {
        int seatNumber;
        string passengerName;

        cout << "Enter seat number to book: ";
        cin >> seatNumber;
        cout << "Enter passenger name: ";
        cin.ignore();
        getline(cin, passengerName);

        if (airplane.bookSeat(seatNumber)) {
           
            tickets.emplace_back(passengerName, seatNumber, flight.getFlightNumber(), flight.getFlightDate());
            cout << "Ticket booked successfully! ID: " << tickets.back().getTicketID() << endl;
        }
        else {
            cout << "Seat not available." << endl;
        }
    }


    void returnTicket(Airplane& airplane, int ticketID) {
        auto it = find_if(tickets.begin(), tickets.end(), [ticketID](const Ticket& t) {
            return t.getTicketID() == ticketID;
            });

        if (it != tickets.end()) {
            if (airplane.returnSeat(it->getSeatNumber())) {
                it->returnTicket(); 
                cout << "Ticket returned successfully!" << endl;
            }
            else {
                cout << "Error in returning seat!" << endl;
            }
        }
        else {
            cout << "Ticket not found!" << endl;
        }
    }

  
    void viewTicket(int ticketID) {
        auto it = find_if(tickets.begin(), tickets.end(), [ticketID](const Ticket& t) {
            return t.getTicketID() == ticketID;
            });

        if (it != tickets.end()) {
            it->printTicketInfo();
        }
        else {
            cout << "Ticket not found!" << endl;
        }
    }

   
    void viewTicketsByUser(const string& userName) {
        bool found = false;
        for (const auto& ticket : tickets) {
            if (ticket.getPassengerName() == userName) {
                ticket.printTicketInfo();
                found = true;
            }
        }
        if (!found) {
            cout << "No tickets found for " << userName << endl;
        }
    }

   
    void viewTicketsByFlight(const string& flightNumber, const string& flightDate) {
        bool found = false;
        for (const auto& ticket : tickets) {
            if (ticket.getFlightNumber() == flightNumber && ticket.getFlightDate() == flightDate) {
                ticket.printTicketInfo();
                found = true;
            }
        }
        if (!found) {
            cout << "No tickets found for flight " << flightNumber << " on " << flightDate << endl;
        }
    }
};
int main() {
    FlightConfiguration config;
    wcout.imbue(locale(""));
  
    config.readConfig("C:\Users\Юлія\Source\Repos\oop1\oop1\flight.txt");

    config.displayConfig();

    

    return 0;
}
