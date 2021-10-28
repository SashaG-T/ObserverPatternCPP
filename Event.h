#ifndef ASPHEGAMES_EVENT_H_
    #define ASPHEGAMES_EVENT_H_

#include <vector>
#include <utility>
#include <functional>
#include <algorithm>
#include <memory>

template<typename ... ArgTypes>
class Observer {
    public:
        typedef void(ObserverFunc)(ArgTypes ... argTypes);
    private:
        std::shared_ptr<std::function<ObserverFunc>> observerPtr;
    public:
        Observer(std::function<ObserverFunc> observer)
        :   observerPtr(std::make_shared<std::function<ObserverFunc>>(observer))
        {}
        template<typename ... Args>
        void operator()(Args&& ... args) {
            observerPtr.get()->operator()(std::forward<Args>(args)...);
        }
        bool operator==(const Observer& other) {
            return observerPtr == other.observerPtr;
        }
};

template<typename ... ArgTypes>
class Event {
    public:
        typedef Observer<ArgTypes...> ObserverType;
        ///Doesn't check if delegates vector contains delegateFunc since std::function is not comparable.
        ObserverType& operator+=(typename ObserverType::ObserverFunc*&& observerFunc) {
            observers.push_back(std::function<typename ObserverType::ObserverFunc>(std::forward<decltype(observerFunc)>(observerFunc)));
            return observers.back();
        }
        void operator+=(ObserverType& observer) {
            auto iter(std::find(observers.begin(), observers.end(), observer));
            if(iter == observers.end()) {
                observers.push_back(std::forward<decltype(observer)>(observer));
            }
        }
        ///There is no operator-=(typename DelegateType::DelegateFunc*&& delegateFunc) since std::function is not comparable.
        ///Any possible solution?
        void operator-=(ObserverType& observer) {
            auto iter(std::find(observers.begin(), observers.end(), observer));
            if(iter != observers.end()) {
                observers.erase(iter);
            }
        }
        template<typename ... Args>
        void operator()(Args&& ... args) {
            for(ObserverType& observer : observers) {
                observer(std::forward<Args>(args)...);
            }
        }
    private:
        std::vector<ObserverType> observers;
};

#endif // ASPHEGAMES_EVENT_H_

