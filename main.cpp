#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <stdexcept>

using namespace std;


/*====================================================
                CUSTOM EXCEPTION
====================================================*/
class DeliveryException : public exception
{
private:
    string message;

public:
    DeliveryException(string msg)
    {
        message = msg;
    }

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};



/*====================================================
                   USER (ABSTRACT)
====================================================*/
class User
{
protected:
    string username;
    string password;
    int id;

public:
    User(string u, string p, int i)
    {
        username = u;
        password = p;
        id = i;
    }

    /* GETTERS */
    string getUsername() { return username; }
    string getPassword() { return password; }
    int getId() { return id; }

    /* SETTERS */
    void setUsername(string u) { username = u; }
    void setPassword(string p) { password = p; }
    void setId(int i) { id = i; }

    virtual void display() = 0;
    virtual ~User() {}
};



/*====================================================
                    CUSTOMER
====================================================*/
class Customer : public User
{
public:
    Customer(string u, string p, int i)
        : User(u, p, i) {
    }

    void display() override
    {
        cout << "Customer: " << username << endl;
    }
};



/*====================================================
                DELIVERY ABSTRACT CLASS
====================================================*/
enum OrderStatus
{
    PENDING,
    PICKED_UP,
    IN_TRANSIT,
    DELIVERED
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
    OrderStatus status;

public:

    Delivery(int id,
        string customer,
        string driver,
        string pickup,
        string drop,
        double dist,
        int eta)
    {
        orderId = id;
        customerName = customer;
        driverName = driver;
        pickupAddress = pickup;
        dropAddress = drop;
        distance = dist;
        etaMinutes = eta;
        status = PENDING;
    }

    virtual double calculateFee() = 0;

    /*====================================================
                        SETTERS
    ====================================================*/
    void setOrderId(int id) { orderId = id; }
    void setCustomerName(string c) { customerName = c; }
    void setDriverName(string d) { driverName = d; }
    void setPickupAddress(string p) { pickupAddress = p; }
    void setDropAddress(string d) { dropAddress = d; }
    void setDistance(double d) { distance = d; }
    void setEtaMinutes(int e) { etaMinutes = e; }
    void setStatus(OrderStatus s) { status = s; }

    /*====================================================
                        GETTERS
    ====================================================*/
    int getOrderId() { return orderId; }
    string getCustomer() { return customerName; }
    string getDriverName() { return driverName; }
    string getPickupAddress() { return pickupAddress; }
    string getDropAddress() { return dropAddress; }
    double getDistance() { return distance; }
    int getEtaMinutes() { return etaMinutes; }
    OrderStatus getStatus() { return status; }

    string getStatusText()
    {
        switch (status)
        {
        case PENDING: return "Pending";
        case PICKED_UP: return "Picked Up";
        case IN_TRANSIT: return "In Transit";
        case DELIVERED: return "Delivered";
        }
        return "Unknown";
    }

    virtual void showOrderSummary()
    {
        cout << left
            << setw(10) << orderId
            << setw(15) << customerName
            << setw(18) << getStatusText()
            << setw(10)
            << fixed << setprecision(2)
            << calculateFee()
            << endl;
    }

    virtual void showTrackingDetails()
    {
        cout << "\n=====================================\n";
        cout << "          ORDER TRACKING\n";
        cout << "=====================================\n";

        cout << "Order ID        : " << orderId << endl;
        cout << "Customer        : " << customerName << endl;
        cout << "Driver          : " << driverName << endl;
        cout << "Pickup Address  : " << pickupAddress << endl;
        cout << "Drop Address    : " << dropAddress << endl;
        cout << "Distance        : " << distance << " km" << endl;
        cout << "Fee             : " << calculateFee() << endl;
        cout << "ETA             : " << etaMinutes << " minutes" << endl;
        cout << "Status          : " << getStatusText() << endl;
    }

    virtual ~Delivery() {}
};



/*====================================================
                 STANDARD DELIVERY
====================================================*/
class StandardDelivery : public Delivery
{
public:
    StandardDelivery(int id,
        string customer,
        string driver,
        string pickup,
        string drop,
        double dist,
        int eta)
        : Delivery(id, customer, driver, pickup, drop, dist, eta) {
    }

    double calculateFee() override
    {
        return distance * 2.50;
    }
};



/*====================================================
                    ORDER TRACKER
====================================================*/
class OrderTracker
{
public:
    void updateStatus(Delivery* order, OrderStatus newStatus)
    {
        if (!order)
            throw DeliveryException("Order missing");

        order->setStatus(newStatus);
    }
};



/*====================================================
                   ORDER SYSTEM
====================================================*/
class OrderSystem
{
private:
    vector<Delivery*> orders;

public:

    void addOrder(Delivery* order)
    {
        if (!order)
            throw DeliveryException("Invalid order");

        orders.push_back(order);
    }

    void saveUser(Customer user)
    {
        ofstream file("users.txt", ios::app);

        file << user.getUsername()
            << " "
            << user.getPassword()
            << endl;

        file.close();
    }

    void saveOrders()
    {
        ofstream file("orders.txt");

        for (auto order : orders)
        {
            file << order->getOrderId()
                << " "
                << order->getCustomer()
                << endl;
        }

        file.close();
    }

    void viewMyOrders()
    {
        string user, pass;

        cout << "\nEnter username: ";
        cin >> user;

        cout << "Enter password: ";
        cin >> pass;

        ifstream file("users.txt");

        string u, p;
        bool valid = false;

        while (file >> u >> p)
        {
            if (user == u && pass == p)
            {
                valid = true;
                break;
            }
        }

        file.close();

        if (!valid)
            throw DeliveryException("Login failed");

        cout << "\nMY ORDERS\n";
        cout << "------------------------------------\n";

        for (auto order : orders)
        {
            if (order->getCustomer() == user)
                order->showOrderSummary();
        }
    }

    void trackSpecificOrder(int id)
    {
        for (auto order : orders)
        {
            if (order->getOrderId() == id)
            {
                order->showTrackingDetails();
                return;
            }
        }

        throw DeliveryException("Order ID not found");
    }

    ~OrderSystem()
    {
        for (auto order : orders)
            delete order;
    }
};



/*====================================================
                       MAIN
====================================================*/
int main()
{
    try
    {
        OrderSystem system;
        OrderTracker tracker;

        ofstream file("users.txt");
        file << "ali 1234\n";
        file << "sara abcd\n";
        file.close();

        Customer c1("ali", "1234", 1);
        Customer c2("sara", "abcd", 2);

        system.saveUser(c1);
        system.saveUser(c2);

        Delivery* o1 = new StandardDelivery(101, "ali", "Omar", "Adana", "Cukurova", 20, 25);
        Delivery* o2 = new StandardDelivery(102, "ali", "Omar", "Seyhan", "Yuregir", 14, 18);
        Delivery* o3 = new StandardDelivery(103, "sara", "Ahmad", "Tarsus", "Mersin", 35, 40);

        system.addOrder(o1);
        system.addOrder(o2);
        system.addOrder(o3);

        tracker.updateStatus(o1, DELIVERED);
        tracker.updateStatus(o2, IN_TRANSIT);
        tracker.updateStatus(o3, PICKED_UP);

        system.saveOrders();

        int option;

        do
        {
            cout << "\n1. View My Orders\n2. Track Order\n3. Exit\n";
            cout << "Enter option: ";
            cin >> option;

            if (option == 1)
                system.viewMyOrders();
            else if (option == 2)
            {
                int id;
                cout << "Enter Order ID: ";
                cin >> id;
                system.trackSpecificOrder(id);
            }

        } while (option != 3);

    }
    catch (exception& e)
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
