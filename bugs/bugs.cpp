#include<iostream>
#include<string>
#include"mineutilshpp/__stdutils__.h"

using std::cout;
using std::endl;
using std::string;
using namespace mineutils;

namespace bug001
    /*
        -当func1是模板函数时：
            当func1的输入是自定义类型M的对象时，能找到func2；
            当func1的输入是内置类型int时(或string甚至ncnn::Mat)，找不到func2。
        -当func1的输入类型是自定义类型M时：
            当func1是模板函数时，能找到func2；
            当func1是普通函数时，找不到func2。
    */
{
    class M {};

    template<class T>
    void func1(T a)
    {
        func2(a);
    }
    //void func1(M a)
    //{
    //    func2(a);
    //}
    void func2(M a)
    {
        cout << "func2(M)" << endl;
    }
    void func2(int a)
    {
        cout << "func2(int)" << endl;
    }

    void main()
    {
        M m;
        func1(m);
        //func1(0);
    }
}

namespace bug002
    /*
        -在t_print递归过程中：
            如果Idx不改变，i每次递归减1，递归没有问题；
            如果Idx每次递归加1，i不改变，递归陷入死循环；
            如果Idx每次递归加1，i每次递归减1，递归也会陷入死循环。
    */
{
    template<int Idx = 0, class... Ts>
    void t_print(int i)
    {
        if (Idx <= i)
        {
            cout << i << endl;
            t_print<Idx, Ts...>(i - 1);   //正常
            //t_print<Idx + 1, Ts...>(i);   //死循环：递归上下文太复杂
            //t_print<Idx + 1, Ts...>(i - 1);   //死循环：递归上下文太复杂
        }
        else
        {
            cout << "]";
            return;
        }
    }

    void main()
    {
        t_print(5);
    }
}

namespace bug003
{
    class Base
    {
    public:
        void fun1()
        {
            print("base1!");
        }

        virtual void fun2()
        {
            print("base2!");
        }
    };

    class Top :public Base
    {
    public:
        void fun1()
        {
            print("top1!");
        }

        virtual void fun2()
        {
            print("top2!");
        }
    };

    void main()
    {
        Top top;
        Base base1 = top;
        Base& base2 = top;
        base1.fun1();   //base1!
        base1.fun2();   //base2!
        base2.fun1();   //base1!
        base2.fun2();   //top2!
    }
}



int main()
{
    bug003::main();
    return 0;
}

