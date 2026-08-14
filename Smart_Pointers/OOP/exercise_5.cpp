#include <iostream>
#include <memory>
#include <vector>

class PaymentMethod {
public:
    virtual ~PaymentMethod() = default;
    virtual void pay(double amount) const = 0;
};

class CreditCard : public PaymentMethod {
public:
    void pay(double amount) const override {
        std::cout << "Paid " << amount << " using Credit Card" << std::endl;
    }
};

class PayPal : public PaymentMethod {
public:
    void pay(double amount) const override {
        std::cout << "Paid " << amount << " using PayPal" << std::endl;
    }
};

class BankTransfer : public PaymentMethod {
public:
    void pay(double amount) const override {
        std::cout << "Paid " << amount << " using Bank Transfer" << std::endl;
    }
};

class PaymentProcessor {
private:
    std::unique_ptr<PaymentMethod> paymentMethod;

public:
    PaymentProcessor(std::unique_ptr<PaymentMethod> method) 
        : paymentMethod(std::move(method)) {}

    void Process(double amount) {
        if (paymentMethod) {
            paymentMethod->pay(amount);
        }
    }
};

int main() {
    auto paymentMethod = std::make_unique<CreditCard>();

    PaymentProcessor processor(std::move(paymentMethod));
    processor.Process(100.0);

    return 0;
}