#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

class VendingMachine;

class State
{
public:
    virtual void insertMoney(VendingMachine *machine, int amount) = 0;
    virtual void selectProduct(VendingMachine *machine, const string &product) = 0;
    virtual void dispenseProduct(VendingMachine *machine) = 0;
    virtual void cancelTransaction(VendingMachine *machine) = 0;
};

// VendingMachine class
class VendingMachine
{
private:
    State *currentState;
    unordered_map<string, pair<int, int>> inventory; // <product, <quantity, price>>
    int currentBalance;
    string selectedProduct;

    State *idleState;
    State *hasMoneyState;
    State *dispenseProductState;
    State *outOfStockState;

public:
    VendingMachine();

    void setState(State *state)
    {
        currentState = state;
    }

    void insertMoney(int amount)
    {
        currentState->insertMoney(this, amount);
    }

    void selectProduct(const string &product)
    {
        currentState->selectProduct(this, product);
    }

    void dispenseProduct()
    {
        currentState->dispenseProduct(this);
    }

    void cancelTransaction()
    {
        currentState->cancelTransaction(this);
    }

    void setBalance(int amount)
    {
        currentBalance = amount;
    }

    int getBalance() const
    {
        return currentBalance;
    }

    int getProductPrice(const string &product) const
    {
        auto it = inventory.find(product);
        if (it != inventory.end())
        {
            return it->second.second;
        }
        return 0;
    }

    void setSelectedProduct(const string &product)
    {
        selectedProduct = product;
    }

    const string &getSelectedProduct() const
    {
        return selectedProduct;
    }

    void updateInventory(const string &product, int count, int price)
    {
        inventory[product] = {count, price};
    }

    int getInventory(const string &product) const
    {
        auto it = inventory.find(product);
        if (it != inventory.end())
        {
            return it->second.first;
        }
        return 0;
    }

    void reduceInventory(const string &product)
    {
        if (inventory[product].first > 0)
        {
            inventory[product].first--;
        }
    }

    void displayInventory() const
    {
        cout << "Inventory:\n";
        for (const auto &item : inventory)
        {
            cout << "Product: " << item.first << ", Quantity: " << item.second.first << ", Price: " << item.second.second << "\n";
        }
    }

    State *getIdleState() const
    {
        return idleState;
    }

    State *getHasMoneyState() const
    {
        return hasMoneyState;
    }

    State *getDispenseProductState() const
    {
        return dispenseProductState;
    }

    State *getOutOfStockState() const
    {
        return outOfStockState;
    }

    ~VendingMachine();
};

// IdleState class
class IdleState : public State
{
public:
    void insertMoney(VendingMachine *machine, int amount) override;
    void selectProduct(VendingMachine *machine, const string &product) override;
    void dispenseProduct(VendingMachine *machine) override
    {
        cout << "No money inserted yet.\n";
    }
    void cancelTransaction(VendingMachine *machine) override
    {
        cout << "No transaction to cancel.\n";
    }
};

// HasMoneyState class
class HasMoneyState : public State
{
public:
    void insertMoney(VendingMachine *machine, int amount) override
    {
        machine->setBalance(machine->getBalance() + amount);
        cout << "Additional money inserted. Current balance: " << machine->getBalance() << "\n";
    }
    void selectProduct(VendingMachine *machine, const string &product) override;
    void dispenseProduct(VendingMachine *machine) override
    {
        cout << "Select a product first.\n";
    }
    void cancelTransaction(VendingMachine *machine) override;
};

// DispenseProductState class
class DispenseProductState : public State
{
public:
    void insertMoney(VendingMachine *machine, int amount) override
    {
        cout << "Wait! Dispensing the product.\n";
    }
    void selectProduct(VendingMachine *machine, const string &product) override
    {
        cout << "Already selected a product. Wait for it to be dispensed.\n";
    }
    void dispenseProduct(VendingMachine *machine) override;
    void cancelTransaction(VendingMachine *machine) override
    {
        cout << "Cannot cancel. Product is being dispensed.\n";
    }
};

// OutOfStockState class
class OutOfStockState : public State
{
public:
    void insertMoney(VendingMachine *machine, int amount) override
    {
        cout << "Cannot accept money. The machine is out of stock.\n";
    }
    void selectProduct(VendingMachine *machine, const string &product) override
    {
        cout << "Cannot select product. The machine is out of stock.\n";
    }
    void dispenseProduct(VendingMachine *machine) override
    {
        cout << "Cannot dispense product. The machine is out of stock.\n";
    }
    void cancelTransaction(VendingMachine *machine) override
    {
        cout << "No transaction to cancel.\n";
    }
};

// Implementation of VendingMachine constructor and destructor
VendingMachine::VendingMachine()
    : currentBalance(0), selectedProduct("")
{
    idleState = new IdleState();
    hasMoneyState = new HasMoneyState();
    dispenseProductState = new DispenseProductState();
    outOfStockState = new OutOfStockState();
    currentState = idleState;
}

VendingMachine::~VendingMachine()
{
    delete idleState;
    delete hasMoneyState;
    delete dispenseProductState;
    delete outOfStockState;
}

// Implementation of State methods for IdleState
void IdleState::insertMoney(VendingMachine *machine, int amount)
{
    machine->setBalance(amount);
    cout << "Money inserted. Current balance: " << machine->getBalance() << "\n";
    machine->setState(machine->getHasMoneyState());
}

void IdleState::selectProduct(VendingMachine *machine, const string &product)
{
    cout << "Insert money first.\n";
}

// Implementation of State methods for HasMoneyState
void HasMoneyState::selectProduct(VendingMachine *machine, const string &product)
{
    if (machine->getInventory(product) > 0)
    {
        machine->setSelectedProduct(product);
        cout << "Product " << product << " selected.\n";
        machine->setState(machine->getDispenseProductState());
    }
    else
    {
        cout << "Product " << product << " is out of stock.\n";
        machine->setState(machine->getOutOfStockState());
    }
}

void HasMoneyState::cancelTransaction(VendingMachine *machine)
{
    cout << "Transaction cancelled. Returning money: " << machine->getBalance() << "\n";
    machine->setBalance(0);
    machine->setState(machine->getIdleState());
}

// Implementation of State methods for DispenseProductState
void DispenseProductState::dispenseProduct(VendingMachine *machine)
{
    string product = machine->getSelectedProduct();
    if (machine->getBalance() >= machine->getProductPrice(product))
    {
        machine->reduceInventory(product);
        machine->setBalance(machine->getBalance() - 10);
        cout << "Dispensing product: " << product << "\n";
        cout << "Returning balance: " << machine->getBalance() << "\n";
        machine->setState(machine->getIdleState());
    }
    else
    {
        cout << "Insufficient balance to dispense product.\n";
        machine->setState(machine->getHasMoneyState());
    }
}

int main()
{
    VendingMachine vendingMachine;
    vendingMachine.updateInventory("Soda Can", 10, 30);
    vendingMachine.updateInventory("Lays Chips", 5, 10);
    vendingMachine.updateInventory("Dairy Milk", 2, 10);

    vendingMachine.displayInventory();

    vendingMachine.insertMoney(20);
    vendingMachine.selectProduct("Lays Chips");
    vendingMachine.dispenseProduct();

    vendingMachine.displayInventory();
    vendingMachine.insertMoney(10);
    vendingMachine.selectProduct("Soda Can");
    vendingMachine.dispenseProduct();

    return 0;
}
