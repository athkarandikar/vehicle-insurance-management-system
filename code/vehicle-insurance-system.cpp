#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <cstring>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

class Company {
    string name = "Ensure Ltd.";
    string officeAddress = "Ensure Ltd. Office 3, Koregoan Park, Pune - 411001, Maharashtra, India";
    string contactNo = "+91-9822266622";

    public:
        string getCompanyName() {
            return name;
        }

        string getCompanyOfficeAddress() {
            return officeAddress;
        }

        string getCompanyContactNo() {
            return contactNo;
        }
};

class BaseUi {
    public:
        void print(vector<string> texts) {  // prints strings one below another
            for (auto text : texts) {
                cout << text << endl;
            }
        }

        void printInCenter(string text) {  // prints a string in center
            int width = (149 - text.length()) / 2;
            cout.width(width + text.length());
            cout.fill(' ');
            cout << right << text << endl;
        }

        void printInCenter(vector<string> texts) {  // prints vector of strings in center
            for (auto text : texts) printInCenter(text);
        }

        vector<string> insertTextInBox(string text) {  // returns a string inside a box
            string displayText = "| " + text + " |";
            string border(displayText.length(), '-');
            return vector<string> {border, displayText, border};
        }

        vector<string> insertTextsInBox(vector<string> texts) {  // returns vector of strings inside a box
        // To have properly displayed text in the box, all the strings should either have odd length, or even length.
        // If the strings have mixed length, prepend a space to the strings with odd length.
            bool allHaveOddLength = true;

            for (auto text : texts) {  // checks if all the strings have odd length
                if (text.length() % 2 == 0) {
                    allHaveOddLength = false;
                    break;
                }
            }

            if (!allHaveOddLength) {  // if all do not have odd lengths, then make it odd
                for (int i = 0; i < texts.size(); i++) {
                    if (texts[i].length() % 2 == 1) {
                        texts[i] = " " + texts[i];  // prepends a space to make length even
                    }
                }
            }

            string maxString = "";
            for (auto text : texts) {
                if (text.length() > maxString.length()) {
                    maxString = text;
                }
            }
            int maxLength = maxString.length();

            vector<string> displayTexts;  // stores the texts to display in the box
            string border(maxLength + 4, '-');

            displayTexts.push_back(border);
            for (auto text : texts) {  // inserts borders around the texts
                string displayText;
                if (text.length() < maxLength) {
                    int width = (maxLength - text.length()) / 2;
                    displayText = "| " + string(width, ' ') + text + string(width, ' ') + " |";
                } else {
                    displayText = "| " + text + " |";
                }
                displayTexts.push_back(displayText);
            }
            displayTexts.push_back(border);

            return displayTexts;
        }

        void printInBoxInCenter(string text) {  // prints a string in box in center
            printInCenter(insertTextInBox(text));
        }

        void printInBoxInCenter(vector<string> texts) {  // prints vector of strings in box in center
            printInCenter(insertTextsInBox(texts));
        }

        void printInBox(string text) {  // prints a string in box
            for (auto line : insertTextInBox(text)) {
                cout << line << endl;
            }
        }

        void printInBox(vector<string> texts) {  // prints vector of strings in box
            for (auto text : insertTextsInBox(texts)) {
                cout << text << endl;
            }
        }

        void printKeyAndValue(string key, string value) {  // prints key in box and its value after it
            vector<string> displayTexts;
            vector<string> temp = insertTextInBox(key);

            for (int j = 0; j < temp.size(); j++) {
                if (j == 1) {
                    displayTexts.push_back(temp.at(j) + " : " + value);
                } else {
                    displayTexts.push_back(temp.at(j));
                }
            }
            print(displayTexts);
        }

        void printKeysAndValues(vector<string> keys, vector<string> values) {  // prints keys in boxes & their values after them
            int minSize = (keys.size() < values.size()) ? keys.size() : values.size();

            for (int i = 0; i < minSize; i++) printKeyAndValue(keys.at(i), values.at(i));
        }

        string formatCurrency(string money) {  // returns a formatted currency string (Indian rules for commas)
            string amount = money;

            if (amount.size() <= 3) return amount;

            int validSize = amount.size();
            string formattedAmount;

            while (validSize > 2) {
                string temp;
                if (validSize == amount.size()) {
                    temp = amount.substr(validSize - 3, 3);
                    validSize -= 3;
                } else {
                    temp = amount.substr(validSize - 2, 2);
                    validSize -= 2;
                }
                formattedAmount = "," + temp + formattedAmount;
            }

            return amount.substr(0, validSize) + formattedAmount;
        }

        void printCustomerWithPolicies(string customerId, vector<string> policyNos, vector<string> premiums) {  // prints customer ID, associated policy numbers & their premiums, & their total premium
            printInBoxInCenter("Customer ID : " + customerId);

            int totalPremium = 0;
            string displayText;
            displayText += "Policy No " + policyNos.at(0) + " : Rs. " + formatCurrency(premiums.at(0));
            totalPremium += stoi(premiums.at(0));

            for (int i = 1; i < policyNos.size(); i++) {
                displayText += ", Policy No " + policyNos.at(i) + " : Rs. " + formatCurrency(premiums.at(i));
                totalPremium += stoi(premiums.at(i));
            }

            printKeyAndValue("Policies & Premiums", displayText);
            printKeyAndValue("Total Premium", "Rs. " + formatCurrency(to_string(totalPremium)));
        }
};

class BaseUtility {
    public:
        string inputChoice(string &choice, vector<string> validChoices = {"1","2","3","4","5","6","7","8","C","Q"}, string indent = "") {  // takes input from user of a choice from a list of valid choices
            cout << "\n" << indent << "Enter your choice: ";
            cin >> choice;
            choice = toupper(choice[0]);

            for (auto validChoice : validChoices) {
                if (choice == validChoice) return choice;  // returns choice itself if choice is valid
            }
            return "0";  // returns string 0 if choice is invalid
        }

        string getFileLine(string fileDirectory, int policyNo, int fileLineNo) {  // gets a line from a customer or a policy file
            string fileDir = "./Database/Policies/";
            string fileName = "policy_" + to_string(policyNo);

            if (fileDirectory == "customer" || fileDirectory == "Customer") {
                fstream file(fileDir + fileName + ".txt", ios::in);

                if (file.is_open()) {
                    string customerId;
                    for (int i = 1; i <= 2; i++) file >> customerId;
                    fileName = "customer_" + customerId;
                }
                fileDir = "./Database/Customers/";
                file.close();
            }

            fstream file(fileDir + fileName + ".txt", ios::in);
            string line;

            if (file.is_open()) {
                for (int i = 1; i <= fileLineNo; i++) getline(file, line);
            }
            file.close();
            return line;
        }

        string capitalizeFirstLetter(string text) {  // capitalizes & returns every first letter of every word in a string
            string capitalizedText = "";

            capitalizedText += toupper(text[0]);

            for (int i = 1; i < text.length(); i++) {
                if (text.at(i) == ' ') {
                    capitalizedText += text.at(i);
                    capitalizedText += toupper(text[++i]);
                } else capitalizedText += text[i];
            }
            return capitalizedText;
        }
};

class Customer : public BaseUi, virtual public BaseUtility {
    int customerId;
    string namePrefix;  // Mr., Mrs., or Mx.
    string firstName;
    string middleName;
    string lastName;
    int birthDate;
    int birthMonth;
    int birthYear;
    string occupation;
    string residenceNo;
    string residenceName;
    string residenceStreet;
    string residenceCity;
    string residencePinCode;
    string residenceState;

    int setCustomerId() {  // sets customer id
        fstream file("./Database/Customers/all_customers.txt", ios::in);
        int id;

        if (file.is_open()) {  // if file is present, sets the customer id to last customer id + 1
            while (file >> id) {}  // stores last customer id in id
            customerId = id + 1;
            file.close();

            return customerId;
        } else {  // if file is not present, it creates the file & sets the customer id to 1
            fs::create_directories("./Database/Customers");
            fstream file("./Database/Customers/all_customers.txt", ios::out);  // creates file
            customerId = 1;
            file.close();

            return customerId;
        }
        file.close();
        return false;  // returns false if file could not be opened
    }

    bool writeCustomerIdToFile() {  // writes customer id to file "all_customers.txt"
        fstream file("./Database/Customers/all_customers.txt", ios::app);

        if (file.is_open()) {
            file << customerId << endl;
            file.close();
            return true;
        }
        file.close();
        return false;
    }

    public:
        int getCustomerId() {  // returns customer id
            return customerId;
        }

        bool findCustomerId(int customerId) {  // finds if a customer id exists
            fstream file("./Database/Customers/all_customers.txt", ios::in);
            int id;

            if (file.is_open()) {
                while (file >> id) {
                    if (id == customerId) {
                        this->customerId = id;
                        return true;
                    }
                }
            }
            return false;
        }

        void createCustomer() {  // creates a new customer
            cout << "\nEnter the following details about the customer:" << endl;

            cout << "  Name Prefix (Mr, Mrs, or Mx): ";  // Mx for transgender
            cin >> namePrefix;
            namePrefix = capitalizeFirstLetter(namePrefix);

            cout << "  First Name: ";
            getline(cin>>ws, firstName);
            firstName = capitalizeFirstLetter(firstName);

            cout << "  Middle Name: ";
            getline(cin>>ws, middleName);
            middleName = capitalizeFirstLetter(middleName);

            cout << "  Last Name: ";
            getline(cin>>ws, lastName);
            lastName = capitalizeFirstLetter(lastName);

            cout << "  Birth Date (DD): ";
            cin >> birthDate;
            cout << "  Birth Month (MM): ";
            cin >> birthMonth;
            cout << "  Birth Year (YYYY): ";
            cin >> birthYear;

            cout << "  Occupation: ";
            getline(cin>>ws, occupation);
            occupation = capitalizeFirstLetter(occupation);

            cout << "  Residence No: ";
            cin >> residenceNo;
            residenceNo = capitalizeFirstLetter(residenceNo);

            cout << "  Residence Name: ";
            getline(cin>>ws, residenceName);
            residenceName = capitalizeFirstLetter(residenceName);

            cout << "  Residence Street / Chowk: ";
            getline(cin>>ws, residenceStreet);
            residenceStreet = capitalizeFirstLetter(residenceStreet);

            cout << "  Residence City: ";
            getline(cin>>ws, residenceCity);
            residenceCity = capitalizeFirstLetter(residenceCity);

            cout << "  Residence Pin Code: ";
            getline(cin>>ws, residencePinCode);

            cout << "  Residence State: ";
            getline(cin>>ws, residenceState);
            residenceState = capitalizeFirstLetter(residenceState);

            setCustomerId();  // creates a new customer id
            writeCustomerIdToFile();  // writes customer id to file "all_customers.txt"
        }

        bool writeCustomerDetailsToFile() {  // writes customer details to file "customer_<customerId>.txt"
            fstream file("./Database/Customers/customer_" + to_string(customerId) + ".txt", ios::out);

            if (file.is_open()) {
                file << namePrefix << endl;
                file << firstName << endl;
                file << middleName << endl;
                file << lastName << endl;
                file << birthDate << endl;
                file << birthMonth << endl;
                file << birthYear << endl;
                file << occupation << endl;
                file << residenceNo << endl;
                file << residenceName << endl;
                file << residenceStreet << endl;
                file << residenceCity << endl;
                file << residencePinCode << endl;
                file << residenceState << endl;
                file.close();
                return true;
            }
            file.close();
            return false;
        }

        void editCustomer() {  // replaces existing customer details with new details
            int customerId;
            cout << "\nEnter the customer ID to edit the customer: ";
            cin >> customerId;

            if (findCustomerId(customerId)) {
                cout << "\nEnter the following details about the customer:" << endl;

                cout << "  Name Prefix (Mr, Mrs, or Mx): ";  // Mx for transgender
                cin >> namePrefix;
                namePrefix = capitalizeFirstLetter(namePrefix);

                cout << "  First Name: ";
                getline(cin>>ws, firstName);
                firstName = capitalizeFirstLetter(firstName);

                cout << "  Middle Name: ";
                getline(cin>>ws, middleName);
                middleName = capitalizeFirstLetter(middleName);

                cout << "  Last Name: ";
                getline(cin>>ws, lastName);
                lastName = capitalizeFirstLetter(lastName);

                cout << "  Birth Date (DD): ";
                cin >> birthDate;
                cout << "  Birth Month (MM): ";
                cin >> birthMonth;
                cout << "  Birth Year (YYYY): ";
                cin >> birthYear;

                cout << "  Occupation: ";
                getline(cin>>ws, occupation);
                occupation = capitalizeFirstLetter(occupation);

                cout << "  Residence No: ";
                cin >> residenceNo;

                cout << "  Residence Name: ";
                getline(cin>>ws, residenceName);
                residenceName = capitalizeFirstLetter(residenceName);

                cout << "  Residence Street / Chowk: ";
                getline(cin>>ws, residenceStreet);
                residenceStreet = capitalizeFirstLetter(residenceStreet);

                cout << "  Residence City: ";
                getline(cin>>ws, residenceCity);
                residenceCity = capitalizeFirstLetter(residenceCity);

                cout << "  Residence Pin Code: ";
                getline(cin>>ws, residencePinCode);

                cout << "  Residence State: ";
                getline(cin>>ws, residenceState);
                residenceState = capitalizeFirstLetter(residenceState);

                this->customerId = customerId;
                writeCustomerDetailsToFile();

                cout << "\nCustomer details of customer with customer ID " << customerId << " edited successfully!" << endl;
            } else {
                cout << "\nCustomer with customer ID " << customerId << " not found." << endl;
            }
        }

        void viewCustomers() {  // displays all customers with their policy numbers and premiums, and total premiums of each customer
            vector<string> customerIds, policyNos, premiums;
            bool anyPolicyExists = false;

            fstream file ("./Database/Customers/all_customers.txt", ios::in);

            if (file.is_open()) {  // stores all the existing customer ids in "all_customers.txt" file
                while (!file.eof()) {
                    string customerId;
                    getline(file, customerId);
                    customerIds.push_back(customerId);
                }
                file.close();
                customerIds.pop_back();
            }

            for (auto customerId : customerIds) {  // for each customer id you have
                policyNos.clear();
                premiums.clear();

                if (fs::is_directory("./Database/Policies")) {
                    for (const auto &content : fs::directory_iterator("./Database/Policies")) {  // iterate through all files in Database/Policies directory
                        string fileName = content.path().filename().string();
                        if (fileName == "all_policies.txt") {  // skip this file
                            continue;
                        }
                        anyPolicyExists = true;

                        fstream file("./Database/Policies/" + fileName, ios::in);

                        string fileCustomerId;
                        for (int i = 1; i <= 2; i++) getline(file, fileCustomerId);  // gets the customer id from the file

                        file.seekg(0, ios::beg);  // put the get pointer at the file beginning

                        if (file.is_open()) {
                            if (customerId == fileCustomerId) {  // if the for loop's customer id matches the file's customer id
                                for (int i = 1; i <= 20; i++) {
                                    string line;
                                    getline(file, line);

                                    if (i == 1) policyNos.push_back(line);
                                    else if (i == 20) premiums.push_back(line);
                                }
                            }
                        }
                        file.close();
                    }
                    printCustomerWithPolicies(customerId, policyNos, premiums);  // prints the customer details with policy numbers and premiums, and total premium of a single customer
                }
            }

            if (!anyPolicyExists) cout << "\nNo policy exists." << endl;
        }
};

class Vehicle : virtual public BaseUtility {
    int manufactureYear;
    string make;
    string model;
    int idv;  // insured's declared value (selling price of the vehicle at the time of purchase)
    string registrationNo;
    string engineNo;
    string chassisNo;
    int cc;  // cubic capacity
    int seatingCapacity;
    string registrationAuthority;

    public:
        int getIdv() {
            return idv;
        }

        void inputVehicleDetails() {  // takes input from the user of vehicle details
            cout << "\nEnter the following details about the vehicle:" << endl;

            cout << "  Manufacture Year (YYYY): ";
            cin >> manufactureYear;

            cout << "  Make (Manufacturer): ";
            getline(cin>>ws, make);
            make = capitalizeFirstLetter(make);

            cout << "  Model: ";
            getline(cin>>ws, model);
            model = capitalizeFirstLetter(model);

            cout << "  IDV (Insured's Declared Value) (in Rs.): ";
            cin >> idv;

            cout << "  Registration No (SSDDAANNNN): ";  // state district alphabets number
            cin >> registrationNo;
            transform(registrationNo.begin(), registrationNo.end(), registrationNo.begin(), ::toupper);  // capitalizes the whole string

            cout << "  Engine No: ";
            cin >> engineNo;
            transform(engineNo.begin(), engineNo.end(), engineNo.begin(), ::toupper);

            cout << "  Chassis No: ";
            cin >> chassisNo;
            transform(chassisNo.begin(), chassisNo.end(), chassisNo.begin(), ::toupper);

            cout << "  CC (Cubic Capacity): ";
            cin >> cc;

            cout << "  Seating Capacity (N): ";
            cin >> seatingCapacity;

            cout << "  Registration Authority: ";
            getline(cin>>ws, registrationAuthority);
            registrationAuthority = capitalizeFirstLetter(registrationAuthority);
        }

        bool writeVehicleDetailsToFile(int policyNo) {  // writes the vehicle details to the file "policy_<policyNo>.txt"
            fstream file("./Database/Policies/policy_" + to_string(policyNo) + ".txt", ios::app);

            if (file.is_open()) {
                file << manufactureYear << endl;
                file << make << endl;
                file << model << endl;
                file << idv << endl;
                file << registrationNo << endl;
                file << engineNo << endl;
                file << chassisNo << endl;
                file << cc << endl;
                file << seatingCapacity << endl;
                file << registrationAuthority << endl;
                file.close();
                return true;
            }
            file.close();
            return false;
        }
};

class Policy : public Customer, public Vehicle {
    int policyNo;
    int policyStartDate;
    int policyStartMonth;
    int policyStartYear;
    int policyEndDate;
    int policyEndMonth;
    int policyEndYear;
    int policyDuration;  // in years
    int premium;
    int percentCgst = 9;  // 9% of the premium
    int percentSgst = 9;  // 9% of the premium
    int stampDuty = 10;  // Rs. 10
    int policyTotalPrice;

    int setPolicyNo() {  // sets policy no
        fstream file("./Database/Policies/all_policies.txt", ios::in);
        int no;

        if (file.is_open()) {  // if file is present, sets the policy no to last policy no + 1
            while (file >> no) {}  // stores last policy no in no
            policyNo = no + 1;
            file.close();

            return policyNo;
        } else {  // if file is not present, it creates the file & sets the policy no to 1
            fs::create_directories("./Database/Policies");
            fstream file("./Database/Policies/all_policies.txt", ios::out);  // creates file
            policyNo = 1;
            file.close();

            return policyNo;
        }
        file.close();
        return false;  // returns false if file could not be opened
    }

    bool writePolicyNoToFile() {  // writes policy number to file "all_policies.txt"
        fstream file("./Database/Policies/all_policies.txt", ios::app);

        if (file.is_open()) {
            file << policyNo << endl;
            file.close();
            return true;
        }
        file.close();
        return false;
    }

    void inputPolicyDetails() {  // takes input from the user of policy details
        inputVehicleDetails();  // takes input from the user of vehicle details

    // Calculate policy start & end dates automatically.
        time_t now = time(0);  // current date
        tm *start = localtime(&now);  // parsing the current date

        policyDuration = 1;  // The duration will be 1 year for all policies.

        policyStartDate = start->tm_mday;  // start dates
        policyStartMonth = 1 + start->tm_mon;
        policyStartYear =  1900 + start->tm_year;

        policyEndDate = policyStartDate - 1;  // end dates
        policyEndMonth = policyStartMonth;
        policyEndYear = policyStartYear + 1;

        premium = (getIdv() * 0.1) * policyDuration;  // premium = 10% of the IDV * policy duration
        premium += premium * (percentCgst + percentSgst) / 100;  // premium = premium + CGST + SGST
        premium += stampDuty;  // premium = premium + stamp duty
    }

    bool writePolicyDetailsToFile() {  // writes policy details to file "policy_<policyNo>.txt"
        fstream file("./Database/Policies/policy_" + to_string(policyNo) + ".txt", ios::app);

        if (file.is_open()) {
            file << policyNo << endl;
            file << getCustomerId() << endl;
            writeVehicleDetailsToFile(policyNo);
            file << policyStartDate << endl;
            file << policyStartMonth << endl;
            file << policyStartYear << endl;
            file << policyEndDate << endl;
            file << policyEndMonth << endl;
            file << policyEndYear << endl;
            file << policyDuration << endl;
            file << premium << endl;
            file.close();
            return true;
        }
        file.close();
        return false;
    }

    bool findPolicyNo(int policyNo) {  // finds if a policy no exists
        fstream file("./Database/Policies/all_policies.txt", ios::in);
        int id;

        if (file.is_open()) {
            while (!file.eof()) {
                file >> id;
                if (id == policyNo) return true;
            }
        }
        return false;
    }

    int findPolicyUsingKey(string key, int fileLineNo) {  // finds & returns policy number using key
        if (fs::is_directory("./Database/Policies")) {
            for (const auto &content : fs::directory_iterator("./Database/Policies")) {  // searches for all files in Database/Policies directory
                string fileName = content.path().filename().string();

                if (fileName == "all_policies.txt") continue;  // skips this file

                fstream file("./Database/Policies/" + fileName, ios::in);
                int policyNo;
                string line;

                if (file.is_open()) {
                    file >> policyNo;
                    for (int i = 1; i <= fileLineNo; i++) getline(file, line);
                }
                file.close();
                if (line == key) return policyNo;
            }
        }
        return false;
    }

    public:
        void createPolicy() {  // creates a new policy
            string choice;

            cout << "\nChoose an option:\n" << endl;
            cout << "1) Create Policy for Existing Customer" << endl;
            cout << "2) Create Policy for New Customer" << endl;
            cout << "3) Go back" << endl;

            inputChoice(choice, {"1","2","3"});
            if (choice == "1") {
                int customerId;
                cout << "\nEnter Customer ID: ";
                cin >> customerId;

                if (findCustomerId(customerId)) cout << "Customer found." << endl;
                else {
                    cout << "Customer not found! Create a policy for a new customer." << endl;
                    return;
                }
            }
            else if (choice == "2") {
                createCustomer();  // creates a new customer
                writeCustomerDetailsToFile();  // writes customer details to file
            }
            if (choice == "1" || choice == "2") {
                setPolicyNo();  // creates a new policy no
                writePolicyNoToFile();  // writes policy no to file "all_policies.txt"

                inputPolicyDetails();  // takes input from the user of policy details
                writePolicyDetailsToFile();  // writes policy details to file "policy_<policyNo>.txt"

                cout << "\nPolicy with policy number " << policyNo << " created successfully for customer with customer ID " << getCustomerId() << "." << endl;
            } else if (choice == "3") {
                return;
            }
        }

        int findPolicy() {  // finds a policy
            string choice;

            cout << "\nChoose an option:\n" << endl;
            cout << "1) Find Policy by Policy Number" << endl;
            cout << "2) Find Policy by Vehicle Registration Number" << endl;
            cout << "3) Find Policy by Engine Number" << endl;
            cout << "4) Find Policy by Chassis Number" << endl;
            cout << "5) Go back" << endl;

            inputChoice(choice, {"1","2","3","4","5","6"});

            string key;

            if (choice == "1") {
                int policyNo;
                cout << "\nEnter Policy Number: ";
                cin >> policyNo;

                cout << "Policy with policy number " << policyNo;
                if (findPolicyNo(policyNo)) {
                    cout << " exists." << endl;
                    return policyNo;
                } else {
                    cout << " does not exist." << endl;
                    return false;
                }
            } else if (choice == "2") {
                cout << "\nEnter Vehicle Registration Number (SSDDAANNNN): ";
                cin >> key;
                transform(key.begin(), key.end(), key.begin(), ::toupper);

                int policyNo = findPolicyUsingKey(key, 7);

                cout << "Policy with vehicle registration number " << key;
                if (policyNo) {
                    cout << " exists, and has policy number " << policyNo << endl;
                } else {
                    cout << " does not exist." << endl;
                }
                return policyNo;
            } else if (choice == "3") {
                cout << "\nEnter Engine Number: ";
                cin >> key;
                transform(key.begin(), key.end(), key.begin(), ::toupper);

                int policyNo = findPolicyUsingKey(key, 8);

                cout << "Policy with engine number " << key;
                if (policyNo) {
                    cout << " exists, and has policy number " << policyNo << endl;
                } else {
                    cout << " does not exist." << endl;
                }
                return policyNo;
            } else if (choice == "4") {
                cout << "\nEnter Chassis Number: ";
                cin >> key;
                transform(key.begin(), key.end(), key.begin(), ::toupper);

                int policyNo = findPolicyUsingKey(key, 9);

                cout << "Policy with chassis number " << key;
                if (policyNo) {
                    cout << " exists, and has policy number " << policyNo << endl;
                } else {
                    cout << " does not exist." << endl;
                }
                return policyNo;
            } else {
                return false;
            }
        }

        void deletePolicy() {  // deletes a policy
            int policyNo = findPolicy();

            if (policyNo) {
                fs::remove("./Database/Policies/policy_" + to_string(policyNo) + ".txt");

                fstream ifile("./Database/Policies/all_policies.txt", ios::in);

                string fileContent;

            // Delete the policy number from file "all_policies.txt"
                if (ifile.is_open()) {
                    while (!ifile.eof()) {
                        string temp;
                        ifile >> temp;
                        if (temp != to_string(policyNo)) fileContent += temp + "\n";
                    }
                    ifile.close();
                    fileContent.pop_back();

                    fstream ofile("./Database/Policies/all_policies.txt", ios::out);
                    ofile << fileContent;
                    ofile.close();

                    cout << "Deleted the policy." << endl;
                }
            }
        }

        void viewPolicyDetails() {  // prints policy details
            int policyNo = findPolicy();

            if (policyNo) {
                fstream file("./Database/Policies/policy_" + to_string(policyNo) + ".txt", ios::in);

                string line;
                vector<string> keys = {"Policy Number", "Customer ID", "Customer Name", "Customer Address", "Policy Start Date", "Policy End Date", " Total Premium (with GST)", "Vehicle Registration Number", "Registration Authority", "Insured's Declared Value (IDV)", "Engine Number", "Chassis Number", "Make & Model", "Manufacture Year", "Cubic Capacity (CC)", "Seating Capacity"};
                vector<string> values;

                if (file.is_open()) {
                    values.push_back(getFileLine("policy", policyNo, 1));
                    values.push_back(getFileLine("policy", policyNo, 2));
                    values.push_back(getFileLine("customer", policyNo, 1) + ". " + getFileLine("customer", policyNo, 2) + " " + getFileLine("customer", policyNo, 3) + " " + getFileLine("customer", policyNo, 4));
                    values.push_back(getFileLine("customer", policyNo, 9) + ", " + getFileLine("customer", policyNo, 10) + ", " + getFileLine("customer", policyNo, 11) + ", " + getFileLine("customer", policyNo, 12) + " - " + getFileLine("customer", policyNo, 13) + ", " + getFileLine("customer", policyNo, 14) + ", " + getFileLine("customer", policyNo, 15) + "India");
                    values.push_back(getFileLine("policy", policyNo, 13) + "/" + getFileLine("policy", policyNo, 14) + "/" + getFileLine("policy", policyNo, 15));
                    values.push_back(getFileLine("policy", policyNo, 16) + "/" + getFileLine("policy", policyNo, 17) + "/" + getFileLine("policy", policyNo, 18));
                    values.push_back("Rs. " + formatCurrency(getFileLine("policy", policyNo, 20)));
                    values.push_back(getFileLine("policy", policyNo, 7));
                    values.push_back(getFileLine("policy", policyNo, 12));
                    values.push_back("Rs. " + formatCurrency(getFileLine("policy", policyNo, 6)));
                    values.push_back(getFileLine("policy", policyNo, 8));
                    values.push_back(getFileLine("policy", policyNo, 9));
                    values.push_back(getFileLine("policy", policyNo, 4) + " " + getFileLine("policy", policyNo, 5));
                    values.push_back(getFileLine("policy", policyNo, 3));
                    values.push_back(getFileLine("policy", policyNo, 10));
                    values.push_back(getFileLine("policy", policyNo, 11));

                    printKeysAndValues(keys, values);  // prints details in key-value form
                }
            }
        }

        void viewTotalPremium() {  // prints the sum of premiums of all policies
            int totalPremium = 0;

            if (fs::is_directory("./Database/Policies")) {
                for (const auto &content : fs::directory_iterator("./Database/Policies")) {  // iterate through all files in Database/Policies directory
                    string fileName = content.path().filename().string();
                    if (fileName == "all_policies.txt") continue;  // skip this file

                    fstream file("./Database/Policies/" + fileName, ios::in);

                    if (file.is_open()) {
                        string temp;
                        for (int i = 1; i <= 20; i++) {
                            getline(file, temp);
                            if (i == 20) totalPremium += stoi(temp);
                        }
                    }
                    file.close();
                }
            }

            if (totalPremium) printKeyAndValue("Total Premium", "Rs. " + formatCurrency(to_string(totalPremium)));
            else cout << "\nNo policy exists." << endl;
        }

        void viewTotalPolicies() {  // prints each policy number, its premium, and associated customer ID
            int totalPolicies = 0;

            if (fs::is_directory("./Database/Policies")) {
                for (const auto &content : fs::directory_iterator("./Database/Policies")) {  // iterate through all files in Database/Policies directory
                    string fileName = content.path().filename().string();
                    if (fileName == "all_policies.txt") continue;  // skip this file

                    totalPolicies++;
                    string policyNo, premium, customerId;

                    fstream file("./Database/Policies/" + fileName, ios::in);

                    if (file.is_open()) {
                        string value;
                        for (int i = 1; i <= 20; i++) {
                            getline(file, value);

                            if (i == 1) policyNo = value;
                            else if (i == 2) customerId = value;
                            else if (i == 20) premium = value;
                        }
                    }
                    file.close();
                    printKeyAndValue("Policy Number " + policyNo, "Rs. " + formatCurrency(premium) + " (Customer ID: " + customerId + ")");
                }
            }

            if (!totalPolicies) cout << "\nNo policy exists." << endl;
        }
};

class Ui : public Company, public Policy {
    string titleOption1 = "1) Create Policy";
    string descriptionOption1 = "To create new policy";
    string titleOption2 = "2) Find Policy";
    string descriptionOption2 = "To find policy based on key";
    string titleOption3 = "3) View Policy Details";
    string descriptionOption3 = "To find & print policy";
    string titleOption4 = "4) View Customers";
    string descriptionOption4 = "To list customers & their policies with their premiums";
    string titleOption5 = "5) Edit Customer";
    string descriptionOption5 = "To find & edit details of customer";
    string titleOption6 = "6) View Total Premium";
    string descriptionOption6 = "To get total premium of all policies";
    string titleOption7 = "7) View All Policies";
    string descriptionOption7 = "To list all policies";
    string titleOption8 = "8) Delete Policy";
    string descriptionOption8 = "To find & delete policy";
    string titleOption9 = "C) Clear Console Screen";
    string descriptionOption9 = "To clear text of console screen";
    string titleOption10 = "Q) Quit";
    string descriptionOption10 = "To quit program";

    void printHeader() {  // prints header containing company details
        printInBoxInCenter(vector<string> {
            getCompanyName(),
            getCompanyOfficeAddress(),
            "Contact Number: " + getCompanyContactNo()
        });
    }

    void printMenu() {  // prints menu of options
        int maxWidth = 23;

        cout << "\nChoose an option:\n" << endl;

        cout << left;  // aligns text to the left
        cout.width(maxWidth);
        cout << titleOption1 << " - " << descriptionOption1 << endl;
        cout.width(maxWidth);
        cout << titleOption2 << " - " << descriptionOption2 << endl;
        cout.width(maxWidth);
        cout << titleOption3 << " - " << descriptionOption3 << endl;
        cout.width(maxWidth);
        cout << titleOption4 << " - " << descriptionOption4 << endl;
        cout.width(maxWidth);
        cout << titleOption5 << " - " << descriptionOption5 << endl;
        cout.width(maxWidth);
        cout << titleOption6 << " - " << descriptionOption6 << endl;
        cout.width(maxWidth);
        cout << titleOption7 << " - " << descriptionOption7 << endl;
        cout.width(maxWidth);
        cout << titleOption8 << " - " << descriptionOption8 << endl;
        cout.width(maxWidth);
        cout << titleOption9 << " - " << descriptionOption9 << endl;
        cout.width(maxWidth);
        cout << titleOption10 << " - " << descriptionOption10 << endl;
    }

    public:
        void driveProgram() {  // drives the whole program
            system("cls");  // clears console screen
            printHeader();  // prints company details

            string choice;

            while (true) {
                printMenu();
                inputChoice(choice);

                if (choice == "0") {
                    cout << "Invalid choice! Please try again." << endl;
                    continue;
                }
                if (choice == "1") {
                    printInBoxInCenter(titleOption1 + " - " + descriptionOption1);
                    createPolicy();
                } else if (choice == "2") {
                    printInBoxInCenter(titleOption2 + " - " + descriptionOption2);
                    findPolicy();
                } else if (choice == "3") {
                    printInBoxInCenter(titleOption3 + " - " + descriptionOption3);
                    viewPolicyDetails();
                } else if (choice == "4") {
                    printInBoxInCenter(titleOption4 + " - " + descriptionOption4);
                    viewCustomers();
                } else if (choice == "5") {
                    printInBoxInCenter(titleOption5 + " - " + descriptionOption5);
                    editCustomer();
                } else if (choice == "6") {
                    printInBoxInCenter(titleOption6 + " - " + descriptionOption6);
                    viewTotalPremium();
                } else if (choice == "7") {
                    printInBoxInCenter(titleOption7 + " - " + descriptionOption7);
                    viewTotalPolicies();
                } else if (choice == "8") {
                    printInBoxInCenter(titleOption8 + " - " + descriptionOption8);
                    deletePolicy();
                } else if (choice == "C") {
                    system("cls");
                    printHeader();
                } else if (choice == "Q") {
                    printInBoxInCenter("You Quit the Program");
                    break;
                }
            }
        }
};

int main() {
    Ui ui;
    ui.driveProgram();
}