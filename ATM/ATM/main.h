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



const string iFile = "input.txt";
const string oFile = "/Users/hayden/Library/Mobile Documents/com~apple~CloudDocs/Coding/C++ programs/ATM_Sim/ATM/ATM/output.txt";
const string AcctNames[] = {"Checking", "Savings", "Roth-IRA"};
enum status{FAILURE, SUCCESS};
typedef enum status Status;



class owner{
public:
    owner():name("Unknown"), pin(-1111){}
    owner(string iName):name(iName), pin(-1111){}
    owner(string iName, int iPin):name(iName), pin(iPin){}
    owner(string iName, int iPin, map<string, double> iAccounts):name(iName), pin(iPin), accounts(iAccounts){}
    
    string getName(void)const{return name;}
    int getPin(void)const{return pin;}
    map<string, double> getAccounts(void)const{return accounts;}
    double getAcctAmt(string name){return accounts[name];}
    void withdrawAmt(string name, double amt){accounts[name] -= amt;}
    void depositAmt(string name, double amt){accounts[name] += amt;}
    
    void setName(string iName){name = iName;}
    
    void insertAcct(string type, double ammt){
        accounts[type] = ammt;
    };
    
    void removeAcct(string type){
        accounts.erase(type);
    }
    
    bool findAcct(string type){
        if(accounts.find(type) == accounts.end()){
            return false;
        }
        return true;
    }
    
    void displayAccts(void){
        for(auto eachItem : accounts){
            cout << eachItem.first << ": " << eachItem.second << endl;
        }
    }

    friend bool lessThan(owner left, owner right);
    
private:
    string name;
    int pin;
    map<string, double> accounts;
};


bool lessThan(owner left, owner right){
    return left.name < right.name;
}

//Make a check pin/user function
//Check what accounts they have first and only display those options instead of canceling if they pick the wrong thing
//overload the << operator and fix all functions
//after deleting account, if the user has no more accounts, delete the user?
