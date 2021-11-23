#include <iostream>


using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::string;
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::map;
using std::vector;
using std::sort;


//Global string variables for the input file name, output file name, and all account names
const string iFile = "input.txt";
const string oFile = "/Users/hayden/Library/Mobile Documents/com~apple~CloudDocs/Coding/C++ programs/ATM_Sim/ATM/ATM/input.txt";
const string AcctNames[] = {"Checking", "Savings", "Roth-IRA"};

//Status variable which allows for for an easy check on the result of each function call
enum status{FAILURE, SUCCESS};
typedef enum status Status;


//Owner class holds the owners name, pin for login, and all of the users accounts
class owner{
public:
    //Constructors
    owner():name("Unknown"), pin(-1111){}
    owner(string iName):name(iName), pin(-1111){}
    owner(int iPin):name("Unknown"), pin(iPin){}
    owner(string iName, int iPin):name(iName), pin(iPin){}
    owner(string iName, int iPin, map<string, double> iAccounts):name(iName), pin(iPin), accounts(iAccounts){}
    
    //Accessors
    string getName(void)const{return name;}
    int getPin(void)const{return pin;}
    map<string, double> getAccounts(void)const{return accounts;}
    double getAcctAmt(string name){return accounts[name];}
    bool findAcct(string type){
        if(accounts.find(type) == accounts.end()){
            return false;
        }
        return true;
    }
    bool isEmpty(){return accounts.empty();}
    
    //Modifiers
    void withdrawAmt(string name, double amt){accounts[name] -= amt;}
    void depositAmt(string name, double amt){accounts[name] += amt;}
    void setName(string iName){name = iName;}
    void insertAcct(string type, double ammt){
        accounts[type] = ammt;
    };
    void removeAcct(string type){
        accounts.erase(type);
    }
    
    //Other Member Functions
    void displayAccts(void){
        for(auto eachItem : accounts){
            cout << eachItem.first << ": " << eachItem.second << endl;
        }
    }

    //Operator overload
    friend bool operator < (const owner &left, const owner &right);
    friend bool operator == (const owner &left, const owner &right);
    
    
private:
    string name;
    int pin;
    map<string, double> accounts;
};


//Operator takes two owner objects
//The operator evaluates if the names are currently in alphabetical order
//Returns the result of the evaluation as a bool
bool operator < (const owner &left, const owner &right){
    return left.name < right.name;
}

//Operator takes two owner objects
//The operator checks if the two pins are the same
//Returns the result of the evaluation as a bool
bool operator == (const owner &left, const owner &right){
    return left.pin == right.pin;
}


//Check what accounts they have first and only display those options instead of canceling if they pick the wrong thing
//overload the << operator and fix all functions
//after deleting account, if the user has no more accounts, delete the user?
//Create user interface
