/*****************************************************************************
 * PyC++ - C++ Python bindings
 * Copyright (C) 2014 MORI Taeyeon <orochimarufan.x3@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <PyC++.h>
#include <PyC++Ex.h>

using namespace Py;

struct Hello : public Class
{
    using Class::Class;

    int number;

    void __init__(const Tuple &args, const Dict &)
    {
        number = Long(args.getItem(0));
    }

    Object get_number(void *)
    {
        return number;
    }

    void set_number(const Object &v, void*)
    {
        number = Long(v);
    }

    void hello()
    {
        print("Hello World!");
    }
};

PC_MEMBERS(Hello)
{
    CMD_MEMBER(number);
    CMD_PROPERTY(property, get_number, set_number);
    CMD_METHOD(hello);
}

void run()
{
    print("hello = Hello(3)");
    CppType<Hello>::ready();
    auto hello = CppObject<Hello>::create(3);

    hello->hello();

    print("hello->number:", hello->number);
    print("[hello.number = 13]");
    hello.setAttr("number", 13);
    print("hello.property:", hello.getAttr("property"));
    print("[hello->number = 44]");
    hello->number = 44;
    print("hello.number:", hello.getAttr("number"));
    print("[hello.property = 99]");
    hello.setAttr("property", 99);
    print("hello->number:", hello->number);

    Dict locals;
    locals.setItem("hello", hello);

    Dict globals;
    globals.setItem("__builtins__", import_module("builtins"));

    Run::String("print('Running Python! :D'); hello.hello()", globals, locals);

    throw SystemExit::create(1);
}

int main(int argc, char **argv)
{
    initialize();

    int ret = 0;

    try {
        run();
    } catch (SystemExit &e) {
        if (e.getAttr("args").getItem(0).isLong())
            ret = Long(e.getAttr("args").getItem(0));
        else
            e.print();
    }

    finalize();
    return ret;
}
