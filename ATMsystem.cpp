#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

string clientFileLocation = "clientData.txt";

void showMainManueScreen();
void showLoginScreen();
void showQuickWithdrawScreen();
typedef struct stClient{
    string accountNumber;
    string pinCode;
    string name;
    string phone;
    double balance;
}stClient;

enum enMainMenueOptions {
    eQuickWithdraw  = 1,
    eNormalWithdraw = 2,
    eDeposit        = 3,
    eCheckBalance   = 4,
    eLogout         = 5
};

enum enQuickWithdraw {
    e20     = 1,
    e50     = 2,
    e100    = 3,
    e200    = 4,
    e400    = 5,
    e600    = 6,
    e800    = 7,
    e1000   = 8,
    eExit   = 9
};

short readShort(string message) {
    short input = 0;
    cout << message;
    cin >> input;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "invalid input." << endl;
        cout << message;
        cin >> input;
    }
    return input;
}
double readDouble(string message) {
    double input = 0;
    cout << message;
    cin >> input;
    while (cin.fail() || input <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "invalid input." << endl;
        cout << message;
        cin >> input;
    }
    return input;
}
int readInteger(string message) {
    int input = 0;
    cout << message;
    cin >> input;
    while (cin.fail() || input <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "invalid input." << endl;
        cout << message;
        cin >> input;
    }
    return input;
}
char readChar(string message) {
    char input = 'N';
    cout << message;
    cin >> input;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "invalid input." << endl;
        cout << message;
        cin >> input;
    }
    return input;
}

stClient currentClient;
vector<stClient> vAllClient;

vector<string> splitString(string dataLine, string seperater = "#//#") {
    short pos = 0;
    string word = "";
    vector<string> vWords;
    while ((pos = dataLine.find(seperater)) != string::npos) {
        word = dataLine.substr(0, pos);
        if (word != "") {
            vWords.push_back(word);
        }
        dataLine.erase(0, pos + seperater.length());
    }
    if (dataLine != "") {
        vWords.push_back(dataLine);
    }
    return vWords;
}

string replaceString(string text, string toReplace, string replaceTo) {
    short pos = text.find(toReplace);
    while (pos != string::npos) {
        text.replace(pos, toReplace.length(), replaceTo);
        pos = text.find(toReplace);
    }
    return text;
}

stClient convertDataLineToRecord(string dataLine) {
    vector<string> dataRecord = splitString(dataLine);
    stClient client;
    client.accountNumber = dataRecord[0];
    client.pinCode = dataRecord[1];
    client.name = dataRecord[2];
    client.phone = dataRecord[3];
    client.balance = stod(dataRecord[4]);
    return client;
}
string convertRecordToDataLine(stClient &client, string delimeter = "#//#") {
    string dataLine = "";
    dataLine += client.accountNumber + delimeter;
    dataLine += client.pinCode + delimeter;
    dataLine += client.name + delimeter;
    dataLine += client.phone + delimeter;
    dataLine += to_string(client.balance);
    return dataLine;
}
bool saveDateToFile(vector<stClient> &vClient, string fileLocation) {
    fstream fs;
    fs.open(fileLocation, ios::out);
    if (fs.is_open()) {
        for (stClient & c : vClient) {
            fs << convertRecordToDataLine(c) << endl;
        }
        fs.close();
        return true;
    }
    fs.close();
    return false;
}

vector<stClient> loadDataFromFile(string fileLocation) {
    fstream fs;
    string dataLine = "";
    vector<stClient> vClient;
    fs.open(fileLocation, ios::in);
    if (fs.is_open()) {
        while (getline(fs,dataLine)) {
            if (dataLine != "") {
                vClient.push_back(convertDataLineToRecord(dataLine));
            }
        }
        fs.close();
    }
    return vClient;
}
void backToMainManueScreen() {
    cout << "\n\npress any key to leave this screen...";
    system("pause>0");
    showMainManueScreen();
}
void backToQuickWithdrawScreen() {
    cout << "\n\npress any key to leave this screen...";
    system("pause>0");
    showQuickWithdrawScreen();
}
short readQuickWithdrawOptions(string message) {
    short input = 0;
    cout << message;
    cin >> input;
    while (cin.fail() || input < 1 || input > 9) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "invalid input." << endl;
        cout << message;
        cin >> input;
    }
    return input;
}
bool checkBalanceValid(stClient client, double value) {
    if (client.balance < value) {
        return false;
    }
    return true;
}
bool checkValueValid(stClient client, double value) {
    if (value <= 0) {
        return false;
    }
    return true;
}
bool reloadDataAfterTransaction(vector<stClient> &vClient, stClient &client, double value) {
    for (stClient& c : vAllClient) {
        if (c.accountNumber == currentClient.accountNumber) {
            currentClient.balance -= value;
            c = currentClient;
            return true;
        }
    }
    return false;
}
void transactionType(double value, bool type) { // type true == withdraw || false == deposit
    if (type) {
        if (checkBalanceValid(currentClient, value)) {
            if (reloadDataAfterTransaction(vAllClient, currentClient, value)) {
                if (saveDateToFile(vAllClient, clientFileLocation)) {
                    cout << "\n\nAction successfully. New balance is: " << currentClient.balance << '\n';
                }
                else {
                    cout << "Action falied: Data not saved after transaction.\n";
                }
            }
            else {
                cout << "Action falied: Data not reloaded after transaction.\n";
            }
        }
        else {
            cout << "Action falied: you dont have enough balance.\n";
        }
    }
    else {
        if (checkValueValid(currentClient, value)) {
            value *= -1;
            if (reloadDataAfterTransaction(vAllClient, currentClient, value)) {
                if (saveDateToFile(vAllClient, clientFileLocation)) {
                    cout << "\n\nAction successfully. New balance is: " << currentClient.balance << '\n';
                }
                else {
                    cout << "Action falied: Data not saved after transaction.\n";
                }
            }
            else {
                cout << "Action falied: Data not reloaded after transaction.\n";
            }
        }
        else {
            cout << "Action falied: you dont have enough balance.\n";
        }
    }
}
void acceptTransaction(double value, bool type = true) {
    char answer = 'N';
    answer = readChar("\n\nAre you sure you want perform this transaction? y/n? ");
    if (toupper(answer) == 'Y') {
        transactionType(value, type);
    }
}
void performQuickWithdrawOptions(enQuickWithdraw quickWithdrawOptions) {
    switch (quickWithdrawOptions) {
    case enQuickWithdraw::e20:
        acceptTransaction(20);
        backToQuickWithdrawScreen();
        break;
    case enQuickWithdraw::e50:
        acceptTransaction(50);
        backToQuickWithdrawScreen();
        break;
    case enQuickWithdraw::e100:
        acceptTransaction(100);
        backToQuickWithdrawScreen();
        break;
    case enQuickWithdraw::e200:
        acceptTransaction(200);
        backToQuickWithdrawScreen();
        break;
    case enQuickWithdraw::e400:
        acceptTransaction(400);
        backToQuickWithdrawScreen();
        break;
    case enQuickWithdraw::e600:
        acceptTransaction(600);
        backToQuickWithdrawScreen();
        break;
    case enQuickWithdraw::e800:
        acceptTransaction(800);
        backToQuickWithdrawScreen();
        break;
    case enQuickWithdraw::e1000:
        acceptTransaction(1000);
        backToQuickWithdrawScreen();
        break;
    case enQuickWithdraw::eExit:
        backToMainManueScreen();
        break;
    }
}
void showQuickWithdrawScreen() {
    system("cls");
    cout << "====================================================================================\n";
    cout << "\t\t\t Quick Withdraw Screen" << " (current user: " << currentClient.name << ")\n";
    cout << "====================================================================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "====================================================================================\n";
    cout << "Your Balance is " << currentClient.balance << '\n';
    performQuickWithdrawOptions((enQuickWithdraw)readQuickWithdrawOptions("Choose what to withdraw from [1] to [9]? "));
}
void showNormalWithdrawScreen() {
    system("cls");
    cout << "====================================================================================\n";
    cout << "\t\t\t Normal Withdraw Screen" << " (current user: " << currentClient.name << ")\n";
    cout << "====================================================================================\n";
    cout << "Your Balance is " << currentClient.balance << '\n';
    cout << "====================================================================================\n";
    int value = readInteger("Enter an Amount multiple of 5's ? ");
    while (((value % 5) != 0)) {
        value = readInteger("Enter an Amount multiple of 5's ? ");
    }
    acceptTransaction(value);
}
void showDepositScreen() {
    system("cls");
    cout << "====================================================================================\n";
    cout << "\t\t\t Deposit Screen" << " (current user: " << currentClient.name << ")\n";
    cout << "====================================================================================\n";
    cout << "Your Balance is " << currentClient.balance << '\n';
    cout << "====================================================================================\n";
    double value = readDouble("Enter a positive Deposit Amount? ");
    acceptTransaction(value, false);
}
void showBalanceScreen() {
    cout << "====================================================================================\n";
    cout << "\t\t\t Check Balance Screen" << " (current user: " << currentClient.name << ")\n";
    cout << "====================================================================================\n";
    cout << "Your Balance is " << currentClient.balance << '\n';
}

bool checkIfAccountNumberValid(vector<stClient> &vClient, stClient &client, string number) {
    for (stClient &c : vClient) {
        if (number == c.accountNumber) {
            client = c;
            return true;
        }
    }
    return false;
}

void performMainManueOptions(enMainMenueOptions mainManueOption) {
    switch (mainManueOption) {
    case enMainMenueOptions::eQuickWithdraw:
        system("cls");
        showQuickWithdrawScreen();
        backToMainManueScreen();
        break;
    case enMainMenueOptions::eNormalWithdraw:
        system("cls");
        showNormalWithdrawScreen();
        backToMainManueScreen();
        break;
    case enMainMenueOptions::eDeposit:
        system("cls");
        showDepositScreen();
        backToMainManueScreen();
        break;
    case enMainMenueOptions::eCheckBalance:
        system("cls");
        showBalanceScreen();
        backToMainManueScreen();
        break;
    case enMainMenueOptions::eLogout:
        system("cls");
        showLoginScreen();
        break;
    }
}
short readMainManueOptions(string message) {
    short input = 0;
    cout << message;
    cin >> input;
    while (cin.fail() || input < 1 || input > 5) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "invalid input." << endl;
        cout << message;
        cin >> input;
    }
    return input;
}

void showMainManueScreen() {
    system("cls");
    cout << "====================================================================================\n";
    cout << "\t\t\t ATM Main Manue Screen" << " (current user: " << currentClient.name << ")\n";
    cout << "====================================================================================\n";
    cout << "\t[1] Quick withdraw.\n";
    cout << "\t[2] Normal withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "====================================================================================\n";
    performMainManueOptions((enMainMenueOptions)readMainManueOptions("Choose what do you want to do? [1 to 5]? "));
}

void showLoginScreen() {
    bool LoginFail = false;
    string number, pinCode;
    stClient client;
    do {
        cout << "====================================================================================\n";
        cout << "\t\t\t Login Screen\n";
        cout << "====================================================================================\n";
        if (LoginFail) {
            cout << "invalid Account Number/PinCode!\n";
        }
        cout << "Account Number? ";
        getline(cin >> ws, number);
        LoginFail = !checkIfAccountNumberValid(vAllClient, client, number);
        if (!LoginFail) {
            cout << "Pin Code? ";
            getline(cin >> ws, pinCode);
            if (client.pinCode != pinCode) {
                LoginFail = true;
            }
        }
        system("cls");
    } while (LoginFail);
    currentClient = client;
    showMainManueScreen();
}

int main()
{
    vAllClient = loadDataFromFile(clientFileLocation);
    showLoginScreen();
}