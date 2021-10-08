#include<bits/stdc++.h>
using namespace std;

class convertStringtoArray {
    vector<string> input;
    public:
    convertStringtoArray(string ip) {
        stringstream ss(ip);
        string s;
        char delimiter = ' ';
        while(getline(ss, s, delimiter)) {
            input.push_back(s);
        }
    }
    vector<string> getArray() {
        return input;
    }
};

enum command { SHOW, EXPENSE };
enum type { EQUAL, EXACT, PERCENT };

class prepareInput {
    command cmd;
    string userPaid;
    int amountPaid, amountSplitBetweenNumber;
    vector<string> amountSplitBetweenUserList;
    type expenseType;
    vector<float> expenseShare;
    public:
    prepareInput(vector<string> input) {
        int c = 0;
        cmd = convertToCommand(input[c++]);
        userPaid = input[c++];
        amountPaid = stoi(input[c++]);
        amountSplitBetweenNumber = stoi(input[c++]);
        for(int i= 0;i<amountSplitBetweenNumber;i++)
            amountSplitBetweenUserList.push_back(input[c++]);
        expenseType = convertToType(input[c++]);
        if(expenseType == EQUAL) {
            float amt = (float)amountPaid/(float)amountSplitBetweenNumber;
            float amt1 = (int)(amt * 100 + .5);
            // cout<<amt<<endl;
            for(int i=0;i<amountSplitBetweenNumber-1;i++)
                expenseShare.push_back(float(amt1)/100);
            expenseShare.push_back((float)amountPaid - (float(amt1*2)/100));

            // cout<<(float)amountPaid - (float)((float)(amountSplitBetweenNumber-1)*amt)<<endl;
            cout<<(float)amountPaid - float(amt1*2)/100<<endl;
            // cout<<(float)((float)(amountSplitBetweenNumber-1)*amt)<<endl;
        } else if(expenseType == EXACT) {
            for(int i=0;i<amountSplitBetweenNumber;i++)
                expenseShare.push_back(stoi(input[c++]));
        } else {
            for(int i=0;i<amountSplitBetweenNumber;i++) {
                int amt = (amountPaid * stoi(input[c++]))%100;
                expenseShare.push_back(amt);
            }
        }

    }
    command convertToCommand(string cmd) {
        if(cmd == "SHOW") return SHOW;
        else return EXPENSE;
    } 
    type convertToType(string type) {
        if(type == "EQUAL") return EQUAL;
        else if(type == "EXACT") return EXACT;
        else return PERCENT;
    }

    command getCommand () {
        return cmd;
    }
    string getUserPaid() {
        return userPaid;
    }
    int getAmountPaid() {
        return amountPaid;
    }
    int getAmountSplitBetweenNumber() {
        return amountSplitBetweenNumber;
    }
    vector<string> getAmountSplitBetweenUserList() {
        return amountSplitBetweenUserList;
    }
    type getType() {
        return expenseType;
    }
    vector<float> getExpenseShare () {
        for(int i=0;i<amountSplitBetweenNumber;i++)
        cout<<expenseShare[i]<<" ";
        cout<<endl;
        return expenseShare;
    }

};

class Compute {
    unordered_map<string, unordered_map<string, float>> mp;
    public:
    void displayBlanceSheet() {
        for(auto itr = mp.begin(); itr != mp.end(); itr++) {
            cout<<"\n"<<itr->first<<"\tList of Users who owes to "<<itr->first<<endl;
            for(auto it = itr->second.begin(); it != itr->second.end(); it++) {
                cout<<"\t"<<it->first<<" ----> "<<it->second<<endl;
            }
            cout<<endl;
        }
    }
    void displayBlanceSheet(vector<string> ip) {
        for(auto it = mp[ip[1]].begin(); it != mp[ip[1]].end(); it++) {
                cout<<"\t"<<it->first<<" ----> "<<it->second<<endl;
        }
    }
    void updateBalnceSheet(vector<string> ip) {
        prepareInput *p = new prepareInput(ip);
        int n = p->getAmountSplitBetweenNumber();
        string userPaid = p->getUserPaid();
        vector<string> userList = p->getAmountSplitBetweenUserList();
        vector<float> expense = p->getExpenseShare();
        for(int i=0;i<n;i++) {
            if(userList[i] == userPaid) continue;
            if(mp[userList[i]].find(userPaid) != mp[userList[i]].end()) {
                if(mp[userList[i]][userPaid] > expense[i])
                    mp[userList[i]][userPaid] -= expense[i];
                else {
                    mp[userPaid][userList[i]] += (expense[i] - mp[userList[i]][userPaid]);
                    mp[userList[i]].erase(userPaid);
                }
            } else {
                mp[userPaid][userList[i]] += expense[i];
            }
        }
    }
};

int main() {
    ifstream file;
    file.open("sampleInput.txt");
    if(!file) {
        cout<<"Files doesn't exist";
        exit(1);
    }
    int n;
    file >> n;
    vector< vector<string> > userDetailsList;
    while(n--) {
        string id, name, email, contact;
        file >> id;
        file >> name;
        file >> email;
        file >> contact;
        vector<string> v = {id, name, email, contact};
        userDetailsList.push_back(v);
    }
    string s;
    Compute *c = new Compute();
    while(getline(cin,s)) {
        convertStringtoArray *csta = new convertStringtoArray(s);
        vector<string> ip = csta->getArray();
        if(ip.size() == 1) c->displayBlanceSheet();
        else if(ip.size() == 2) c->displayBlanceSheet(ip);
        else {
            c->updateBalnceSheet(ip);
            cout<<"OK\n";
        }
        // if(s == "SHOW") c->displayBlanceSheet();
        // else {
        //     c->updateBalnceSheet(s);
        //     cout<<"OK\n";
        // }
    }
    return 0;
}

/*
    Write transactions in a file
    Precision upto 2 decimal point - done
    group of users - implementation  
    OOP pillars demonstration - done
    SHOW command for individual user - done
    Handle floating point - done

    Things i'm handling:
    1) file handling
    2) Encapsulation
    3) Abstraction
    4) Stringstream
*/