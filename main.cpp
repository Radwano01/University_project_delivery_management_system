#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <string>

using namespace std;

/*====================================================
    BEGINNER EXPLANATION (IMPORTANT BEFORE CODE)

    *  (pointer)   → stores ADDRESS of an object (where it lives in memory)
    &  (reference) → uses ORIGINAL variable (no copy is made)
    new            → creates object in heap memory
    delete         → removes object from memory

    SIMPLE WAY TO UNDERSTAND:
    - *  = "where is the object?"
    - &  = "use the same variable"
    - new = "create object"
    - delete = "remove object"
====================================================*/

/*================ EXCEPTION ================*/
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

/*====================================================
    DELIVERY CLASS (BASE CLASS)

    SIMPLE EXPLANATION:
    - This is a BLUEPRINT for all deliveries
    - We CANNOT create object from this class directly
    - Because it has a pure virtual function (calculateFee)

    WHY IT EXISTS:
    - To share common data between all delivery types
====================================================*/

/*================ DELIVERY ================*/
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
    /* Constructor: sets initial values for order */
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

    /* PURE VIRTUAL FUNCTION
   - must be implemented in child classes
   - defines how price is calculated */
    virtual double calculateFee() = 0;

    /* GETTERS: return values */
    int getOrderId() { return orderId; }
    string getCustomer() { return customerName; }

    /* SETTERS: modify values */
    void setDriver(string d) { driverName = d; }
    void setPickup(string p) { pickupAddress = p; }
    void setDrop(string d) { dropAddress = d; }
    void setDistance(double d) { distance = d; }
    void setEta(int e) { etaMinutes = e; }

    /* printRow:
   shows short info in table format */
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

    /* printDetails:
     shows full order information */
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

    /* Destructor */
    virtual ~Delivery() {}
};

/*====================================================
    STANDARD DELIVERY CLASS (CHILD CLASS)

    SIMPLE:
    - This is a normal delivery type
    - It defines its own pricing rule
====================================================*/
/*================ STANDARD DELIVERY ================*/
class StandardDelivery : public Delivery
{
public:
    StandardDelivery(int id, string customer, string driver,
        string pickup, string drop,
        double dist, int eta)
        : Delivery(id, customer, driver, pickup, drop, dist, eta) {
    }

    /* Fee calculation rule:
   price = distance × 2.5 */
    double calculateFee() override
    {
        return distance * 2.5;
    }
};


/*====================================================
    ORDER SYSTEM CLASS

    SIMPLE:
    - Controls all orders
    - Stores orders in vector using POINTERS

    WHY POINTERS (*)?
    - Allows storing different object types
    - Needed for polymorphism
    - Objects live in heap memory (new/delete)
====================================================*/
/*================ SYSTEM ================*/
class OrderSystem
{
private:
    vector<Delivery*> orders;// stores addresses of objects

public:
    /* Add new order */
    void addOrder(Delivery* o)
    {
        orders.push_back(o);
    }

    /* CREATE ORDER */
    /*================ CREATE =================*/
    void createOrder()
    {
        int id, eta;
        string customer, driver;
        string pickup, drop;
        double distance;

        cout << "\n--- CREATE ORDER ---\n";

        cout << "Order ID: ";
        cin >> id;

        cout << "Customer: ";
        cin >> customer;

        cout << "Driver: ";
        cin >> driver;

        /* Clears leftover '\n' from input buffer.
           Needed because cin >> leaves Enter key behind,
           which can break getline().

           Example:
           cin >> id;
           cin.ignore();  // removes '\n'
           getline(cin, name);  // now works correctly
        */

        cin.ignore(); // fixes input buffer issue

        /*====================================================
            getline(cin, variable)

            PURPOSE:
            - Reads a FULL LINE of input (including spaces)

            WHY WE USE IT:
            - cin >> only reads until first space
              Example: "Ali Ahmed" → only "Ali"

            - getline reads everything:
              Example: "Ali Ahmed" → "Ali Ahmed"

            IMPORTANT NOTE:
            - If you use cin before getline,
              you MUST use cin.ignore() first

            WHY?
            - Because cin leaves '\n' (enter key) in the buffer
            - getline would read that empty line instead of real input
        ====================================================*/

        cout << "Pickup Address: ";
        getline(cin, pickup);

        cout << "Drop Address: ";
        getline(cin, drop);

        cout << "Distance: ";
        cin >> distance;

        cout << "ETA: ";
        cin >> eta;

        /* new = create object in memory */
        orders.push_back(new StandardDelivery(id, customer, driver, pickup, drop, distance, eta));

        cout << "Order created!\n";
    }

    /* VIEW ALL ORDERS */
    /*================ TABLE =================*/
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

        for (auto o : orders)
            o->printRow();
    }

    /* VIEW BY ID */
    /*================ DETAIL BY ID =================*/
    void viewOrderById()
    {
        int id;
        cout << "Enter Order ID: ";
        cin >> id;

        for (auto o : orders)
        {
            if (o->getOrderId() == id)
            {
                o->printDetails();
                return;
            }
        }

        cout << "Order not found!\n";
    }

    /* UPDATE ORDER */
    /*================ UPDATE =================*/
    void updateOrder()
    {
        int id;
        cout << "Enter Order ID: ";
        cin >> id;

        for (auto o : orders)
        {
            if (o->getOrderId() == id)
            {
                int choice;

                cout << "\n--- UPDATE MENU ---\n";
                cout << "0. Cancel\n";
                cout << "1. Driver\n";
                cout << "2. Pickup\n";
                cout << "3. Drop\n";
                cout << "4. Distance\n";
                cout << "5. ETA\n";

                cout << "Choice: ";
                cin >> choice;

                cin.ignore(); // FIX INPUT

                if (choice == 0) return;

                switch (choice)
                {
                case 1:
                {
                    string driver;
                    cout << "New Driver: ";
                    getline(cin, driver);
                    o->setDriver(driver);
                    break;
                }
                case 2:
                {
                    string pickup;
                    cout << "New Pickup: ";
                    getline(cin, pickup);
                    o->setPickup(pickup);
                    break;
                }
                case 3:
                {
                    string drop;
                    cout << "New Drop: ";
                    getline(cin, drop);
                    o->setDrop(drop);
                    break;
                }
                case 4:
                {
                    double dist;
                    cout << "New Distance: ";
                    cin >> dist;
                    o->setDistance(dist);
                    break;
                }
                case 5:
                {
                    int eta;
                    cout << "New ETA: ";
                    cin >> eta;
                    o->setEta(eta);
                    break;
                }
                default:
                    cout << "Invalid option!\n";
                    return;
                }

                cout << "Updated successfully!\n";
                return;
            }
        }

        cout << "Order not found!\n";
    }

    /* DELETE ORDER */
    /*================ DELETE =================*/
    void deleteOrder()
    {
        int id;
        cout << "Enter Order ID: ";
        cin >> id;

        for (auto it = orders.begin(); it != orders.end(); ++it)
        {
            if ((*it)->getOrderId() == id)
            {
                delete *it;
                orders.erase(it);
                cout << "Deleted!\n";
                return;
            }
        }

        cout << "Not found!\n";
    }

    /* Destructor cleans memory */
    ~OrderSystem()
    {
        for (auto o : orders)
            delete o;
    }
};

/* LOGIN FUNCTION */
/*================ LOGIN ================*/
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
    }else
    {
        isAdmin = false;
        return user;
    }

    throw runtime_error("Invalid login!");
}

/* MAIN PROGRAM */
/*================ MAIN ================*/
int main()
{
    OrderSystem system;

    system.addOrder(new StandardDelivery(101, "ali", "Omar", "Adana", "Cukurova", 20, 25));
    system.addOrder(new StandardDelivery(102, "ali", "Omar", "Seyhan", "Yuregir", 14, 18));
    system.addOrder(new StandardDelivery(103, "sara", "Ahmad", "Tarsus", "Mersin", 35, 40));

    while (true)
    {
        try
        {
            bool isAdmin;
            string user = login(isAdmin);

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
