#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Airplane {
private:
    int totalSeats;
    vector<bool> seatAvailability;
public:
    Airplane(int seats) : totalSeats(seats), seatAvailability(seats, true) {}

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
};

class Ticket {
private:
    string passengerName;
    int seatNumber;
    string flightInfo;
    bool isBooked;

public:
    Ticket(string name, int seat, string flight, bool booked = false)
        : passengerName(name), seatNumber(seat), flightInfo(flight), isBooked(booked) {}

    void setBookingStatus(bool status) {
        isBooked = status;
    }

    bool getBookingStatus() const {
        return isBooked;
    }

    void printTicketInfo() const {
        cout << "Passenger Name: " << passengerName << endl;
        cout << "Seat Number: " << seatNumber << endl;
        cout << "Flight Information: " << flightInfo << endl;
        cout << "Booking Status: " << (isBooked ? "Confirmed" : "Pending") << endl;
    }
};

class Flight {
public:
    string date;
    string flightNumber;
    int seatsPerRow;
    vector<pair<int, int>> seatPriceRanges;
    vector<int> prices;

    Flight(const string& d, const string& fn, int spr)
        : date(d), flightNumber(fn), seatsPerRow(spr) {}

    void display() const {
        cout << "Flight Number: " << flightNumber << ", Date: " << date << ", Seats per row: " << seatsPerRow << endl;
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
class BookingSystem {
private:
    Airplane airplane;
    vector<Ticket> tickets;

public:
    BookingSystem(int numSeats) : airplane(numSeats) {}

    void checkSeatAvailability() const {
        cout << "Available seats: " << airplane.availableSeats() << endl;
    }

    void bookTicket() {
        int seatNumber;
        string passengerName;
        string flightInfo;

        cout << "Enter seat number to book: ";
        cin >> seatNumber;
        cin.ignore(); // Ignore remaining newline

        cout << "Enter passenger name: ";
        getline(cin, passengerName);

        cout << "Enter flight information: ";
        getline(cin, flightInfo);

        if (airplane.bookSeat(seatNumber)) {
            tickets.emplace_back(passengerName, seatNumber, flightInfo, true);
            cout << "Ticket booked successfully!" << endl;
        }
        else {
            cout << "Seat is not available." << endl;
        }
    }

    void returnTicket() {
        int seatNumber;

        cout << "Enter seat number to return: ";
        cin >> seatNumber;

        if (airplane.returnSeat(seatNumber)) {
            auto it = remove_if(tickets.begin(), tickets.end(), [seatNumber](const Ticket& t) {
                return t.getSeatNumber() == seatNumber;
                });
            tickets.erase(it, tickets.end());
            cout << "Ticket returned successfully!" << endl;
        }
        else {
            cout << "Failed to return seat." << endl;
        }
    }

    void viewBookedTickets() const {
        if (tickets.empty()) {
            cout << "No tickets booked." << endl;
        }
        else {
            for (const auto& ticket : tickets) {
                ticket.printTicketInfo();
                cout << "-------------------" << endl;
            }
        }
    }

    void run() {
        while (true) {
            cout << "1. Check seat availability" << endl;
            cout << "2. Book a ticket" << endl;
            cout << "3. Return a ticket" << endl;
            cout << "4. View booked tickets" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                checkSeatAvailability();
                break;
            case 2:
                bookTicket();
                break;
            case 3:
                returnTicket();
                break;
            case 4:
                viewBookedTickets();
                break;
            case 5:
                cout << "Exiting..." << endl;
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

int main() {
    FlightConfiguration config;
    // Specify the correct file path here
    config.readConfig("flight.txt");

    config.displayConfig();

    Airplane airplane(10);

    Ticket ticket1("John Doe", 3, "Flight 101");
    if (airplane.bookSeat(3)) {
        ticket1.setBookingStatus(true);
    }
    ticket1.printTicketInfo();

    Ticket ticket2("Jane Smith", 3, "Flight 101");
    if (airplane.bookSeat(3)) {
        ticket2.setBookingStatus(true);
    }
    else {
        cout << "Seat 3 is already booked." << endl;
    }
    ticket2.printTicketInfo();

    return 0;
}
