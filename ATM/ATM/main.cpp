#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "main.h"

//Global functions used by the system
Status initialize(vector<owner>& Owners);
Status end(const vector<owner> Owners);
void clear(void);

//Modifier functions which help the program control and modify the Vector of Account Owners
Status createUser(vector<owner>& Owners);
Status createAcct(vector<owner>& Owners);
Status deleteAcct(vector<owner>& Owners);
Status withdrawFromAcct(vector<owner>& Owners);
Status depositToAcct(vector<owner>& Owners);
Status deleteUser(vector<owner>& Owners);
Status checkAcct(vector<owner> &Owners);
Status checkPin_User(vector<owner>& Owners, int& index);


// ################ MAIN ###################################################
int main(int argc, const char * argv[]) {
    int decision = -1;
    
    vector<owner> Owners;   // a vector of owner classes that contains each account owner and their information
    if(!initialize(Owners)){
        exit(1);
    }
    
    
    //Follow commands given by the user until the program is asked to turn off
    while(decision != 0){
        do{
            cout << "What would you like to do?\nSign Up(1)\nCreate an account(2)\nDelete an account(3)\nWithdraw(4)\nDeposit(5)\nDelete User(6)\nCheck Acct(7)\nExit(0)\n::";
            cin >> decision;
        }while(decision > 7 || decision < 0);
        
        
        switch (decision) {
            case 1:
                createUser(Owners);
                sort(Owners.begin(), Owners.end(), lessThan);
                clear();
                break;
            case 2:
                createAcct(Owners);
                clear();
                break;
            case 3:
                deleteAcct(Owners);
                clear();
                break;
            case 4:
                withdrawFromAcct(Owners);
                clear();
                break;
            case 5:
                depositToAcct(Owners);
                clear();
                break;
            case 6:
                deleteUser(Owners);
                clear();
                break;
            case 7:
                checkAcct(Owners);
                clear();
            default:
                break;
        }
        cout << "\n\n";
    }
    
    
    //When the program ends, output all information to the output file
    if(!end(Owners)){
        exit(1);
    }
    
    return 0;
}



//Function takes the unititalized vector of Owners
//The function reads each user and their corresponding information from the input file and stores the information in the Owners Vector
//The function returns a Status to represent a successfull initialization or a failed initialization
Status initialize(vector<owner>& Owners){
    int numOwners;
    string firstName;
    string lastName;
    string acctName;
    int tempPin;
    int numAcct;
    double acctAmt;
    
    // Open the file and check that it opened properly
    ifstream inFile;
    inFile.open(iFile);
    if(!inFile.is_open()){
        cout << "Failed to open the file\n";
        return FAILURE;
    }
    
    // Read in the number of account owners and get each owners information
    inFile >> numOwners;
    for(int i = 0; i < numOwners; i++){
        inFile >> firstName >> lastName;
        inFile >> tempPin;
        Owners.push_back(owner(firstName+" "+lastName, tempPin));
        
        inFile >> numAcct;
        for(int j = 0; j < numAcct; j++){
            inFile >> acctName;
            inFile >> acctAmt;
            Owners[i].insertAcct(acctName, acctAmt);
        }
    }
    
    inFile.close();
    
    //As long as the file opened properly, return success
    return SUCCESS;
}


//The function takes the Owners Vector after the user has asked to end the program
//All of the information found in the Owners Vector is printed to the output file in the same format as the input file
//The function returns a Status to represent the success or failure of the ending process
Status end(const vector<owner> Owners){
    ofstream outFile;
    outFile.open(oFile, ofstream::out);
    if(!outFile.is_open()){
        cout << "Failed to open output file\n";
        return FAILURE;
    }
    
    outFile << Owners.size() << endl;

    for(auto eachItem : Owners){
        map<string, double> tempAccts = eachItem.getAccounts();
        outFile << eachItem.getName() << endl << eachItem.getPin() << endl;
        outFile << tempAccts.size() << endl;
        for(auto eachAcct : tempAccts){
            outFile << eachAcct.first << endl << eachAcct.second << endl;
        }
    }
    
    outFile.close();
    return SUCCESS;
}


//This function prints a given number of newlines to the screen to clear/space out the information on the screen
void clear(void){
    cout << "\n\n\n";
}


//This function takes the Owners Vector
//The function prompts the user for their name and the pin that they would like to use in the future for login and stores the infromation in the Owners Vector
//The function returns a Status which represents the sucess or failure of the insert
Status createUser(vector<owner>& Owners){
    string name;
    int pin;
    int acctType;
    double startAmt = -1;

    cin.ignore();
    cout << "Please enter your full name: ";
    getline(cin, name);
    cout << "Please enter the pin that you would like to use: ";
    cin >> pin;
    cin.ignore();
    
    owner tmpOwner(name, pin);
    Owners.push_back(owner(name, pin));
    
    do{
        cout << "What account would you like?\n"<< AcctNames[0] << "(1)\n"<< AcctNames[1] <<"(2)\n"<< AcctNames[2]<< "(3)\nCancel(0)\n:: ";
        cin >> acctType;
        cin.ignore();
    }while(acctType > 3 || acctType < 0);
    
    
    while(startAmt < 0 && acctType != 0){
        cout << "How much money would you like to open the account with: ";
        cin >> startAmt;
        cin.ignore();
    }
    
    switch (acctType) {
        case 1:
        case 2:
        case 3:
            Owners.at(Owners.size()-1).insertAcct(AcctNames[acctType - 1], startAmt);
            break;
            
        default:
            Owners.pop_back();
            return FAILURE;
            break;
    }
    
    return SUCCESS;
}

//This function takes the Owners Vector
//The Function has the user login and then asks what account they would like to create
//The function returns a status that represents a successful/failed login and creation
Status createAcct(vector<owner>& Owners){
    int acctType;
    int startAmt = -1;
    int index = 0;

    if(!checkPin_User(Owners, index)){
        return FAILURE;
    }
    
    do{
        cout << "What account would you like?\n" << AcctNames[0] << "(1)\n"<< AcctNames[1] <<"(2)\n"<< AcctNames[2]<< "(3)\nCancel(0)\n:: ";;
        cin >> acctType;
        cin.ignore();
    }while(acctType > 3 || acctType < 0);
    
    while(startAmt < 0 && acctType != 0){
        cout << "How much money would you like to open the account with: ";
        cin >> startAmt;
        cin.ignore();
    }
    
    if(Owners.at(index).findAcct(AcctNames[acctType - 1])){
        cout << "You already have a "<< AcctNames[acctType - 1] <<" account with us...";
        return FAILURE;
    }else{
        Owners.at(index).insertAcct(AcctNames[acctType - 1], startAmt);
    }
    
    return SUCCESS;
}

//This function takes the Owners Vector
//The Function has the user login and then asks what account they would like to delete
//The Function returns a Status that represents the success/failure of login and deletion
Status deleteAcct(vector<owner>& Owners){
    int index = 0;
    int acctType;
    
    if(!checkPin_User(Owners, index)){
        return FAILURE;
    }
    
    do{
        cout << "What account would you like to delete?\n" << AcctNames[0] << "(1)\n"<< AcctNames[1] <<"(2)\n"<< AcctNames[2]<< "(3)\nCancel(0)\n:: ";;
        cin >> acctType;
        cin.ignore();
    }while(acctType > 3 || acctType < 0);
    
    

    if(Owners.at(index).findAcct(AcctNames[acctType - 1])){
        cout << "Removed " << AcctNames[acctType - 1] << " accout with: $" << (Owners.at(index).getAccounts())[AcctNames[acctType - 1]];
        Owners.at(index).removeAcct(AcctNames[acctType - 1]);
    }else{
        cout << "You do not have this account\n";
        return FAILURE;
    }

    return SUCCESS;
}


//This function takes the Owners Vector
//The Function has the user login and then asks what account they would like to withdraw from
//The Function returns a Status that represents the success/failure of login and withdrawl
Status withdrawFromAcct(vector<owner>& Owners){
    int index = 0;
    int acctType;
    double curAmmt, withdrawAmt;

    if(!checkPin_User(Owners, index)){
        return FAILURE;
    }
    
    do{
        cout << "What account would you like to withdraw from?\n" << AcctNames[0] << "(1)\n"<< AcctNames[1] <<"(2)\n"<< AcctNames[2]<< "(3)\nCancel(0)\n:: ";
        cin >> acctType;
        cin.ignore();
    }while(acctType > 3 || acctType < 0);
    

    if(Owners.at(index).findAcct(AcctNames[acctType - 1])){
        curAmmt = Owners.at(index).getAcctAmt(AcctNames[acctType - 1]);
        do{
            cout << "You have $" << curAmmt << ". How much would you like to withdraw?: ";
            cin >> withdrawAmt;
            cin.ignore();
        }while(withdrawAmt > curAmmt || withdrawAmt < 0);
        
        Owners.at(index).withdrawAmt(AcctNames[acctType - 1], withdrawAmt);
        
        cout << "You have $" << Owners.at(index).getAcctAmt(AcctNames[acctType - 1]) << " remaining.";
        
    }else{
        cout << "You do not have a " << AcctNames[acctType - 1] << " account\n";
        return FAILURE;
    }
    
    return SUCCESS;
}


//This function takes the Owners Vector
//The Function has the user login and then asks what account they would like to deposit into
//The Function returns a Status that represents the success/failure of login and deposit
Status depositToAcct(vector<owner>& Owners){
    int index = 0;
    int acctType;
    double curAmmt, depositAmount;
    
    if(!checkPin_User(Owners, index)){
        return FAILURE;
    }
    
    do{
        cout << "What account would you like to deposit into?\n" << AcctNames[0] << "(1)\n"<< AcctNames[1] <<"(2)\n"<< AcctNames[2]<< "(3)\nCancel(0)\n:: ";
        cin >> acctType;
        cin.ignore();
    }while(acctType > 3 || acctType < 0);
    
    
    if(Owners.at(index).findAcct(AcctNames[acctType - 1])){
        curAmmt = Owners.at(index).getAcctAmt(AcctNames[acctType - 1]);
        do{
            cout << "You currently have $" << curAmmt << ". How much would you like to deposit?: ";
            cin >> depositAmount;
            cin.ignore();
        }while(depositAmount < 0);
        
        Owners.at(index).depositAmt(AcctNames[acctType - 1], depositAmount);
        
        cout << "You now have $" << Owners.at(index).getAcctAmt(AcctNames[acctType - 1]) << " in your " << AcctNames[acctType - 1] << " account.\n";
        
    }else{
        cout << "You do not have a " << AcctNames[acctType - 1] << " account\n";
        return FAILURE;
    }
    
    return SUCCESS;
}


//This function takes the Owners Vector
//The Function has the user login and then checks that the user actually wants to delete there entire account
//The Function returns a Status that represents the success/failure of login and deletion
Status deleteUser(vector<owner>& Owners){
    string name;
    vector<owner> tempOwners;
    int pin;
    int stillDel;
    int index = 0;
    
    cin.ignore();
    cout << "What is your name on record: ";
    getline(cin, name);
    

    for (auto eachOwner : Owners){
        if(eachOwner.getName() == name){
            tempOwners.push_back(eachOwner);
        }
    }
    
    cout << "What is your pin: ";
    cin >> pin;
    cin.ignore();
    
    vector<owner>::iterator currentOwner;
    currentOwner = tempOwners.begin();
    
    if(tempOwners.size() != 0){
        while( currentOwner != tempOwners.end() && currentOwner->getPin() != pin){
            currentOwner++;
            index++;
        }
    }
    
    if(currentOwner == tempOwners.end() || tempOwners.size() == 0){
        cout << "No account with this name and pin exist...\n";
        return FAILURE;
    }
    
    cout << "Are you sure you want to delete your account?(1 = yes, 0 = no): ";
    cin >> stillDel;
    cin.ignore();
    
    if(stillDel){
        cout << "\n\nReturning: ";
        for(auto eachAcct : currentOwner->getAccounts()){
            cout << eachAcct.first << ": " << eachAcct.second;
        }
        Owners.erase(Owners.begin() + index);
    }
    
    return SUCCESS;
}


//This function takes the Owners Vector
//The Function has the user login and then asks what account they would like to check
//The Function returns a Status that represents the success/failure of login and checking of the account
Status checkAcct(vector<owner> &Owners){
    int index = 0;
    int acctType;

    if(!checkPin_User(Owners, index)){
        return FAILURE;
    }
    
    do{
        cout << "What account would you like to check?\n" << AcctNames[0] << "(1)\n"<< AcctNames[1] <<"(2)\n"<< AcctNames[2]<< "(3)\nCancel(0)\n:: ";
        cin >> acctType;
        cin.ignore();
    }while(acctType > 3 || acctType < 0);
    
    if(Owners.at(index).findAcct(AcctNames[acctType - 1])){
        cout << "You have $" << Owners.at(index).getAcctAmt(AcctNames[acctType-1]) << " in your " << AcctNames[acctType - 1] << " account.\n";
    }else{
        cout <<"You do not have a " << AcctNames[acctType - 1] << " account\n";
        return FAILURE;
    }
    
    return SUCCESS;
}

//This function takes the Owners Vector and an index passed by reference
//The function asks for the users name and login pin and checks that they are correct
//At the end of the function, the index should be the index in the Owners Vector of that user and the function will return a Status that represents the success/failure of login
Status checkPin_User(vector<owner>& Owners, int& index){
    string name;
    int pin;
    vector<owner> tempOwners;
    
    cin.ignore();
    cout << "What is your name on record: ";
    getline(cin, name);
    

    for (auto eachOwner : Owners){
        if(eachOwner.getName() == name){
            tempOwners.push_back(eachOwner);
        }else{
            if(tempOwners.empty()){
                index++;
            }
        }
    }
    
    cout << "What is your pin: ";
    cin >> pin;
    cin.ignore();
    
    vector<owner>::iterator currentOwner;
    currentOwner = tempOwners.begin();
    
    if(tempOwners.size() != 0){
        while( currentOwner != tempOwners.end() && currentOwner->getPin() != pin){
            currentOwner++;
            index++;
        }
    }
    
    if(currentOwner == tempOwners.end() || tempOwners.size() == 0){
        cout << "No account with this name and pin exist...\n";
        return FAILURE;
    }
    
    return SUCCESS;
}
