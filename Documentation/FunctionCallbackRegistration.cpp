// From: https://godbolt.org/z/9GoEWMnMb
// as part of discussion: https://stackoverflow.com/questions/73744246/unique-id-for-any-kind-of-callable-object-in-c17

#include <any>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <cassert>
#include <iostream>
#include <unordered_set>

void bar(){}
void foo() { volatile int a = 0; std::cout << "In Foo!"<<std::endl; }


class Function
{
public:
    Function(std::string name)
     : m_name(name)
    {}

    virtual ~Function(){}

    std::string name() {
        return m_name;
    }
    const std::string name() const { return m_name; }

    virtual std::uintptr_t id() const = 0;
    
    template <typename... Args>
    decltype(auto) operator()(Args&&... args)
    {
         // delegate to invoke, implementation not relevant for question
    }
private:
    std::string m_name;
    virtual std::any invoke(std::initializer_list<std::any> const& args) const = 0;
};

bool operator==(const Function& lhs, const Function& rhs) {
    return lhs.id() == rhs.id();
}

template<> struct std::hash<Function> {
    std::size_t operator()(Function const& s) const noexcept
    {
        return std::hash<std::uintptr_t>{}((std::uintptr_t)s.id());
    }
};

namespace details {
    template <typename>
    constexpr bool is_std_function_v = false;

    template <typename F>
    constexpr bool is_std_function_v<std::function<F>> = true;

    template <typename Class, typename Member>
    size_t hash_member_ptr(Member Class::* memberPtr)
    {
        char buf[sizeof memberPtr];
        memcpy(&buf, &memberPtr, sizeof memberPtr);
        return std::hash<std::string_view>{}(std::string_view(buf, sizeof buf));
    }
}

template <typename F>
class FunctionImpl : public Function
{
public:
    FunctionImpl(F const& f, std::string name)
     : Function(name)
     , function(f)
    {
        std::cout<<name<< " address is "<<(void*)&f<< " id is "<<id()<<std::endl;

    }

    std::uintptr_t id() const override {
        return reinterpret_cast<std::uintptr_t&>(const_cast<F&>(function));
    }

private:
    std::any invoke(std::initializer_list<std::any> const& args) const override
    {
        // implementation not relevant for question
        return std::any();
    }

    const F& function;
};

template <typename F>
class FunctionImpl<std::function<F>> : public Function
{
    typedef std::function<F> Func;
public:
    FunctionImpl(Func f, std::string name)
     : Function(name)
     , function(f)
    {
        std::cout<<name<< " address is "<<(void*)&f<< " id is "<<id()<<std::endl;
        auto g = function.template target<std::add_pointer_t<F>>();
        (*g)();
        std::cout<<(std::uintptr_t)*g<< " and "<<(std::uintptr_t)foo<< " =? " << (*g == foo) << std::endl;
    }

    std::uintptr_t id() const override {
        return (std::uintptr_t)*function.template target<std::add_pointer_t<F>>();
    }

private:
    std::any invoke(std::initializer_list<std::any> const& args) const override
    {
        // implementation not relevant for question
        return std::any();
    }

    Func function;
};




using FPointer = std::unique_ptr<Function>;

template <typename F>
std::pair<const std::uintptr_t, FPointer> make_function(F const& f, std::string name)
{
    return {reinterpret_cast<std::uintptr_t&>(const_cast<F&>(f)), std::make_unique<FunctionImpl<F>>(f, name)};
}
template <typename F>
std::pair<const std::uintptr_t, FPointer> make_function(std::function<F> const& f, std::string name)
{
    auto g = f.template target<std::add_pointer_t<F>>();
    return {(std::uintptr_t)*g, std::make_unique<FunctionImpl<std::function<F>>>(f, name)};
}


using FContainer = std::unordered_map<std::uintptr_t, FPointer>;

template <typename F>
bool contains(FContainer const& v, F && f)
{
    std::cout<<"Contains looking for "<<reinterpret_cast<std::uintptr_t&>(f) << " "<<(std::uintptr_t)foo<<std::endl;
    return v.find(reinterpret_cast<std::uintptr_t&>(f)) != v.end();
}
template <typename F>
bool contains(FContainer const& v, std::function<F> && f)
{
    auto g = f.template target<std::add_pointer_t<F>>();
    std::cout<<"Contains looking for "<<(std::uintptr_t)*g << " "<<(std::uintptr_t)foo<<std::endl;
    return v.find((std::uintptr_t)*g) != v.end();
}



struct AClass {
    void MemberFunction1(){}
    void MemberFunction2(){}
};

struct ACallableClass
{
    void operator()(){}
};

int main()
{
    FContainer v;

    // function pointer
    v.insert( 
        make_function(
            &foo,
            "foo"
        )
    );
/**/
    // std::function
    v.insert( 
        make_function(
            std::function<void()>(&foo),
            "func foo"
        )
    );
/**/
    // member function
 
    v.insert( 
        make_function(
            &AClass::MemberFunction1,
            "AClass::MemberFunction1"
        )
    );
/**/
    // callable
    v.insert(
        make_function(
            ACallableClass(),
            "CallableClass"
        )
    );

    // lambda
    v.insert( 
        make_function(
            [](){},
            "empty lambda"
        )
    );

    for( auto& i : v){
        std::cout<<i.second->name()<<" = "<<i.first<<"/"<<i.second->id()<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout<< "Now " <<std::endl;// << (std::uintptr_t)&xfoo << std::endl;

    assert(contains(v, &foo));
    assert(contains(v, std::function<void()>(&foo)));
    assert(contains(v, &AClass::MemberFunction1));
//  assert(!contains(v, [](){})); // every lambda is different

    assert(!contains(v, &bar));
    assert(!contains(v, std::function<void()>(&bar)));
 //   assert(!contains(v, &AClass::MemberFunction2));

    return 0;
}