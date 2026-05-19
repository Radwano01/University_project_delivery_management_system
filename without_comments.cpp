#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <limits>

using namespace std;

class DeliveryException : public exception
{
    string message;

public:
    DeliveryException(string msg) : message(msg) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

class Delivery
{
protected:
    int orderId;
    string customerName;
    string driverName;
    string pickupAddress;
    string dropAddress;
    double distance;
    int etaMinutes;

public:
    Delivery(int id, string customer, string driver,
        string pickup, string drop,
        double dist, int eta)
    {
        orderId = id;
        customerName = customer;
        driverName = driver;
        pickupAddress = pickup;
        dropAddress = drop;
        distance = dist;
        etaMinutes = eta;
    }

    virtual double calculateFee() = 0;

    int getOrderId() { return orderId; }
    string getCustomer() { return customerName; }
    string getDriver() { return driverName; }
    string getPickup() { return pickupAddress; }
    string getDrop() { return dropAddress; }
    double getDistance() { return distance; }
    int getEta() { return etaMinutes; }

    void setDriver(string d) { driverName = d; }
    void setPickup(string p) { pickupAddress = p; }
    void setDrop(string d) { dropAddress = d; }
    void setDistance(double d) { distance = d; }
    void setEta(int e) { etaMinutes = e; }

    virtual void printRow()
    {
        cout << left
            << setw(10) << orderId
            << setw(15) << customerName
            << setw(15) << driverName
            << setw(12) << fixed << setprecision(2) << distance
            << setw(12) << fixed << setprecision(2) << calculateFee()
            << endl;
    }

    virtual void printDetails()
    {
        cout << "\n=========== ORDER DETAILS ===========\n";
        cout << left << setw(20) << "Order ID:" << orderId << endl;
        cout << left << setw(20) << "Customer:" << customerName << endl;
        cout << left << setw(20) << "Driver:" << driverName << endl;
        cout << left << setw(20) << "Pickup:" << pickupAddress << endl;
        cout << left << setw(20) << "Drop:" << dropAddress << endl;
        cout << left << setw(20) << "Distance:"
            << fixed << setprecision(2) << distance << " km" << endl;
        cout << left << setw(20) << "Fee:"
            << fixed << setprecision(2) << calculateFee() << endl;
        cout << left << setw(20) << "ETA:" << etaMinutes << " min" << endl;
        cout << "====================================\n";
    }

    virtual ~Delivery() {}
};


class StandardDelivery : public Delivery
{
public:
    StandardDelivery(int id, string customer, string driver,
        string pickup, string drop,
        double dist, int eta)
        : Delivery(id, customer, driver, pickup, drop, dist, eta) {
    }

    double calculateFee() override
    {
        return distance * 2.5;
    }
};



class OrderSystem
{
private:
    const string filename = (filesystem::current_path() / "orders.txt").string();

    static string serializeOrder(Delivery* o)
    {
        ostringstream ss;
        ss << o->getOrderId() << "|"
            << o->getCustomer() << "|"
            << o->getDriver() << "|"
            << o->getPickup() << "|"
            << o->getDrop() << "|"
            << fixed << setprecision(2) << o->getDistance() << "|"
            << o->getEta();
        return ss.str();
    }

    static bool parseLine(const string& line, int& id, string& customer, string& driver,
        string& pickup, string& drop, double& distance, int& eta)
    {
        vector<string> parts;
        string part;
        istringstream ss(line);
        while (getline(ss, part, '|')) parts.push_back(part);
        if (parts.size() != 7) return false;
        try {
            id = stoi(parts[0]);
            customer = parts[1];
            driver = parts[2];
            pickup = parts[3];
            drop = parts[4];
            distance = stod(parts[5]);
            eta = stoi(parts[6]);
        }
        catch (...) { return false; }
        return true;
    }

    int getNextId()
    {
        ifstream fin(filename);
        int maxId = 0;

        string line;
        while (getline(fin, line))
        {
            int id; string a, b, c, d, e; double dist; int eta;
            if (parseLine(line, id, a, b, c, d, dist, eta))
            {
                if (id > maxId)
                    maxId = id;

            }
        }
        return maxId + 1;
    }

public:
    string currentUser;
    bool isAdmin = false;
    OrderSystem() { ofstream f(filename, ios::app); }

    void addOrder(Delivery* o)
    {
        ofstream fout(filename, ios::app);
        if (!fout) { cout << "Error: cannot open orders file for writing.\n"; delete o; return; }
        fout << serializeOrder(o) << "\n";
        fout.close();
        delete o;
    }

    bool isDuplicateId(int id)
    {
        ifstream fin(filename);
        if (!fin) return false;
        string line;
        while (getline(fin, line))
        {
            int fid; string a, b, c; string d, e; double dist; int eta;
            if (parseLine(line, fid, a, b, d, e, dist, eta))
            {
                if (fid == id) return true;
            }
        }
        return false;
    }

    void createOrder()
    {
        int id = getNextId(), eta;
        string customer, driver;
        string pickup, drop;
        double distance;

        cout << "\n--- CREATE ORDER ---\n";
        cout << "Generated Order ID: " << id << "\n";

        if (isDuplicateId(id)) {
            cout << "Error: Order ID already exists!\n";
            cin.ignore();
            cin.get();
            return;
        }

        cout << "Customer: ";
        cin >> customer;
        cout << "Driver: ";
        cin >> driver;
        cin.ignore();
        cout << "Pickup Address: ";
        getline(cin, pickup);
        cout << "Drop Address: ";
        getline(cin, drop);
        while (true)
        {
            cout << "Distance: ";
            if (cin >> distance)
                break;

            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "ETA: ";
        while (true)
        {
            cout << "ETA: ";
            if (cin >> eta)
                break;

            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        addOrder(new StandardDelivery(id, customer, driver, pickup, drop, distance, eta));
        cout << "Order created!\n";
    }

    void viewAllOrders()
    {
        cout << "\n=========== ALL ORDERS ===========\n";
        cout << left
            << setw(10) << "ID"
            << setw(15) << "Customer"
            << setw(15) << "Driver"
            << setw(12) << "Distance"
            << setw(12) << "Fee"
            << endl;
        cout << "--------------------------------------\n";
        ifstream fin(filename);
        if (!fin) { cout << "No orders found.\n"; return; }
        string line;
        while (getline(fin, line))
        {
            int id; string customer, driver, pickup, drop; double dist; int eta;
            if (parseLine(line, id, customer, driver, pickup, drop, dist, eta))
            {
                if (customer == currentUser || isAdmin)
                {
                    StandardDelivery tmp(id, customer, driver, pickup, drop, dist, eta);
                    tmp.printRow();
                }
            }
        }
    }

    void viewOrderById()
    {
        int id; cout << "Enter Order ID: "; cin >> id;
        ifstream fin(filename);
        if (!fin) { cout << "Order not found!\n"; return; }
        string line;
        while (getline(fin, line))
        {
            int fid; string customer, driver, pickup, drop; double dist; int eta;
            if (parseLine(line, fid, customer, driver, pickup, drop, dist, eta))
            {
                if (fid == id) { StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); tmp.printDetails(); return; }
            }
        }
        cout << "Order not found!\n";
    }

    void updateOrder()
    {
        int id; cout << "Enter Order ID: "; cin >> id;
        ifstream fin(filename);
        if (!fin) { cout << "Order not found!\n"; return; }
        vector<string> allOrders;
        string line; bool found = false;
        while (getline(fin, line))
        {
            int fid; string customer, driver, pickup, drop; double dist; int eta;
            if (!parseLine(line, fid, customer, driver, pickup, drop, dist, eta)) continue;
            if (fid == id)
            {
                found = true;
                int choice;
                cout << "\n--- UPDATE MENU ---\n";
                cout << "0. Cancel\n1. Driver\n2. Pickup\n3. Drop\n4. Distance\n5. ETA\n";
                cout << "Choice: "; cin >> choice; cin.ignore();
                if (choice == 0) { allOrders.push_back(line); }
                else if (choice == 1) { string nd; cout << "New Driver: "; getline(cin, nd); driver = nd; StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); allOrders.push_back(serializeOrder(&tmp)); }
                else if (choice == 2) { string np; cout << "New Pickup: "; getline(cin, np); pickup = np; StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); allOrders.push_back(serializeOrder(&tmp)); }
                else if (choice == 3) { string ndrop; cout << "New Drop: "; getline(cin, ndrop); drop = ndrop; StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); allOrders.push_back(serializeOrder(&tmp)); }
                else if (choice == 4) { double ndist; cout << "New Distance: "; cin >> ndist; dist = ndist; StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); allOrders.push_back(serializeOrder(&tmp)); }
                else if (choice == 5) { int neta; cout << "New ETA: "; cin >> neta; eta = neta; StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); allOrders.push_back(serializeOrder(&tmp)); }
                else { cout << "Invalid option!\n"; allOrders.push_back(line); }
            }
            else { allOrders.push_back(line); }
        }
        fin.close();
        if (!found) { cout << "Order not found!\n"; return; }
        ofstream fout(filename);
        for (const string& order : allOrders) { fout << order << "\n"; }
        fout.close();
        cout << "Updated successfully!\n";
    }

    void deleteOrder()
    {
        int id; cout << "Enter Order ID: "; cin >> id;
        ifstream fin(filename);
        if (!fin) { cout << "Not found!\n"; return; }
        vector<string> allOrders;
        string line; bool found = false;
        while (getline(fin, line))
        {
            int fid; string a, b, c; string d, e; double dist; int eta;
            if (!parseLine(line, fid, a, b, d, e, dist, eta)) continue;
            if (fid == id) { found = true; continue; }
            allOrders.push_back(line);
        }
        fin.close();
        if (!found) { cout << "Not found!\n"; return; }
        ofstream fout(filename);
        for (const string& order : allOrders) { fout << order << "\n"; }
        fout.close();
        cout << "Deleted!\n";
    }

    ~OrderSystem()
    {
        remove(filename.c_str());
    }
};

string login(bool& isAdmin)
{
    string user, pass;

    cout << "\n=========== LOGIN ===========\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    if (user == "admin" && pass == "admin123")
    {
        isAdmin = true;
        return user;
    }

    if (user == "user" && pass == "1234")
    {
        isAdmin = false;
        return user;
    }

    throw runtime_error("Invalid login!");
}

int main()
{
    OrderSystem system;


    while (true)
    {
        try
        {
            bool isAdmin;
            string user = login(isAdmin);
            system.currentUser = user;
            system.isAdmin = isAdmin;
            int choice;
            while (true)
            {
                if (isAdmin)
                {
                    cout << "\n========== ADMIN PANEL ==========\n";
                    cout << "1. Create Order\n";
                    cout << "2. View Orders\n";
                    cout << "3. View Order By ID\n";
                    cout << "4. Update Order\n";
                    cout << "5. Delete Order\n";
                    cout << "6. Logout\n";
                    cout << "Choice: ";
                    cin >> choice;
                    if (choice == 1) system.createOrder();
                    else if (choice == 2) system.viewAllOrders();
                    else if (choice == 3) system.viewOrderById();
                    else if (choice == 4) system.updateOrder();
                    else if (choice == 5) system.deleteOrder();
                    else if (choice == 6) break;
                }
                else
                {
                    cout << "\n========== CUSTOMER ==========\n";
                    cout << "1. View All Orders\n";
                    cout << "2. View Order By ID\n";
                    cout << "3. Logout\n";
                    cout << "Choice: ";
                    cin >> choice;
                    if (choice == 1) system.viewAllOrders();
                    else if (choice == 2) system.viewOrderById();
                    else if (choice == 3) break;
                    else cout << "Invalid choice!\n";
                }
            }
        }
        catch (exception& e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
}
