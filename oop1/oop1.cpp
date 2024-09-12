#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

class Ticket;  // Forward declaration


class Seat {
public:
    int seatNumber;
    int price;
    bool isBooked;

    Seat() : seatNumber(0), price(0), isBooked(false) {}

    Seat(int number, int price, bool booked = false)
        : seatNumber(number), price(price), isBooked(booked) {}

    bool isAvailable() const {
        return !isBooked;
    }

    void book() {
        isBooked = true;
    }

    void cancel() {
        isBooked = false;
    }
};


class Airplane {
private:
    int totalSeats;
    map<int, Seat> seats;
    string flightNumber;
    string date;

public:
    Airplane() : totalSeats(0) {}

    Airplane(int totalSeats, const string& flightNum, const string& d, const map<int, Seat>& seatMap)
        : totalSeats(totalSeats), flightNumber(flightNum), date(d), seats(seatMap) {}
    void setTotalSeats(int seats) {
        totalSeats = seats;
    }

    void setFlightNumber(const string& flightNum) {
        flightNumber = flightNum;
    }

    void setDate(const string& d) {
        date = d;
    }

    string getDate() const {
        return date;
    }

    string getFlightNumber() const {
        return flightNumber;
    }

    bool isSeatAvailable(int seatNumber) const {
        auto it = seats.find(seatNumber);
        return it != seats.end() && it->second.isAvailable();
    }

    bool bookSeat(int seatNumber) {
        auto it = seats.find(seatNumber);
        if (it != seats.end() && it->second.isAvailable()) {
            it->second.book();
            return true;
        }
        return false;
    }

    bool returnSeat(int seatNumber) {
        auto it = seats.find(seatNumber);
        if (it != seats.end() && !it->second.isAvailable()) {
            it->second.cancel();
            return true;
        }
        return false;
    }

    void setSeatPrice(int start, int end, int price) {
        for (int i = start; i <= end; ++i) {
            if (seats.find(i) != seats.end()) {
                seats[i].price = price;
            }
        }
    }

    void listAvailableSeatsWithPrices() const {
        if (seats.empty()) {
            cout << "No seats available111." << endl;
            return;
        }

        bool hasAvailableSeats = false;
        for (const auto& pair : seats) {
            const Seat& seat = pair.second;
            if (seat.isAvailable()) {
                cout << "Seat " << seat.seatNumber << ": $" << seat.price << endl;
                hasAvailableSeats = true;
            }
        }
        if (!hasAvailableSeats) {
            cout << "No available seats." << endl;
        }
    }



    void display() const {
        cout << "Flight Number: " << flightNumber << endl;
        cout << "Date: " << date << endl;
        for (const auto& pair : seats) {
            const Seat& seat = pair.second;
            cout << "Seat " << seat.seatNumber << ": "
                << (seat.isAvailable() ? "Available" : "Booked")
                << ", Price: $" << seat.price << endl;
        }
    }
};


class Ticket {
public:
    string passengerName;
    int seatNumber;
    string flightInfo;
    string date;
    int confirmationID;

public:
    Ticket(string name, int seat, string flight, string d, int id)
        : passengerName(name), seatNumber(seat), flightInfo(flight), date(d), confirmationID(id) {}

    int getSeatNumber() const {
        return seatNumber;
    }

    string getPassengerName() const {
        return passengerName;
    }

    int getConfirmationID() const {
        return confirmationID;
    }

    string getFlightInfo() const {
        return flightInfo;
    }

    string getDate() const {
        return date;
    }

    void printTicketInfo() const {
        cout << "Passenger Name: " << passengerName << endl;
        cout << "Seat Number: " << seatNumber << endl;
        cout << "Flight Information: " << flightInfo << endl;
        cout << "Date: " << date << endl;
        cout << "Confirmation ID: " << confirmationID << endl;
    }

    bool isMatchByDate(const string& searchDate) const {
        return date == searchDate;
    }

    bool isMatchByConfirmationID(int searchID) const {
        return confirmationID == searchID;
    }
};


class Passenger {
public:
    string passengerName;
    vector<Ticket> tickets;

    Passenger(const string& name) : passengerName(name) {}

    void addTicket(const Ticket& ticket) {
        tickets.push_back(ticket);
    }

    void viewTicketsByUsername(const string& username) const {
        if (username == passengerName) {
            cout << "Tickets for " << passengerName << ":" << endl;
            for (const auto& ticket : tickets) {
                ticket.printTicketInfo();
            }
        }
        else {
            cout << "No tickets found for user: " << username << endl;
        }
    }
};


class FlightConfiguration {
private:
    vector<Airplane*> airplanes;

public:
    void readConfig(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
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

            int start1, end1, start2, end2;
            sscanf_s(range1.c_str(), "%d-%d", &start1, &end1);
            sscanf_s(range2.c_str(), "%d-%d", &start2, &end2);

            int numSeats = end2 * seatsPerRow; 

            map<int, Seat> seatMap;
            for (int i = 1; i <= numSeats; ++i) {
                seatMap[i] = Seat(i, 100); 
            }

            int pr1 = stoi(price1.substr(0, price1.size() - 1)); 
            int pr2 = stoi(price2.substr(0, price2.size() - 1));

            /
            Airplane* airplane = new Airplane(numSeats, flightNumber, date, seatMap);
            end1 = end1 * seatsPerRow;
            end2 = end2 * seatsPerRow;
            start2 = end1 + 1;
            airplane->setSeatPrice(start1, end1, pr1);
            airplane->setSeatPrice(start2, end2, pr2);

            airplanes.push_back(airplane);
        }

        file.close();
    }




    vector<Airplane*> getAirplanes() const {
        return airplanes;
    }

    void displayConfig() const {
        for (const auto& airplane : airplanes) {
            airplane->display();
            cout << "-------------------" << endl;
        }
    }
};
class BookingSystem {
private:
    vector<Airplane*> airplanes;
    vector<Ticket> tickets;
    vector<Passenger> passengers;  
    int nextConfirmationID;

public:
    BookingSystem() : nextConfirmationID(12345) {}

    ~BookingSystem() {
        for (Airplane* airplane : airplanes) {
            delete airplane;
        }
    }

    void loadFlights(const vector<Airplane*>& flights) {
        for (const auto& flight : flights) {
            cout << "Loading flight: " << flight->getFlightNumber() <<flight->getDate()<< endl;
            airplanes.push_back(flight);
        }
    }

    Passenger* findPassenger(const string& name) {
        for (auto& passenger : passengers) {
            if (passenger.passengerName == name) {
                return &passenger;
            }
        }
        return nullptr;
    }

    Passenger* createPassenger(const string& name) {
        passengers.push_back(Passenger(name));
        return &passengers.back();
    }

    void checkSeatAvailability() {
        string date, flightNumber;
        cout << "Enter date and flight number: ";
        cin >> date >> flightNumber;

        bool found = false;
        for (const auto& airplane : airplanes) {
            
            if (airplane->getFlightNumber() == flightNumber && airplane->getDate() == date) {
                airplane->display();
                cout << "Available seats for flight " << flightNumber << " on " << date << ":" << endl;
                airplane->listAvailableSeatsWithPrices();
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Flight not found or no available seats for the specified date and flight number." << endl;
        }
    }



    void bookTicket() {
        string date, flightNumber, username;
        int seatNumber;
        cout << "Enter date, flight number, seat number, and username: ";
        cin >> date >> flightNumber >> seatNumber >> username;

        Passenger* passenger = findPassenger(username);
        if (!passenger) {
            passenger = createPassenger(username);
        }

        for (const auto& airplane : airplanes) {
            if (airplane->getFlightNumber() == flightNumber && airplane->getDate() == date) {
                if (airplane->isSeatAvailable(seatNumber)) {
                    if (airplane->bookSeat(seatNumber)) {
                        Ticket ticket(username, seatNumber, flightNumber, date, nextConfirmationID++);
                        tickets.push_back(ticket);
                        passenger->addTicket(ticket);  // Додаємо квиток до пасажира
                        cout << "Booking confirmed with ID " << ticket.getConfirmationID() << endl;
                        return;
                    }
                    else {
                        cout << "Failed to book seat." << endl;
                        return;
                    }
                }
                else {
                    cout << "Seat not available." << endl;
                    return;
                }
            }
        }

        cout << "Flight not found." << endl;
    }

    void returnTicket() {
        int confirmationID;
        cout << "Enter confirmation ID: ";
        cin >> confirmationID;

        // Дебаговий вивід всіх квитків
        cout << "Tickets:" << endl;
        for (const auto& ticket : tickets) {
            ticket.printTicketInfo();
        }

        auto it = find_if(tickets.begin(), tickets.end(), [confirmationID](const Ticket& ticket) {
            return ticket.getConfirmationID() == confirmationID;
            });

        if (it != tickets.end()) {
            // Запам'ятати дані квитка для подальшої перевірки
            const Ticket& ticketToReturn = *it;

            for (auto& airplane : airplanes) {
                if (airplane->getFlightNumber() == ticketToReturn.getFlightInfo() && airplane->getDate() == ticketToReturn.getDate()) {
                    if (airplane->returnSeat(ticketToReturn.getSeatNumber())) {
                        // Видалення квитка після успішного повернення
                        tickets.erase(it);

                        // Вивести інформацію про повернення квитка
                        cout << "Ticket returned. Refund: $" << ticketToReturn.getSeatNumber() << endl;
                        return;
                    }
                    else {
                        cout << "Failed to return seat." << endl;
                        return;
                    }
                }
            }
            cout << "Flight not found." << endl;
        }
        else {
            cout << "Ticket not found." << endl;
        }
    }

    void viewTicket() {
        int confirmationID;
        cout << "Enter confirmation ID: ";
        cin >> confirmationID;

        auto it = find_if(tickets.begin(), tickets.end(), [confirmationID](const Ticket& ticket) {
            return ticket.getConfirmationID() == confirmationID;
            });

        if (it != tickets.end()) {
            it->printTicketInfo();
        }
        else {
            cout << "Ticket not found." << endl;
        }
    }

    void viewTicketsByUsername() {
        string username;
        cout << "Enter username: ";
        cin >> username;

        Passenger* passenger = findPassenger(username);
        if (passenger) {
            passenger->viewTicketsByUsername(username);
        }
        else {
            cout << "No tickets found for user: " << username << endl;
        }
    }

    void viewTicketsByFlight() {
        string date, flightNumber;
        cout << "Enter date and flight number: ";
        cin >> date >> flightNumber;

        for (const auto& ticket : tickets) {
            if (ticket.getFlightInfo() == flightNumber && ticket.getDate() == date) {
                cout << "Seat " << ticket.getSeatNumber() << ", Username " << ticket.getPassengerName()
                    << ", Price " << ticket.getSeatNumber() << "$" << endl;
            }
        }
    }

    void run() {
        while (true) {
            cout << "1. Check seat availability" << endl;
            cout << "2. Book a ticket" << endl;
            cout << "3. Return a ticket" << endl;
            cout << "4. View the booking by ID" << endl;
            cout << "5. View booked tickets by UserName" << endl;
            cout << "6. View booked tickets by Date, FlightNo" << endl;
            cout << "7. Exit" << endl;
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
                viewTicket();
                break;
            case 5:
                viewTicketsByUsername();
                break;
            case 6:
                viewTicketsByFlight();
                break;
            case 7:
                exit(0);
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};



int main() {
    
    FlightConfiguration config;
    BookingSystem system;
    config.readConfig("flight.txt");
    system.loadFlights(config.getAirplanes());
    system.run();
    

    return 0;
}
