#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
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
    /* orderId: unique number for each order (like tracking number) */
    int orderId;
    /* customerName: name of person who ordered delivery */
    string customerName;
    /* driverName: name of person who delivers the package */
    string driverName;
    /* pickupAddress: starting address where driver picks up package */
    string pickupAddress;
    /* dropAddress: final address where package is delivered */
    string dropAddress;
    /* distance: total kilometers the driver needs to travel */
    double distance;
    /* etaMinutes: estimated time in minutes for delivery */
    int etaMinutes;

public:
    /* Constructor: initializes all order information when object is created */
    Delivery(int id, string customer, string driver,
        string pickup, string drop,
        double dist, int eta)
    {
        /* Store the order ID passed as parameter */
        orderId = id;
        /* Store the customer name */
        customerName = customer;
        /* Store the driver name */
        driverName = driver;
        /* Store the pickup address */
        pickupAddress = pickup;
        /* Store the drop address */
        dropAddress = drop;
        /* Store the distance in kilometers */
        distance = dist;
        /* Store the eta (estimated time of arrival) in minutes */
        etaMinutes = eta;
    }

    /* PURE VIRTUAL FUNCTION: calculateFee()
       - This function MUST be written differently in each child class
       - Each delivery type (Standard, Express, etc) calculates cost differently
       - The '= 0' means: no implementation here, child classes must provide it
       - The 'virtual' keyword allows child classes to override it */
    virtual double calculateFee() = 0;

    /* ========== GETTER FUNCTIONS ==========
       These functions READ data from the order but CANNOT change it */
    
    /* getOrderId: returns the order's unique ID number */
    int getOrderId() { return orderId; }
    
    /* getCustomer: returns the name of person who ordered */
    string getCustomer() { return customerName; }
    
    /* getDriver: returns the name of delivery driver */
    string getDriver() { return driverName; }
    
    /* getPickup: returns the address where package is picked up from */
    string getPickup() { return pickupAddress; }
    
    /* getDrop: returns the address where package is delivered to */
    string getDrop() { return dropAddress; }
    
    /* getDistance: returns the distance to travel in kilometers */
    double getDistance() { return distance; }
    
    /* getEta: returns estimated delivery time in minutes */
    int getEta() { return etaMinutes; }

    /* ========== SETTER FUNCTIONS ==========
       These functions MODIFY (change) data in the order */
    
    /* setDriver: changes which driver is assigned to this order */
    void setDriver(string d) { driverName = d; }
    
    /* setPickup: changes the pickup address */
    void setPickup(string p) { pickupAddress = p; }
    
    /* setDrop: changes the drop address */
    void setDrop(string d) { dropAddress = d; }
    
    /* setDistance: changes the distance value */
    void setDistance(double d) { distance = d; }
    
    /* setEta: changes the estimated arrival time */
    void setEta(int e) { etaMinutes = e; }

    /* printRow: displays order information in a single table row
       Used for showing all orders in a compact list format */
    virtual void printRow()
    {
        /* 'cout' sends output to screen */
        /* 'left' aligns text to the left */
        cout << left
            /* setw(10) reserves 10 characters for Order ID */
            << setw(10) << orderId
            /* setw(15) reserves 15 characters for Customer name */
            << setw(15) << customerName
            /* setw(15) reserves 15 characters for Driver name */
            << setw(15) << driverName
            /* setw(12) reserves 12 characters for Distance
               'fixed' shows decimals, 'setprecision(2)' shows exactly 2 decimal places */
            << setw(12) << fixed << setprecision(2) << distance
            /* setw(12) reserves 12 characters for calculated Fee amount */
            << setw(12) << fixed << setprecision(2) << calculateFee()
            /* 'endl' ends the line and moves cursor to next line */
            << endl;
    }

    /* printDetails: shows all information about an order in detailed format
       Used when user wants to see complete order information */
    virtual void printDetails()
    {
        /* Print header separator line */
        cout << "\n=========== ORDER DETAILS ===========\n";

        /* Print Order ID: reserve 20 chars for label, then show the actual ID */
        cout << left << setw(20) << "Order ID:" << orderId << endl;
        /* Print Customer name with 20 char label spacing */
        cout << left << setw(20) << "Customer:" << customerName << endl;
        /* Print Driver name with 20 char label spacing */
        cout << left << setw(20) << "Driver:" << driverName << endl;
        /* Print Pickup address with 20 char label spacing */
        cout << left << setw(20) << "Pickup:" << pickupAddress << endl;
        /* Print Drop address with 20 char label spacing */
        cout << left << setw(20) << "Drop:" << dropAddress << endl;

        /* Print Distance: show with exactly 2 decimal places and " km" text */
        cout << left << setw(20) << "Distance:"
            << fixed << setprecision(2) << distance << " km" << endl;

        /* Print Fee: calculate and show with exactly 2 decimal places */
        cout << left << setw(20) << "Fee:"
            << fixed << setprecision(2) << calculateFee() << endl;

        /* Print ETA (Estimated Time of Arrival) with " min" text */
        cout << left << setw(20) << "ETA:" << etaMinutes << " min" << endl;

        /* Print footer separator line */
        cout << "====================================\n";
    }

    /* Destructor: cleanup function called when Delivery object is deleted
       The 'virtual' keyword allows child classes to override it
       The '{}' means there's nothing to clean up (empty body) */
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
    /* Constructor: creates StandardDelivery object
       The ': Delivery(...)' passes all parameters to parent class constructor */
    StandardDelivery(int id, string customer, string driver,
        string pickup, string drop,
        double dist, int eta)
        : Delivery(id, customer, driver, pickup, drop, dist, eta) {
        /* Empty body - all initialization done by parent class */
    }

    /* calculateFee: OVERRIDE (replace) parent class version
       Implements Standard delivery pricing rule:
       Price = distance multiplied by 2.5 per kilometer
       Example: 10 km × 2.5 = 25 currency units */
    double calculateFee() override
    {
        /* Multiply distance by 2.5 and return the result */
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
    /* filename: stores the name of file where orders are saved
       'const' means this value cannot be changed after initialization */
    const string filename = "orders.txt";

    /* serializeOrder: converts an order object into a text format for saving to file
       Takes a pointer to Delivery object and returns formatted text string
       Pointer (*) is needed because different delivery types can be passed */
    static string serializeOrder(Delivery* o)
    {
        /* ostringstream: tool for building strings by adding pieces together */
        ostringstream ss;
        /* Add order ID followed by | separator */
        ss << o->getOrderId() << "|";
        /* Add customer name followed by | separator */
        ss << o->getCustomer() << "|";
        /* Add driver name followed by | separator */
        ss << o->getDriver() << "|";
        /* Add pickup address followed by | separator */
        ss << o->getPickup() << "|";
        /* Add drop address followed by | separator */
        ss << o->getDrop() << "|";
        /* Add distance with 2 decimal places followed by | separator */
        ss << fixed << setprecision(2) << o->getDistance() << "|";
        /* Add ETA (no separator at end) */
        ss << o->getEta();
        /* Return the complete formatted string */
        return ss.str();
    }

    /* parseLine: does the opposite of serializeOrder
       Takes a text line and splits it into separate order information
       Returns true if parsing succeeded, false if format was wrong */
    static bool parseLine(const string& line, int& id, string& customer, string& driver,
        string& pickup, string& drop, double& distance, int& eta)
    {
        /* Create an empty vector to store text parts after splitting */
        vector<string> parts;
        /* Variable to hold each part temporarily */
        string part;
        /* Create a string input stream from the line (reads text) */
        istringstream ss(line);
        /* Loop: read each part between | separators and add to parts vector */
        while (getline(ss, part, '|')) parts.push_back(part);

        /* Check if we got exactly 7 parts (correct format)
           If not, data is corrupted or wrong format */
        if (parts.size() != 7) return false;

        /* try-catch block: attempt to convert text to numbers
           If conversion fails, catch the error and return false */
        try {
            /* Convert 1st part from text to integer ID */
            id = stoi(parts[0]);
            /* 2nd part stays as text (customer name) */
            customer = parts[1];
            /* 3rd part stays as text (driver name) */
            driver = parts[2];
            /* 4th part stays as text (pickup address) */
            pickup = parts[3];
            /* 5th part stays as text (drop address) */
            drop = parts[4];
            /* Convert 6th part from text to decimal number (distance) */
            distance = stod(parts[5]);
            /* Convert 7th part from text to integer (eta) */
            eta = stoi(parts[6]);
        }
        /* If any conversion fails, catch error and return false */
        catch (...) { return false; }

        /* All data parsed successfully */
        return true;
    }

public:
    /* Constructor: runs when OrderSystem object is created
       It opens the orders.txt file to ensure it exists */
    OrderSystem() { 
        /* Create file stream for writing in append mode
           ios::app means add new data to end of file without deleting old data */
        ofstream f(filename, ios::app); 
    }

    /* addOrder: saves a new order to the file
       Takes a pointer to a Delivery object */
    void addOrder(Delivery* o)
    {
        /* Open file in append mode (add to end of file) */
        ofstream fout(filename, ios::app);
        /* Check if file opened successfully */
        if (!fout) { 
            /* If file failed to open, show error message */
            cout << "Error: cannot open orders file for writing.\n"; 
            /* Delete the order object from memory to prevent memory leak */
            delete o; 
            /* Exit function early */
            return; 
        }
        /* Convert order to text format and write it to file */
        fout << serializeOrder(o) << "\n";
        /* Close the file (save changes and release file lock) */
        fout.close();
        /* Delete the order object - file has copy, don't need object in memory anymore */
        delete o;
    }

    /* isDuplicateId: checks if an order ID already exists in the file
       Returns true if ID found, false if not found */
    bool isDuplicateId(int id)
    {
        /* Open file for reading */
        ifstream fin(filename);
        /* If file cannot be opened, no duplicates exist (empty file) */
        if (!fin) return false;
        /* Variable to hold each line from file */
        string line;
        /* Loop through every line in the file */
        while (getline(fin, line))
        {
            /* Declare variables to hold parsed order data */
            int fid; string a,b,c; string d,e; double dist; int eta;
            /* Try to parse (split) the line into order components
               If parsing fails, skip this line and continue to next */
            if (parseLine(line, fid, a, b, d, e, dist, eta))
            {
                /* Check if the ID we found (fid) matches the ID we're looking for */
                if (fid == id) return true;
            }
        }
        /* Went through entire file and didn't find matching ID */
        return false;
    }

    /* createOrder: allows user to enter new order information and saves it
       Prompts user for all required information then creates StandardDelivery object */
    void createOrder()
    {
        /* Declare variables to store order information entered by user */
        int id, eta;
        string customer, driver;
        string pickup, drop;
        double distance;

        /* Print menu header to guide user */
        cout << "\n--- CREATE ORDER ---\n";

        /* Ask user for order ID */
        cout << "Order ID: ";
        /* Read integer from keyboard into id variable */
        cin >> id;

        /* Check if this order ID already exists in file */
        if (isDuplicateId(id)) { 
            /* If duplicate found, show error and exit function */
            cout << "Error: Order ID already exists! Try another ID.\n"; 
            return; 
        }

        /* Ask user for customer name */
        cout << "Customer: ";
        /* Read word (up to space) from keyboard */
        cin >> customer;

        /* Ask user for driver name */
        cout << "Driver: ";
        /* Read word (up to space) from keyboard */
        cin >> driver;

        /* cin.ignore(): clears leftover newline in input buffer
           This is needed before using getline() */
        cin.ignore();

        /* Ask user for pickup address (can contain spaces) */
        cout << "Pickup Address: ";
        /* getline() reads entire line including spaces */
        getline(cin, pickup);

        /* Ask user for drop address (can contain spaces) */
        cout << "Drop Address: ";
        /* getline() reads entire line including spaces */
        getline(cin, drop);

        /* Ask user for distance */
        cout << "Distance: ";
        /* Read decimal number from keyboard */
        cin >> distance;

        /* Ask user for ETA */
        cout << "ETA: ";
        /* Read integer from keyboard */
        cin >> eta;

        /* Create new StandardDelivery object in memory with all entered data
           'new' keyword allocates memory on heap
           Pass this object to addOrder which saves it and deletes the object */
        addOrder(new StandardDelivery(id, customer, driver, pickup, drop, distance, eta));
        /* Show success message */
        cout << "Order created!\n";
    }

    /* viewAllOrders: displays all orders from file in table format */
    void viewAllOrders()
    {
        /* Print table header */
        cout << "\n=========== ALL ORDERS ===========\n";

        /* Print column headers with proper spacing */
        cout << left
            /* 10 characters for ID column */
            << setw(10) << "ID"
            /* 15 characters for Customer column */
            << setw(15) << "Customer"
            /* 15 characters for Driver column */
            << setw(15) << "Driver"
            /* 12 characters for Distance column */
            << setw(12) << "Distance"
            /* 12 characters for Fee column */
            << setw(12) << "Fee"
            /* Move to next line */
            << endl;

        /* Print separator line */
        cout << "--------------------------------------\n";

        /* Open file for reading */
        ifstream fin(filename);
        /* If file doesn't exist or can't open, no orders exist */
        if (!fin) { cout << "No orders found.\n"; return; }
        
        /* Variable to hold each line from file */
        string line;
        /* Loop through every line in file */
        while (getline(fin, line))
        {
            /* Declare variables for parsed order data */
            int id; string customer, driver, pickup, drop; double dist; int eta;
            /* Parse the line into separate components
               If parsing fails, skip this line */
            if (parseLine(line, id, customer, driver, pickup, drop, dist, eta))
            {
                /* Create temporary StandardDelivery object from parsed data */
                StandardDelivery tmp(id, customer, driver, pickup, drop, dist, eta);
                /* Call printRow() to display this order as one table row */
                tmp.printRow();
            }
        }
    }

    /* viewOrderById: displays full details of one specific order by its ID */
    void viewOrderById()
    {
        /* Declare variable for order ID to search for */
        int id; 
        /* Prompt user to enter order ID */
        cout << "Enter Order ID: "; 
        /* Read the ID from keyboard */
        cin >> id;
        
        /* Open file for reading */
        ifstream fin(filename);
        /* If file can't open, order cannot exist */
        if (!fin) { cout << "Order not found!\n"; return; }
        
        /* Variable to hold each line from file */
        string line;
        /* Loop through every line in file */
        while (getline(fin, line))
        {
            /* Declare variables for parsed order data */
            int fid; string customer, driver, pickup, drop; double dist; int eta;
            /* Parse the line
               If parsing fails, skip to next line */
            if (parseLine(line, fid, customer, driver, pickup, drop, dist, eta))
            {
                /* Check if this is the order ID user is looking for */
                if (fid == id) { 
                    /* Create temporary StandardDelivery object from parsed data */
                    StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); 
                    /* Call printDetails() to show complete order information */
                    tmp.printDetails(); 
                    /* Exit function - order found and displayed */
                    return; 
                }
            }
        }
        /* Looped through entire file without finding the ID */
        cout << "Order not found!\n";
    }

    /* updateOrder: allows user to change specific fields of an existing order
       Reads file, finds matching order, updates it, then rewrites file */
    void updateOrder()
    {
        /* Declare variable for order ID to search for */
        int id; 
        /* Ask user which order to update */
        cout << "Enter Order ID: "; 
        /* Read the ID from keyboard */
        cin >> id;
        
        /* Open file for reading existing orders */
        ifstream fin(filename);
        /* If file can't open, order doesn't exist */
        if (!fin) { cout << "Order not found!\n"; return; }
        
        /* Open temporary file for writing updated orders */
        ofstream fout("orders_tmp.txt");
        
        /* Variable to hold each line from file */
        string line; 
        /* Boolean flag to track if we found the order */
        bool found = false;
        
        /* Loop through every line in the file */
        while (getline(fin, line))
        {
            /* Declare variables for parsed order data */
            int fid; string customer, driver, pickup, drop; double dist; int eta;
            /* Try to parse the line
               If parsing fails, skip to next line */
            if (!parseLine(line, fid, customer, driver, pickup, drop, dist, eta)) continue;
            
            /* Check if this is the order user wants to update */
            if (fid == id)
            {
                /* Mark that we found the order */
                found = true;
                /* Declare variable for user's menu choice */
                int choice;
                /* Show update menu options */
                cout << "\n--- UPDATE MENU ---\n";
                cout << "0. Cancel\n1. Driver\n2. Pickup\n3. Drop\n4. Distance\n5. ETA\n";
                cout << "Choice: "; 
                /* Read user's choice */
                cin >> choice; 
                /* Clear input buffer before using getline */
                cin.ignore();
                
                /* Check which field user wants to update */
                if (choice == 0) { 
                    /* User chose Cancel - keep original line unchanged */
                    fout << line << "\n"; 
                }
                else if (choice == 1) { 
                    /* User chose to update Driver */
                    string nd; 
                    cout << "New Driver: "; 
                    getline(cin, nd); 
                    /* Change driver to new value */
                    driver = nd; 
                    /* Create temporary object with updated driver */
                    StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); 
                    /* Write updated order to temporary file */
                    fout << serializeOrder(&tmp) << "\n"; 
                }
                else if (choice == 2) { 
                    /* User chose to update Pickup */
                    string np; 
                    cout << "New Pickup: "; 
                    getline(cin, np); 
                    /* Change pickup to new value */
                    pickup = np; 
                    /* Create temporary object with updated pickup */
                    StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); 
                    /* Write updated order to temporary file */
                    fout << serializeOrder(&tmp) << "\n"; 
                }
                else if (choice == 3) { 
                    /* User chose to update Drop address */
                    string ndrop; 
                    cout << "New Drop: "; 
                    getline(cin, ndrop); 
                    /* Change drop to new value */
                    drop = ndrop; 
                    /* Create temporary object with updated drop */
                    StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); 
                    /* Write updated order to temporary file */
                    fout << serializeOrder(&tmp) << "\n"; 
                }
                else if (choice == 4) { 
                    /* User chose to update Distance */
                    double ndist; 
                    cout << "New Distance: "; 
                    cin >> ndist; 
                    /* Change distance to new value */
                    dist = ndist; 
                    /* Create temporary object with updated distance */
                    StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); 
                    /* Write updated order to temporary file */
                    fout << serializeOrder(&tmp) << "\n"; 
                }
                else if (choice == 5) { 
                    /* User chose to update ETA */
                    int neta; 
                    cout << "New ETA: "; 
                    cin >> neta; 
                    /* Change eta to new value */
                    eta = neta; 
                    /* Create temporary object with updated eta */
                    StandardDelivery tmp(fid, customer, driver, pickup, drop, dist, eta); 
                    /* Write updated order to temporary file */
                    fout << serializeOrder(&tmp) << "\n"; 
                }
                else { 
                    /* Invalid option - keep original line unchanged */
                    cout << "Invalid option!\n"; 
                    fout << line << "\n"; 
                }
            }
            else { 
                /* This is not the order we're looking for - copy it unchanged */
                fout << line << "\n"; 
            }
        }
        /* Close input file */
        fin.close(); 
        /* Close output temporary file */
        fout.close();
        
        /* Check if we found the order ID */
        if (!found) { 
            /* Order not found - delete temporary file and show error */
            remove("orders_tmp.txt"); 
            cout << "Order not found!\n"; 
            return; 
        }
        /* Replace old file with updated temporary file */
        remove(filename.c_str()); 
        rename("orders_tmp.txt", filename.c_str()); 
        /* Show success message */
        cout << "Updated successfully!\n";
    }

    /* deleteOrder: removes an order from file completely
       Reads file, skips matching order, rewrites file without it */
    void deleteOrder()
    {
        /* Declare variable for order ID to search for */
        int id; 
        /* Ask user which order to delete */
        cout << "Enter Order ID: "; 
        /* Read the ID from keyboard */
        cin >> id;
        
        /* Open file for reading existing orders */
        ifstream fin(filename);
        /* If file can't open, order doesn't exist */
        if (!fin) { cout << "Not found!\n"; return; }
        
        /* Open temporary file for writing (without the deleted order) */
        ofstream fout("orders_tmp.txt"); 
        
        /* Variable to hold each line from file */
        string line; 
        /* Boolean flag to track if we found the order to delete */
        bool found = false;
        
        /* Loop through every line in file */
        while (getline(fin, line))
        {
            /* Declare variables for parsed order data */
            int fid; string a,b,c; string d,e; double dist; int eta;
            /* Try to parse the line
               If parsing fails, skip to next line */
            if (!parseLine(line, fid, a, b, d, e, dist, eta)) continue;
            
            /* Check if this is the order to delete */
            if (fid == id) { 
                /* Mark that we found the order */
                found = true; 
                /* Skip this line (don't write to temporary file) */
                continue; 
            }
            /* This is not the order to delete - copy it to temporary file */
            fout << line << "\n";
        }
        /* Close input file */
        fin.close(); 
        /* Close output temporary file */
        fout.close();
        
        /* Check if we found the order to delete */
        if (!found) { 
            /* Order not found - delete temporary file and show error */
            remove("orders_tmp.txt"); 
            cout << "Not found!\n"; 
            return; 
        }
        /* Replace old file with updated temporary file (without deleted order) */
        remove(filename.c_str()); 
        rename("orders_tmp.txt", filename.c_str()); 
        /* Show success message */
        cout << "Deleted!\n";
    }

    /* Destructor: cleanup function called when OrderSystem object is deleted
       Empty body because nothing special needs cleanup */
    ~OrderSystem() {}
};

/* LOGIN FUNCTION: authenticates user and determines access level */
/*================ LOGIN ================*/
string login(bool& isAdmin)
{
    /* Declare variables to store username and password */
    string user, pass;

    /* Print login header */
    cout << "\n=========== LOGIN ===========\n";

    /* Ask user for username */
    cout << "Username: ";
    /* Read username from keyboard into user variable */
    cin >> user;

    /* Ask user for password */
    cout << "Password: ";
    /* Read password from keyboard into pass variable */
    cin >> pass;

    /* Check if username AND password both match admin credentials */
    if (user == "admin" && pass == "admin123")
    {
        /* Set isAdmin to true because user successfully logged in as admin */
        isAdmin = true;
        /* Return the username (used for logging) */
        return user;
    }
    /* Username or password is incorrect */
    else
    {
        /* Set isAdmin to false - regular customer access only */
        isAdmin = false;
        /* Return the username anyway */
        return user;
    }

    /* This line never executes (would throw error if reached) */
    throw runtime_error("Invalid login!");
}

/* MAIN PROGRAM: starting point where application begins execution */
/*================ MAIN ================*/
int main()
{
    /* Create an OrderSystem object to manage all delivery orders */
    OrderSystem system;

    /* Add 3 sample orders to demonstrate the system */
    /* Create and add first order: Order ID 101 */
    system.addOrder(new StandardDelivery(101, "ali", "Omar", "Adana", "Cukurova", 20, 25));
    /* Create and add second order: Order ID 102 */
    system.addOrder(new StandardDelivery(102, "ali", "Omar", "Seyhan", "Yuregir", 14, 18));
    /* Create and add third order: Order ID 103 */
    system.addOrder(new StandardDelivery(103, "sara", "Ahmad", "Tarsus", "Mersin", 35, 40));

    /* Main infinite loop - program runs until user exits */
    while (true)
    {
        /* try-catch block: catches errors if they occur */
        try
        {
            /* Declare variable to track if user is admin */
            bool isAdmin;
            /* Call login function and get username (pass isAdmin by reference) */
            string user = login(isAdmin);

            /* Declare variable for user's menu choice */
            int choice;

            /* Inner loop: keep showing menu until user logs out */
            while (true)
            {
                /* Check if logged-in user is admin */
                if (isAdmin)
                {
                    /* Show ADMIN menu with full options */
                    cout << "\n========== ADMIN PANEL ==========\n";
                    cout << "1. Create Order\n";
                    cout << "2. View Orders\n";
                    cout << "3. View Order By ID\n";
                    cout << "4. Update Order\n";
                    cout << "5. Delete Order\n";
                    cout << "6. Logout\n";

                    /* Ask admin to choose action */
                    cout << "Choice: ";
                    /* Read choice from keyboard */
                    cin >> choice;

                    /* Execute chosen action */
                    if (choice == 1) system.createOrder();
                    else if (choice == 2) system.viewAllOrders();
                    else if (choice == 3) system.viewOrderById();
                    else if (choice == 4) system.updateOrder();
                    else if (choice == 5) system.deleteOrder();
                    /* Choice 6: Logout */
                    else if (choice == 6) break;  /* Exit inner while loop */
                }
                /* User is regular customer (not admin) */
                else
                {
                    /* Show CUSTOMER menu with limited options */
                    cout << "\n========== CUSTOMER ==========\n";
                    cout << "1. View All Orders\n";
                    cout << "2. View Order By ID\n";
                    cout << "3. Logout\n";

                    /* Ask customer to choose action */
                    cout << "Choice: ";
                    /* Read choice from keyboard */
                    cin >> choice;

                    /* Execute chosen action */
                    if (choice == 1) system.viewAllOrders();
                    else if (choice == 2) system.viewOrderById();
                    /* Choice 3: Logout */
                    else if (choice == 3) break;  /* Exit inner while loop */
                    /* Invalid choice number */
                    else cout << "Invalid choice!\n";
                }
            }
        }
        /* catch block: executes if exception (error) is thrown */
        catch (exception& e)
        {
            /* Print error message to user */
            cout << "Error: " << e.what() << endl;
        }
    }
    /* main() function ends here (program never actually reaches this point) */
}
