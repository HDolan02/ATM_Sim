#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "main.h"


Status initialize(vector<owner>& Owners);
Status end(const vector<owner> Owners);
void clear(void);

Status createUser(vector<owner>& Owners);
Status createAcct(vector<owner>& Owners);
Status deleteAcct(vector<owner>& Owners);
Status withdrawFromAcct(vector<owner>& Owners);
Status depositToAcct(vector<owner>& Owners);
Status deleteUser(vector<owner>& Owners);
Status checkAcct(vector<owner> &Owners);


// ################ MAIN ###################################################
int main(int argc, const char * argv[]) {
    int decision = -1;
    
    vector<owner> Owners;   // a vector of owner classes that contains each account owner and their information
    if(!initialize(Owners)){
        exit(1);
    }
    
    
    while(decision != 0){
        do{
            cout << "What would you like to do?\nSign Up(1)\nCreate an account(2)\nDelete an account(3)\nWithdraw(4)\nDeposit(5)\nDelete User(6)\nCheck Acct(7)\nExit(0)\n::";
            cin >> decision;
        }while(decision > 7 || decision < 0);
        
        
        switch (decision) {
            case 1:
                createUser(Owners);
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
    
    
    if(!end(Owners)){
        exit(1);
    }
    
    return 0;
}




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


void clear(void){
    cout << "\n\n\n";
}


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


Status createAcct(vector<owner>& Owners){
    string name;
    int pin;
    int acctType;
    int startAmt = -1;
    vector<owner> tempOwners;
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
        index++;
    }
    
    if(currentOwner == tempOwners.end() || tempOwners.size() == 0){
        cout << "No account with this name and pin exist...\n";
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
    }else{
        Owners.at(index).insertAcct(AcctNames[acctType - 1], startAmt);
    }
    
    return SUCCESS;
}


Status deleteAcct(vector<owner>& Owners){
    string name;
    int index = 0;
    int pin;
    vector<owner> tempOwners;
    int acctType;
    
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
    }

    return SUCCESS;
}


Status withdrawFromAcct(vector<owner>& Owners){
    string name;
    int index = 0;
    int pin;
    vector<owner> tempOwners;
    int acctType;
    double curAmmt, withdrawAmt;
    
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


Status depositToAcct(vector<owner>& Owners){
    string name;
    int index = 0;
    int pin;
    vector<owner> tempOwners;
    int acctType;
    double curAmmt, depositAmount;
    
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


Status checkAcct(vector<owner> &Owners){
    string name;
    vector<owner> tempOwners;
    int pin;
    int index = 0;
    int acctType;
    
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
