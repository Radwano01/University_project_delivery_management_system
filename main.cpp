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

    string getUsername()
    {
        return username;
    }

    string getPassword()
    {
        return password;
    }

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
        : User(u, p, i)
    {
    }

    void display() override
    {
        cout << "Customer: " << username << endl;
    }
};



/*====================================================
                     DRIVER
====================================================*/
class Driver : public User
{
public:

    Driver(string u, string p, int i)
        : User(u, p, i)
    {
    }

    void display() override
    {
        cout << "Driver: " << username << endl;
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


    void setStatus(OrderStatus s)
    {
        status = s;
    }


    string getStatusText()
    {
        switch (status)
        {
        case PENDING:
            return "Pending";

        case PICKED_UP:
            return "Picked Up";

        case IN_TRANSIT:
            return "In Transit";

        case DELIVERED:
            return "Delivered";
        }

        return "Unknown";
    }


    int getOrderId()
    {
        return orderId;
    }


    string getCustomer()
    {
        return customerName;
    }



    /* summary row */
    virtual void showOrderSummary()
    {
        cout << left
            << setw(10) << orderId
            << setw(15) << customerName
            << setw(18) << getStatusText()
            << setw(10)
            << fixed
            << setprecision(2)
            << calculateFee()
            << endl;
    }



    /* detailed order tracking */
    virtual void showTrackingDetails()
    {
        cout << "\n=====================================\n";
        cout << "          ORDER TRACKING\n";
        cout << "=====================================\n";

        cout << "Order ID        : "
            << orderId << endl;

        cout << "Customer        : "
            << customerName << endl;

        cout << "Driver          : "
            << driverName << endl;

        cout << "Pickup Address  : "
            << pickupAddress << endl;

        cout << "Drop Address    : "
            << dropAddress << endl;

        cout << "Distance        : "
            << distance << " km" << endl;

        cout << "Delivery Fee    : "
            << fixed
            << setprecision(2)
            << calculateFee()
            << endl;

        cout << "ETA             : "
            << etaMinutes
            << " minutes" << endl;

        cout << "Status          : "
            << getStatusText()
            << endl;



        cout << "\nTracking Timeline\n";
        cout << "--------------------------\n";

        cout << "Order Created\n";
        cout << "Driver Assigned\n";

        if (status >= PICKED_UP)
            cout << "Package Picked Up\n";

        if (status >= IN_TRANSIT)
            cout << "Package In Transit\n";

        if (status == DELIVERED)
            cout << "Delivered Successfully\n";
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

        : Delivery(id,
            customer,
            driver,
            pickup,
            drop,
            dist,
            eta)
    {
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

    void updateStatus(Delivery* order,
        OrderStatus newStatus)
    {
        if (order == nullptr)
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
        if (order == nullptr)
            throw DeliveryException("Invalid order");

        orders.push_back(order);
    }



    /*---------------- save users ----------------*/
    void saveUser(Customer user)
    {
        ofstream file("users.txt", ios::app);

        file
            << user.getUsername()
            << " "
            << user.getPassword()
            << endl;

        file.close();
    }



    /*---------------- save orders ----------------*/
    void saveOrders()
    {
        ofstream file("orders.txt");

        for (auto order : orders)
        {
            file
                << order->getOrderId()
                << " "
                << order->getCustomer()
                << endl;
        }

        file.close();
    }




    /*----------- option 1 view my orders --------*/
    void viewMyOrders()
    {
        string user, pass;

        cout << "\nUsername: ";
        cin >> user;

        cout << "Password: ";
        cin >> pass;


        ifstream file("users.txt");

        string u, p;

        bool valid = false;

        while (file >> u >> p)
        {
            if (user == u &&
                pass == p)
            {
                valid = true;
                break;
            }
        }

        file.close();

        if (!valid)
            throw DeliveryException("Login failed");



        cout << "\n====================================\n";
        cout << "           MY ORDERS\n";
        cout << "====================================\n";

        cout << left
            << setw(10) << "ID"
            << setw(15) << "Customer"
            << setw(18) << "Status"
            << setw(10) << "Fee"
            << endl;

        cout << "------------------------------------\n";

        bool found = false;

        for (auto order : orders)
        {
            if (order->getCustomer() == user)
            {
                order->showOrderSummary();
                found = true;
            }
        }

        if (!found)
            cout << "No orders found.\n";
    }




    /*------ option 2 track specific order ------*/
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


        /* customers */
        Customer c1("ali", "1234", 1);
        Customer c2("sara", "abcd", 2);

        system.saveUser(c1);
        system.saveUser(c2);



        /* orders */
        Delivery* o1 =
            new StandardDelivery(
                101,
                "ali",
                "Omar",
                "Adana Center",
                "Cukurova",
                20,
                25
            );


        Delivery* o2 =
            new StandardDelivery(
                102,
                "ali",
                "Omar",
                "Seyhan",
                "Yuregir",
                14,
                18
            );


        Delivery* o3 =
            new StandardDelivery(
                103,
                "sara",
                "Ahmad",
                "Tarsus",
                "Mersin",
                35,
                40
            );



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
            cout << "\n====================================\n";
            cout << " DELIVERY MANAGEMENT SYSTEM\n";
            cout << "====================================\n";

            cout << "1. View My Orders\n";
            cout << "2. Track Specific Order\n";
            cout << "3. Exit\n";

            cout << "\nSelect Option: ";
            cin >> option;


            switch (option)
            {

            case 1:
                system.viewMyOrders();
                break;


            case 2:
            {
                int id;

                cout << "\nEnter Order ID: ";
                cin >> id;

                system.trackSpecificOrder(id);

                break;
            }


            case 3:
                cout << "\nExiting system...\n";
                break;


            default:
                cout << "Invalid option.\n";

            }

        } while (option != 3);


    }
    catch (exception& e)
    {
        cout << "\nError: "
            << e.what()
            << endl;
    }

    return 0;
}