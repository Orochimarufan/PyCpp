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

#pragma once

#include "PyC++Config.h"

#include <Python.h>

#include <string>

namespace Py {

class Type;
class String;
class Tuple;
class Dict;

void PYCPP_EXPORT auto_throw();

class PYCPP_EXPORT Object
{
private:
    PyObject *mp_object;

protected:
    void set(PyObject *object, bool steal=false);
    void unset();
    void validate();

public:
    // ===== Construct =====
    Object(PyObject *object=Py_None, bool steal=false) :
        mp_object(object)
    {
        if (!steal)
            Py_XINCREF(object);
        validate();
    }

    Object(const Object &other) :
        mp_object(other.mp_object)
    {
        Py_XINCREF(mp_object);
        validate();
    }

    Object(Object &&other) :
        mp_object(other.mp_object)
    {
        other.mp_object = NULL;
        validate();
    }

    // Type constructors
    Object(int val);
    Object(long int val);
    Object(long long int val);
    Object(double val);
    Object(const char* val);
    Object(const std::string &val);

    // ===== Assign =====
    inline Object &operator=(const Object &rhs)
    {
        set(rhs.mp_object);
        return *this;
    }

    inline Object &operator=(PyObject *rhs)
    {
        set(rhs);
        return *this;
    }

    // ===== Destroy =====
    virtual ~Object()
    {
        unset();
    }

    // ===== References =====
    inline void incref()
    {
        Py_XINCREF(mp_object);
    }

    void decref();

    Py_ssize_t  refcnt() const;

    inline PyObject *ptr() const
    {
        return mp_object;
    }

    inline PyObject *steal()
    {
        PyObject *p = mp_object;
        mp_object = NULL;
        return p;
    }

    inline PyObject *newref() const
    {
        Py_XINCREF(mp_object);
        return mp_object;
    }

    // ===== Type safety =====
    // Check If a PyObject fits into this C++ class
    virtual bool valid(PyObject *o) const;

    // ===== Attributes =====
    inline bool hasAttr(const char *attr) const
    {
        return PyObject_HasAttrString(mp_object, attr);
    }

    inline bool hasAttr(const std::string &attr) const
    {
        return PyObject_HasAttrString(mp_object, attr.c_str());
    }

    inline bool hasAttr(const Object &attr) const
    {
        return PyObject_HasAttr(mp_object, attr.mp_object);
    }


    inline Object getAttr(const char *attr) const
    {
        return Object(PyObject_GetAttrString(mp_object, attr), true);
    }

    inline Object getAttr(const std::string &attr) const
    {
        return Object(PyObject_GetAttrString(mp_object, attr.c_str()), true);
    }

    inline Object getAttr(const Object &attr) const
    {
        return Object(PyObject_GetAttr(mp_object, attr.mp_object), true);
    }


    inline void setAttr(const char *attr, const Object &value)
    {
        PyObject_SetAttrString(mp_object, attr, value.mp_object);
        auto_throw();
    }

    inline void setAttr(const std::string &attr, const Object &value)
    {
        PyObject_SetAttrString(mp_object, attr.c_str(), value.mp_object);
        auto_throw();
    }

    inline void setAttr(const Object &attr, const Object &value)
    {
        PyObject_SetAttr(mp_object, attr.mp_object, value.mp_object);
        auto_throw();
    }


    inline void delAttr(const char *attr)
    {
        PyObject_DelAttrString(mp_object, attr);
        auto_throw();
    }

    inline void delAttr(const std::string &attr)
    {
        PyObject_DelAttrString(mp_object, attr.c_str());
        auto_throw();
    }

    inline void delAttr(const Object &attr)
    {
        PyObject_DelAttr(mp_object, attr.mp_object);
    }

    // ===== Callables =====
    Object call();
    Object call(const Tuple &args);
    Object call(const Tuple &args, const Dict &kw);

    template <typename... Args>
    inline Object operator()(Args... targs)
    {
        Tuple args({targs...});
        return call(args);
    }

    // ===== Methods =====
    Object callMethod(const char *method);
    Object callMethod(const char *method, const Tuple &args);
    Object callMethod(const char *method, const Tuple &args, const Dict &kw);

    template <typename... Args>
    inline Object m(const char *method, Args... targs)
    {
        Tuple args({targs...});
        return callMethod(method, args);
    }

    template <typename... Args>
    inline Object m(const std::string &method, Args... targs)
    {
        Tuple args({targs...});
        return callMethod(method.c_str(), args);
    }

    // ===== Identity Check =====
    inline bool is(PyObject *other) const
    {
        return mp_object == other;
    }

    inline bool is(const Object &other) const
    {
        return mp_object == other.mp_object;
    }

    // ===== Protocol Checks =====
    inline bool isCallable() const
    {
        return PyCallable_Check(mp_object);
    }

    inline bool isSequence() const
    {
        return PySequence_Check(mp_object);
    }

    inline bool isMapping() const
    {
        return PyMapping_Check(mp_object);
    }

    inline bool isNumber() const
    {
        return PyNumber_Check(mp_object);
    }

    // ====== Type Checks =====
    inline bool isNull() const
    {
        return mp_object == NULL;
    }

    inline bool isNone() const
    {
        return mp_object == Py_None;
    }

    inline bool isLong() const
    {
        return PyLong_Check(mp_object);
    }

    inline bool isFloat() const
    {
        return PyFloat_Check(mp_object);
    }

    inline bool isType() const
    {
        return PyType_Check(mp_object);
    }

    inline bool isString() const
    {
        return PyUnicode_Check(mp_object);
    }

    inline bool isBytes() const
    {
        return PyBytes_Check(mp_object);
    }

    inline bool isBool() const
    {
        return PyBool_Check(mp_object);
    }

    inline bool isTuple() const
    {
        return PyTuple_Check(mp_object);
    }

    inline bool isList() const
    {
        return PyList_Check(mp_object);
    }

    inline bool isDict() const
    {
        return PyDict_Check(mp_object);
    }

    // ===== Sequence/Mapping protocol =====
    inline Object getItem(const Object &key) const
    {
        return Object(PyObject_GetItem(mp_object, key.mp_object));
    }

    inline void setItem(const Object &key, const Object &value)
    {
        PyObject_SetItem(mp_object, key.mp_object, value.mp_object);
        auto_throw();
    }

    inline void delItem(const Object &key)
    {
        PyObject_DelItem(mp_object, key.mp_object);
        auto_throw();
    }

    inline bool contains(const Object &key) const
    {
        return PySequence_Contains(mp_object, key.mp_object) != 0;
    }

    // ===== Comparison =====
    inline bool richCompare(const Object &rhs, int op) const
    {
        int res = PyObject_RichCompareBool(mp_object, rhs.mp_object, op);
        auto_throw();
        return res != 0;
    }

    inline bool operator==(const Object &rhs)
    {
        return richCompare(rhs, Py_EQ);
    }

    inline bool operator!=(const Object &rhs)
    {
        return richCompare(rhs, Py_NE);
    }

    inline bool operator>=(const Object &rhs)
    {
        return richCompare(rhs, Py_GE);
    }

    inline bool operator<=(const Object &rhs)
    {
        return richCompare(rhs, Py_LE);
    }

    inline bool operator<(const Object &rhs)
    {
        return richCompare(rhs, Py_LT);
    }

    inline bool operator>(const Object &rhs)
    {
        return richCompare(rhs, Py_GT);
    }
};

class PYCPP_EXPORT CObject : public Object
{
public:
    CObject(PyObject *object, bool steal=false) :
        Object(object, steal)
    {
    }

    CObject(const Object &other) :
        Object(other)
    {
    }

    CObject(Object &&other) :
        Object(other)
    {
    }

    using Object::operator =;
};

} // namespace Py

#include "Tuple.h"
#include "Dict.h"

namespace Py {

// Implement calls that depend on Tuple and Dict (circle!)
inline Object Object::call()
{
    return Object(PyObject_CallObject(mp_object, NULL), true);
}

inline Object Object::call(const Tuple &args)
{
    return Object(PyObject_Call(mp_object, args.ptr(), NULL), true);
}

inline Object Object::call(const Tuple &args, const Dict &kw)
{
    return Object(PyObject_Call(mp_object, args.ptr(), kw.ptr()), true);
}

// helper
inline Object __call_method(PyObject *self, const char *method, PyObject *args, PyObject *kw)
{
    PyObject *f = PyObject_GetAttrString(self, method);
    if (!f) auto_throw();
    return Object(PyObject_Call(f, args, kw), true);
}

inline Object Object::callMethod(const char *method)
{
    return __call_method(mp_object, method, Tuple::empty().ptr(), NULL);
}

inline Object Object::callMethod(const char *method, const Tuple &args)
{
    return __call_method(mp_object, method, args.ptr(), NULL);
}

inline Object Object::callMethod(const char *method, const Tuple &args, const Dict &kw)
{
    return __call_method(mp_object, method, args.ptr(), kw.ptr());
}

} // namespace Py
