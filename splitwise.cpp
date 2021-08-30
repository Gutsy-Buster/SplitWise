#include<bits/stdc++.h>
using namespace std;

class user {
    string user_id, name, email, contact;
    public:
    user(string user_id, string name, string email, string contact) {
        this->user_id = user_id;
        this->name = name;
        this->email = email;
        this->contact = contact;
    }
    string getUserId() { return user_id; }
    string getUserName() { return name; }
    string getUserEmail() { return email; }
    string getUserContact() { return contact; }
};

enum expenseType { EQUAL, PERCENT, EXACT };

enum command { SHOW, EXPENSE };

class SplitInputToArray {
    vector<string> input;
    public:
    SplitInputToArray(string str) {
        stringstream ss(str);
        const char delim = ' ';
        string s;
        while(getline(ss,s,delim)) {
            input.push_back(s);
        }
    }
    vector<string> getInputArray() {
        return input;
    }

};

class prepareInput {
    string personPaidId;
    int amount, numberOfUsers;
    vector<string> users;
    vector<int> expenseShare;
    expenseType type;
    command cmd;
    public:
    prepareInput(string str) {
        SplitInputToArray *s = new SplitInputToArray(str);
        vector<string> input = s->getInputArray();
        int c = 0;
        cmd = convertToCommand(input[c++]);
        if(cmd == SHOW) {

        } else {
            personPaidId = input[c++];
            amount = stoi(input[c++]);
            numberOfUsers = stoi(input[c++]);
            for(int i=0;i<numberOfUsers;i++) {
                users.push_back(input[c++]);
            }
            type = convertToExpenseType(input[c++]);
            if(type == PERCENT) {
                for(int i=0;i<numberOfUsers;i++) {
                    int amt = (amount * stoi(input[c++]))/100;
                    expenseShare.push_back(amt);
                }
            } else if (type == EXACT) {
                for(int i=0;i < numberOfUsers;i++) {
                    expenseShare.push_back(stoi(input[c++]));
                }
            } else {
                int amt = amount/numberOfUsers;
                for(int i=0;i < numberOfUsers;i++) {
                    expenseShare.push_back(amt);
                }
            }
        }
    }
    command convertToCommand(string s) {
        if(s == "SHOW") return SHOW;
        else return EXPENSE;
    }

    expenseType convertToExpenseType(string s) {
        if(s == "EQUAL") return EQUAL;
        else if(s == "EXACT") return EXACT;
        else return PERCENT;
    }

    int getAmountPaid() { return amount; };
    int getNumberOfUsersAmountDistributed() { return numberOfUsers; }
    string getUserIdOfPersonPaid() { return personPaidId; }
    vector<string> getUserListAmountDistributed() { return users; }
    vector<int> getAmountToBePaidList() { return expenseShare; }
    expenseType getExpenseType() { return type; }
    command getCommand() { return cmd; }
};

class splitExpense {
    unordered_map<string, unordered_map<string, int>> mp;
    public:
    splitExpense(string str) {
        prepareInput *p = new prepareInput(str);
        int numberOfUsers = p->getNumberOfUsersAmountDistributed();
        string userIdWhoPaid = p->getUserIdOfPersonPaid();
        vector<string> userList = p->getUserListAmountDistributed();
        vector<int> expenseList = p->getAmountToBePaidList();
        for(int i=0;i<numberOfUsers;i++) {
            if(userList[i] != userIdWhoPaid) {
                if(mp[userList[i]].find(userIdWhoPaid) != mp[userList[i]].end()) {
                    if(mp[userList[i]][userIdWhoPaid] > expenseList[i]) 
                        mp[userList[i]][userIdWhoPaid] = mp[userList[i]][userIdWhoPaid] - expenseList[i];
                    else {
                        mp[userIdWhoPaid][userList[i]] += (expenseList[i] - mp[userList[i]][userIdWhoPaid]);
                        mp[userList[i]].erase(userIdWhoPaid);
                    }
                } else mp[userIdWhoPaid][userList[i]] += expenseList[i];
            }
        }
    }
    unordered_map<string, unordered_map<string, int>> getBalanceSheet() {
        return mp;
    }

};

class displayBalanceSheet {
    public:
    void display(string input) {
        splitExpense *s = new splitExpense(input);
        unordered_map<string, unordered_map<string, int>> mp = s->getBalanceSheet();
        for(auto itr = mp.begin();itr!= mp.end(); itr++) {
            cout<<itr->first<<" ---> ";
            if(itr->second.size() > 0) {
                    cout<<"List of User who owes to "<<itr->first<<endl;
                    for(auto it = itr->second.begin(); it != itr->second.end(); it++) {
                    cout<<"\t"<<it->first<<" = "<<it->second<<endl;
                }
            } else {
                cout<<"No one owes money to "<<itr->first<<endl;
            }
            cout<<endl;
        }
    }
};


int main() {
    int numberOfUSer, cnt = 0;
    string input = "";
    ifstream file;
    file.open("sampleInput.txt");
    if(!file) {
        cout<<"Error ! Files does not exist."<<endl;
        exit(1);
    }
    file >> numberOfUSer;
    vector<vector<string>> users(numberOfUSer);
    while(cnt < numberOfUSer) {
        string id, name, email, contact;
        file >> id;
        file >> name;
        file >> email;
        file >> contact;
        vector<string> user = {id, name, email, contact};
        users[cnt] = user;
        cnt++;
    }
    while(!file.eof()) {
        string s;
        file >> s;
        input += (s +' ');
        // input="";
    }
    displayBalanceSheet *d = new displayBalanceSheet();
    d->display(input);
    file.close();
    // while(true) {
    //     string s;
    //     char c, cs;
    //     cin.clear();
    //     getline(cin, s);
    //     cin.clear();
    //     cout<<"s : "<<s<<endl;
    //     displayBalanceSheet *d = new displayBalanceSheet();
    //     d->display(s);
    //     cout<<"Do you wish to continue (y): ";
    //     cin>>c;
    //     if(c != 'y' || c != 'Y') break;
    // }
    return 0;
}