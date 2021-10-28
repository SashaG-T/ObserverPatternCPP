#include <iostream>
#include <string>

#include "Event.h"

/// typedefs for readability
struct UserRegistrar {
    typedef Event<const std::string&, const unsigned&, const unsigned&, const unsigned&> NewUserEvent;
    static NewUserEvent newUserEvent;
    /// Call events as you would any callable type
    void registerUser(std::string name, unsigned d, unsigned m, unsigned y) {
        newUserEvent(name, d, m ,y);
    }
};

/// User std::bind for member functions
struct Calandar {
    UserRegistrar::NewUserEvent::ObserverType newUserObserver; /// newUserObserver will store context for unscribing later
    Calandar()
    :   newUserObserver(std::bind(&Calandar::_newUserEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))
    {
        /// Subscribe an observer to an event
        UserRegistrar::newUserEvent += newUserObserver;
    }
    ~Calandar() {
        /// Unsubscribe an observer if you have access to it's context!
        UserRegistrar::newUserEvent -= newUserObserver;
    }
    void _newUserEvent(const std::string& name, const unsigned& d, const unsigned& m, const unsigned& y) {
        std::cout << "Calandar: Added " << d << "/" << m << "/" << y << " as a special cake day for " << name << "!" << std::endl;
    }
};

void _test1Observer(const std::string& name, const unsigned& d, const unsigned& m, const unsigned& y) {
    std::cout << "Webmaster Version 2.0: Howdy, " << name << "! I hope everything is swell!" << std::endl;
}

void _test2Observer(const std::string& name, const unsigned& d, const unsigned& m, const unsigned& y) {
    std::cout << "Sorry, " << name << ". Our calander has been discontinued. No cake for you :(" << std::endl;
}


int main(int argc, char* argv[]) {

    UserRegistrar userRegistrar;
    { ///Calandar scope
        Calandar companyCalandar;

        /// Declare the Observer and register it!
        auto webMasterObserver(userRegistrar.newUserEvent += [](const std::string& name, const unsigned& d, const unsigned& m, const unsigned& y) {
            std::cout << "Webmaster: Welcome " << name << "!" << std::endl;
        });

        Observer<const std::string&, const unsigned&, const unsigned&, const unsigned&> test1Observer(_test1Observer);

        userRegistrar.registerUser("Jhon", 24, 12, 1990);
        userRegistrar.newUserEvent -= webMasterObserver;
        userRegistrar.newUserEvent += test1Observer;
        userRegistrar.registerUser("Krista", 10, 2, 1991);
        userRegistrar.newUserEvent -= test1Observer;
    }
    userRegistrar.newUserEvent += _test2Observer; ///We'd save the returned Observer from the operator+=() here if we wanted to unsubscribe it later.
    userRegistrar.registerUser("Alisha", 1, 2, 2001);

    Event<std::string> e;
    e += [](std::string s){std::cout << s << std::endl;};
    e("Good-bye!");

    return 0;
}

UserRegistrar::NewUserEvent UserRegistrar::newUserEvent;
