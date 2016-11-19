#include <memory>
#include <limits>
#include <iostream>
#include <chrono>

using namespace std;

class Foo {
public:
    Foo() { std::cout << "Create" << endl; }
    ~Foo() { std::cout << "Destroy" << endl; }
    void ping() {}
};

int main() {
    cout << "main begin" << endl;
    auto instance = new Foo;
    std::shared_ptr<Foo> x { instance };
    std::weak_ptr<Foo> wx { x };
    
    auto before = chrono::high_resolution_clock::now();
    for(int i = 0; i < numeric_limits<short>::max() << 6; i++)
    {
        // variant 1
        //auto lwx = wx.lock();
        //auto& value = *lwx;
        //value.ping();
        
        // variant 2
        wx.lock()->ping();
    }

    auto after = chrono::high_resolution_clock::now();
    
    auto elapsed = after - before;

    cout << "Repeatedly locking smart pointer took "
         << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()
         << " ms" 
         << endl;
         
    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    
    before = chrono::high_resolution_clock::now();
    for(int i = 0; i < numeric_limits<short>::max() << 6; i++)
    {
        auto& value = *instance;
        value.ping();
    }

    after = chrono::high_resolution_clock::now();
    
    elapsed = after - before;

    cout << "Just dereferencing raw pointer took "
         << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()
         << " ms" 
         << endl;
    
    cout << "main end" << endl;
}
