#include "../../src/include/signals.h"
#include <iostream>
#include <string>

using Callback = void(const std::string&);

class A
{
public:
    void generateSignal() const
    {
        emit signal("Hello, world!");
    }

public signals:
    es::Signal<Callback> signal;
};

class B
{
public slots:
    void onSignal(const std::string& str)
    {
        std::cout << str << " from method" << std::endl;
    }
};

class Mutable
{
public slots:
    void onSignal(const std::string& str) const
    {
        ++mValue;
        std::cout << str << " from Mutable class" << std::endl;
    }

private:
    mutable uint32_t mValue = 0;
};

void onSignal(const std::string& str)
{
    std::cout << str << " from function" << std::endl;
}

int main()
{
    A a;
    B b;

    auto lambda = [](const std::string& str) {
        std::cout << str << " from lambda" <<std::endl;
    };

    // member pointer
    es::Connector::connect(&a.signal, &b, &B::onSignal);

    // function
    es::Connector::connect(&a.signal, &onSignal);

    // lambda
    es::Connector::connect(&a.signal, lambda);

    const A aa;
    const Mutable mm;

    // member pointer
    es::Connector::connect(&aa.signal, &mm, &Mutable::onSignal);

    // function
    es::Connector::connect(&aa.signal, &onSignal);

    // lambda
    es::Connector::connect(&aa.signal, lambda);

    a.generateSignal();
    aa.generateSignal();
    return 0;
}
