#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <algorithm>

using namespace std;

class SplitStrategy
{
public:
    virtual void split(double amount, const vector<string> &userIds, unordered_map<string, double> &balances) const = 0;
};

class EqualSplit : public SplitStrategy
{
public:
    void split(double amount, const vector<string> &userIds, unordered_map<string, double> &balances) const override
    {
        double splitAmount = amount / userIds.size();
        for (const string &userId : userIds)
        {
            balances[userId] -= splitAmount;
        }
    }
};

class UnequalSplit : public SplitStrategy
{
    vector<double> shares;

public:
    UnequalSplit(const vector<double> &shares) : shares(shares) {}

    void split(double amount, const vector<string> &userIds, unordered_map<string, double> &balances) const override
    {
        for (size_t i = 0; i < userIds.size(); ++i)
        {
            balances[userIds[i]] -= shares[i];
        }
    }
};

class PercentageSplit : public SplitStrategy
{
    vector<double> percentages;

public:
    PercentageSplit(const vector<double> &percentages) : percentages(percentages) {}

    void split(double amount, const vector<string> &userIds, unordered_map<string, double> &balances) const override
    {
        for (size_t i = 0; i < userIds.size(); ++i)
        {
            balances[userIds[i]] -= (amount * percentages[i] / 100);
        }
    }
};

class User
{
    string id;
    string name;

public:
    User(const string &id, const string &name) : id(id), name(name) {}

    string getId() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }
};

// Expense Class
class Expense
{
    double amount;
    string paidBy;
    vector<string> userIds;
    shared_ptr<SplitStrategy> strategy;

public:
    Expense(double amount, const string &paidBy, const vector<string> &userIds, shared_ptr<SplitStrategy> strategy)
        : amount(amount), paidBy(paidBy), userIds(userIds), strategy(strategy) {}

    void apply(unordered_map<string, double> &balances) const
    {
        balances[paidBy] += amount;
        strategy->split(amount, userIds, balances);
    }
};

class ExpenseManager
{
    unordered_map<string, double> balances;
    unordered_map<string, shared_ptr<User>> users;

public:
    const void addUser(const string &id, const string &name)
    {
        users[id] = make_shared<User>(id, name);
    }

    void addExpense(const Expense &expense)
    {
        expense.apply(balances);
    }

    void showBalances() const
    {
        for (const auto &[userId, balance] : balances)
        {
            cout << users.at(userId)->getName() << ": " << balance << endl;
        }
    }

    void showBalanceById(const string &userId) const
    {
        if (balances.find(userId) != balances.end())
        {
            cout << users.at(userId)->getName() << ": " << balances.at(userId) << endl;
        }
        else
        {
            cout << "User ID not found." << endl;
        }
    }
};

int main()
{
    ExpenseManager manager;

    manager.addUser("1", "Alice");
    manager.addUser("2", "Bob");
    manager.addUser("3", "Charlie");

    vector<string> users = {"1", "2", "3"};

    manager.addExpense(Expense(300, "1", users, make_shared<EqualSplit>()));
    cout << "All balances:" << endl;
    manager.showBalances();

    manager.addExpense(Expense(150, "2", users, make_shared<UnequalSplit>(vector<double>{50, 50, 50})));
    cout << "All balances:" << endl;
    manager.showBalances();

    manager.addExpense(Expense(200, "3", users, make_shared<PercentageSplit>(vector<double>{40, 30, 30})));

    cout << "All balances:" << endl;
    manager.showBalances();

    cout << "\nBalance for Bob:" << endl;
    manager.showBalanceById("2");

    return 0;
}
