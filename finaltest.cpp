#include <iostream>
#include <fstream>
#include <string>
#include<chrono>
using namespace std;
using namespace std::chrono;
struct User {
    string name;
    string birthday;
    string email;
    string password;
    string phone;
    bool employee;
};
struct MenuItem {
    int id;
    string name;
    float price;
};
struct ordereditem{
string name;
float price;
int quantity;
};

struct order{
string customername;
string orderdate;
float totalprice;
ordereditem* ordereditems;
};
const float tax=0.11;
string getTodaysDate() {
    auto now = system_clock::now();
    time_t now_c = system_clock::to_time_t(now);
    tm* currentDate = localtime(&now_c);

    int day = currentDate->tm_mday;
    int month = currentDate->tm_mon + 1;
    int year = currentDate->tm_year + 1900;

    string date = to_string(day) + "-" + to_string(month) + "-" + to_string(year);

    return date;
}

bool isBirthdayToday(const string& birthday) {
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);
    tm tm = *localtime(&t);

    int current_day = tm.tm_mday;
    int current_month = tm.tm_mon + 1; // Month is 0-indexed in std::tm

    string::size_type pos1 = birthday.find('-');
    string::size_type pos2 = birthday.rfind('-');

    int day = stoi(birthday.substr(0, pos1));
    int month = stoi(birthday.substr(pos1 + 1, pos2 - pos1 - 1));

    return (current_day == day && current_month == month);
}
void loadmenuFromFile(MenuItem*& menu, int& count) {
    ifstream m("MI.txt");
    if (!m) {
        cout << "Error opening file. No menu data loaded." << endl;
        return;
    }
    m >> count;
    if (menu != nullptr) {
        delete[] menu;
    }
    menu = new MenuItem[count];
    for (int i = 0; i < count; i++) {
        m >> menu[i].id >> menu[i].name >> menu[i].price;
    }
    m.close();
    cout << "Menu items loaded successfully." << endl;
}
void loadUsersFromFile(User*& users, int& numUsers) {
    ifstream inputFile("US.txt");
    if (!inputFile) {
        cout << "Error opening file. No user data loaded." << endl;
        return;
    }
    inputFile >> numUsers;
    users = new User[numUsers];
    for (int i = 0; i < numUsers; i++) {
        inputFile >> users[i].name >> users[i].birthday >> users[i].email
            >> users[i].password >> users[i].phone >> users[i].employee;
    }
    inputFile.close();
    cout << "User data loaded successfully." << endl;
}

void saveUsersToFile(User* users, int numUsers) {
    ofstream outputFile("US.txt");
    if (!outputFile) {
        cout << "Error opening file. No user data saved." << endl;
        return;
    }

    outputFile << numUsers << endl;
    for (int i = 0; i < numUsers; i++) {
        outputFile << users[i].name << " " << users[i].birthday << " "
            << users[i].email << " " << users[i].password << " "
            << users[i].phone << " " << users[i].employee << endl;
    }
    outputFile.close();
    cout << "User data saved successfully." << endl;
}
void printMenu(MenuItem* menu, int count) {
    cout << "*--------Menu--------*" << endl;
    cout<<"ID\t name\t\tprice"<<endl;
    for (int i = 0; i < count; i++) {
        cout <<"("<<menu[i].id << ")\t " << menu[i].name << "  \t$" << menu[i].price << endl;
    }
}

void placeOrder(User u, MenuItem* menu, int menuCount) {
    ordereditem* orderedItems = new ordereditem[menuCount];
    int itemCounter = 0;
    float total = 0;
    string today=getTodaysDate();
    printMenu(menu, menuCount);
    while (true) {
        cout << "Enter the ID of the menu item you want to order (or -1 to finish order): ";
        int itemId;
        cin >> itemId;
        if (itemId == -1) {
            break;
        }
        bool itemFound = false;
        for (int i = 0; i < menuCount; i++) {
            if (menu[i].id == itemId) {
                orderedItems[itemCounter].name = menu[i].name;
                orderedItems[itemCounter].price = menu[i].price;
                cout << "Enter the quantity: ";
                int quantity;
                cin >> quantity;
                orderedItems[itemCounter].quantity = quantity;
                itemCounter++;
                total += menu[i].price * quantity;
                itemFound = true;
                break;
            }
        }
        if (!itemFound) {
            cout << "Item not found in menu." << endl;
        }
    }
    if (itemCounter == 0) {
        cout << "You didn't order anything." << endl;
        delete[] orderedItems;
        return;
    }
    if (isBirthdayToday(u.birthday)) {
        total *= 0.2;
        cout << "Happy birthday " << u.name << "! You get a 50% discount!" << endl;
    }
    total *= 1.0 + tax;
    cout << "Order total: $" << total << endl;
    order newOrder = { u.name, today, total, orderedItems };
    ofstream outfile("orders.txt", ios_base::app);
    if (!outfile) {
        cerr << "Failed to open orders.txt" << endl;
    }
    else {
        outfile << "Customer name: " << newOrder.customername << endl;
        outfile << "Order date: " << newOrder.orderdate << endl;
        outfile << "Total amount: $" << newOrder.totalprice << endl;
        outfile << "Ordered items:" << endl;
        for (int i = 0; i < itemCounter; i++) {
            outfile << "- " << orderedItems[i].name << ", quantity: " << orderedItems[i].quantity << ", price: $" << orderedItems[i].price << endl;
        }
        outfile << endl;
        outfile.close();
        cout << "Order placed successfully and saved to orders.txt." << endl;
    }

    delete[] orderedItems;
}

void customermenu(User u){
int choice=0;
User* user;
MenuItem*menu=nullptr;
int count=0;
loadmenuFromFile(menu,count);
while (true){cout<<"################Welcome to the Main Menu Page################"<<endl;
    cout<<"1-food selection"<<endl;
    cout<<"2-place order"<<endl;
    cout<<"3-exit"<<endl;
    cin>>choice;
    switch(choice){
    case 1:
    printMenu(menu,count);
    break;
    case 2:
    placeOrder(u,menu,count);
    break;
    case 3:
    delete[] menu;
    return ;
    default:
            cout << "Invalid option selected!" << endl;
            break;
    }}
   
    }





void addMenuItem(MenuItem*& menu, int& count) {
    cout << "Enter the ID of the new menu item: ";
    int id;
    cin.ignore();
    cin >> id;

    cout << "Enter the name of the new menu item: ";
   string name;
   cin.ignore();
    getline(cin,name);
 float price;
    cout << "Enter the price of the new menu item: ";
    cin >> price;

    MenuItem newItem = { id, name, price };
    count++;
    MenuItem* newMenu = new MenuItem[count];
    for (int i = 0; i < count-1; i++) {
        newMenu[i] = menu[i];
    }
    newMenu[count-1] = newItem;
   
if (menu != nullptr) {
        delete[] menu;
    }
    menu = newMenu;

    cout << "Menu item added successfully!" << endl;
}

void saveMenuToFile(MenuItem* menu, int count) {
    ofstream m("MI.txt");
    if (!m) {
        cout << "Error opening file. No user data saved." << endl;
        return;
    }
    m << count << endl;
    for (int i = 0; i < count; i++) {
        m << menu[i].id << " " << menu[i].name << " " << menu[i].price << endl;
    }
    m.close();
    cout << "Menu items saved to file" << endl;
}
void removeMenuItem(MenuItem*& menu, int& count) {
    int r;
    cout << "Enter the ID of the menu item to remove: ";
    cin >> r;

    int t = -1;
    for (int i = 0; i < count; i++) {
        if (menu[i].id == r) {
            t = i;
            break;
        }
    }

    if (t == -1) {
        cout << "Menu item not found." << endl;
        return;
    }

    count--;
    MenuItem* newMenu = new MenuItem[count];
    for (int i = 0; i < t; i++) {
        newMenu[i] = menu[i];
    }
    for (int i = t + 1; i < count + 1; i++) {
        newMenu[i - 1] = menu[i];
    }

    if (menu != nullptr) {
        delete[] menu;
    }
    menu = newMenu;

    saveMenuToFile(menu, count);
    loadmenuFromFile( menu, count);


    cout << "Menu item removed successfully!" << endl;
}

int findindex(MenuItem*menu,int count,int id){
for(int i=0; i <  count; i++){
if(menu[i].id==id)
return i;}
return -1;
}

void editmenuitem(MenuItem* menu, int count) {
    cout << "Enter the ID of the menu item to edit: ";
    int id, in, op;
    float price;
    string name;
    cin >> id;
    in = findindex(menu, count, id);
    if (in == -1) {
        cout << "Menu item with ID " << id << " not found." << endl;
        return;
    }

    cout << "Select an option:" << endl;
    cout << "1. Edit name" << endl;
    cout << "2. Edit price" << endl;
    cin >> op;

    switch (op) {
    case 1:
        cout << "Enter the new name of the menu item: ";
        cin >> name;
        menu[in].name = name;
        break;
    case 2:
        cout << "Enter the new price of the menu item: ";
        cin >> price;
        menu[in].price = price;
        break;
    default:
        cout << "Invalid option selected!" << endl;
        break;
    }
}

void registerUser(User*& users, int& numUsers) {
    User u;
    cout << "Please enter your name: ";
    getline(cin, u.name);
    for(int i=0;i<numUsers;i++){
    if(u.name==users[i].name){
    cout<<"name already exists!!"<<endl;
    cin.ignore();
    cout << "Please enter your name: ";
    getline(cin, u.name);
    i=-1;}
    }

    cout << "Please enter your birthday(dd-mm-yyyy): ";
    getline(cin, u.birthday);
    cout << "Please enter your email: ";
    getline(cin, u.email);
    cout << "Please enter your password: ";
    getline(cin, u.password);
    cout << "Please enter your phone number: ";
    getline(cin, u.phone);
    cout << "Are you an employee? (1 for yes, 0 for no): ";
    cin >> u.employee;
    cin.ignore();
    numUsers++;
    User* temp = new User[numUsers];
    for (int i = 0; i < numUsers - 1; i++) {
        temp[i] = users[i];
    }
    temp[numUsers - 1] = u;
    delete[] users;
    users = temp;
    cout << "Registration complete." << endl;
    saveUsersToFile(users, numUsers);
}

int findUser(User* users, int numUsers, string email, string password) {
    for (int i = 0; i < numUsers; i++) {
        if (users[i].email == email && users[i].password == password) {
            return i;
        }
    }
    return -1;
}
void employeemenu(){MenuItem* menu = nullptr;
int count = 0;
loadmenuFromFile(menu,count);
while (true) {
        cout << "Select an option:" << endl;
        cout << "1. Print menu" << endl;
        cout << "2. Add menu item" << endl;
        cout << "3. edit menu item" << endl;
        cout<<"4.remove menu item"<<endl;
        cout << "5. Exit" << endl;

        int option;
        cin >> option;

        switch (option) {
        case 1:
            printMenu(menu, count);
            break;
        case 2:
            addMenuItem(menu, count);
             saveMenuToFile(menu, count);
            break;
        case 3:
           editmenuitem(menu,count);
             saveMenuToFile(menu, count);
            break;
        case 4:
        removeMenuItem(menu, count);
        break;
        case 5:
            delete[] menu;
            return ;
        default:
            cout << "Invalid option selected!" << endl;
            break;
        }
    }
}






void login(User* users, int numUsers) {
    string email, password;
    User u;
    cout << "Please enter your email: ";
    getline(cin, email);
    cout << "Please enter your password: ";
    getline(cin, password);
   int index = findUser(users, numUsers, email, password);
    if (index == -1) {
        cout << "Login failed. Invalid email or password." << endl;
    
    }
    else {
        cout << "Login successful." << endl;
        if (users[index].employee) {
            cout << "Welcome, employee " << users[index].name << "!" << endl;
                employeemenu();
        }
        else {
            cout << "Welcome, " << users[index].name << "!" << endl;
            u.name=users[index].name;
              u.birthday=users[index].birthday;
                u.email=users[index].email;
                u.password=users[index].password;
                u.phone=users[index].phone;
                u.employee=users[index].employee;
                    customermenu(u);
        }
    }
}


void displayMenu() {
cout<<"\twelcome to progII resto!!"<<endl;
cout<<"**********************************************"<<endl;
cout << "1. Register as a new user" << endl;
cout << "2. Log in" << endl;
cout << "3. Quit" << endl;
}

int main(){
User* users = nullptr;
MenuItem* menu = nullptr;
int numUsers = 0;
int count = 0;
loadmenuFromFile(menu,count);
loadUsersFromFile(users, numUsers);
int choice = 0;
do {
displayMenu();
cin >> choice;
cin.ignore();
switch (choice) {
case 1:
registerUser(users, numUsers);
break;
case 2:
login(users, numUsers);
break;
case 3:
cout << "Goodbye!" << endl;
break;
default:
cout << "Invalid choice. Please try again." << endl;
break;
}
} while (choice != 3);
delete[] users;
}