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

#include "Object.h"

// =============== Macros ===============
// MUST be incorporated into every non-copy/move constructor,
// As well as any function calling set(PyObject *, [bool]) and
// Any custom implementation of assign(PyObject *, [bool])
#define PYCPP_OBJECT_VALIDATE()                                                                \
    if (!valid(ptr()))                                                                         \
    invalid()

// SHOULD be put into subclasses of Object
// Just adding PYCPP_OBJECT_DEF_DEFAULTS() should be fine in most cases
// NOTE: There is a reason all these constructors and methods need to be
//       redefined in (almost) every subclass: They must call the subclass'
//       valid(). Virtual methods don't work because they cannot be used in
//       constructors.
#define PYCPP_OBJECT_DEF_POINTER_CONSTRUCTOR(type_name)                                        \
    type_name(PyObject *object, bool steal = false);

#define PYCPP_OBJECT_DEF_UPCAST_CONSTRUCTORS(type_name)                                        \
    type_name(const ObjectPtr &upcast);                                                        \
    type_name(ObjectPtr &&move_upcast);

#define PYCPP_OBJECT_DEF_COPYMOVE_CONSTRUCTORS(type_name)                                      \
    type_name(const type_name &copy);                                                          \
    type_name(type_name &&move);

#define PYCPP_OBJECT_DEF_UPCAST_ASSIGNMENT(type_name)                                          \
    type_name &operator=(const ObjectPtr &upcast);                                             \
    type_name &operator=(ObjectPtr &&move_upcast);

#define PYCPP_OBJECT_DEF_COPYMOVE_ASSIGNMENT(type_name)                                        \
    type_name &operator=(const type_name &copy);                                               \
    type_name &operator=(type_name &&move);

#define PYCPP_OBJECT_DEF_POINTER_ASSIGNMENT(type_name)                                         \
    void assign(PyObject *object, bool steal = false);

#define PYCPP_OBJECT_DEF_DEFAULTS(type_name)                                                   \
public:                                                                                        \
    PYCPP_OBJECT_DEF_POINTER_CONSTRUCTOR(type_name);                                           \
    PYCPP_OBJECT_DEF_UPCAST_CONSTRUCTORS(type_name);                                           \
    PYCPP_OBJECT_DEF_COPYMOVE_CONSTRUCTORS(type_name);                                         \
    PYCPP_OBJECT_DEF_UPCAST_ASSIGNMENT(type_name);                                             \
    PYCPP_OBJECT_DEF_COPYMOVE_ASSIGNMENT(type_name);                                           \
    PYCPP_OBJECT_DEF_POINTER_ASSIGNMENT(type_name);

// MUST be put in the implementation files for headers that use the above
// The implementations can be inlined by passing "inline" to the modifiers argument
// NOTE: If the subclass has any additional data members, you MAY want to use your own
//       implementation instead. You can use these macros in subsequent subclasses.
// NOTE: For templated classes, the template needs to be passed into modifiers.
#define PYCPP_OBJECT_IMPL_POINTER_CONSTRUCTOR(type_name, base_name, modifiers, tempp)          \
    modifiers type_name tempp::type_name(PyObject *object, bool steal)                         \
        : base_name(object, steal)                                                             \
    {                                                                                          \
        PYCPP_OBJECT_VALIDATE();                                                               \
    }

#define PYCPP_OBJECT_IMPL_UPCAST_CONSTRUCTORS(type_name, base_name, modifiers, tempp)          \
    modifiers type_name tempp::type_name(const ObjectPtr &upcast) : base_name(upcast)          \
    {                                                                                          \
        PYCPP_OBJECT_VALIDATE();                                                               \
    }                                                                                          \
    modifiers type_name tempp::type_name(ObjectPtr &&move_upcast) : base_name(move_upcast)     \
    {                                                                                          \
        PYCPP_OBJECT_VALIDATE();                                                               \
    }

#define PYCPP_OBJECT_IMPL_COPYMOVE_CONSTRUCTORS(type_name, base_name, modifiers, tempp)        \
    modifiers type_name tempp::type_name(const type_name &copy) : base_name(copy)              \
    {                                                                                          \
    }                                                                                          \
    modifiers type_name tempp::type_name(type_name &&move) : base_name(move)                   \
    {                                                                                          \
    }

#define PYCPP_OBJECT_IMPL_UPCAST_ASSIGNMENT(type_name, base_name, modifiers, tempp)            \
    modifiers type_name tempp &type_name tempp::operator=(const ObjectPtr &upcast)             \
    {                                                                                          \
        base_name::operator=(upcast);                                                          \
        PYCPP_OBJECT_VALIDATE();                                                               \
        return *this;                                                                          \
    }                                                                                          \
    modifiers type_name tempp &type_name tempp::operator=(ObjectPtr &&move_upcast)             \
    {                                                                                          \
        base_name::operator=(move_upcast);                                                     \
        PYCPP_OBJECT_VALIDATE();                                                               \
        return *this;                                                                          \
    }

#define PYCPP_OBJECT_IMPL_COPYMOVE_ASSIGNMENT(type_name, base_name, modifiers, tempp)          \
    modifiers type_name tempp &type_name tempp::operator=(const type_name &copy)               \
    {                                                                                          \
        base_name::operator=(copy);                                                            \
        return *this;                                                                          \
    }                                                                                          \
    modifiers type_name tempp &type_name tempp::operator=(type_name &&move)                    \
    {                                                                                          \
        base_name::operator=(move);                                                            \
        return *this;                                                                          \
    }

#define PYCPP_OBJECT_IMPL_POINTER_ASSIGNMENT(type_name, base_name, modifiers, tempp)           \
    modifiers void type_name tempp::assign(PyObject *object, bool steal)                       \
    {                                                                                          \
        base_name::assign(object, steal);                                                      \
        PYCPP_OBJECT_VALIDATE();                                                               \
    }

#define PYCPP_OBJECT_IMPL_DEFAULTS_1(type_name, base_name, modifiers, tempp)                   \
    PYCPP_OBJECT_IMPL_POINTER_CONSTRUCTOR(type_name, base_name, modifiers, tempp)              \
    PYCPP_OBJECT_IMPL_UPCAST_CONSTRUCTORS(type_name, base_name, modifiers, tempp)              \
    PYCPP_OBJECT_IMPL_COPYMOVE_CONSTRUCTORS(type_name, base_name, modifiers, tempp)            \
    PYCPP_OBJECT_IMPL_UPCAST_ASSIGNMENT(type_name, base_name, modifiers, tempp)                \
    PYCPP_OBJECT_IMPL_COPYMOVE_ASSIGNMENT(type_name, base_name, modifiers, tempp)              \
    PYCPP_OBJECT_IMPL_POINTER_ASSIGNMENT(type_name, base_name, modifiers, tempp)

#define PYCPP_OBJECT_IMPL_DEFAULTS(type_name, base_name, modifiers)                            \
    PYCPP_OBJECT_IMPL_DEFAULTS_1(type_name, base_name, modifiers, )

#define PYCPP_OBJECT_IMPL_DEFAULTS_TEMPLATE(type_name, base_name, modifiers, tempp)            \
    PYCPP_OBJECT_IMPL_DEFAULTS_1(type_name, base_name, modifiers, tempp)

// Convenience macro to define the valid(PyObject*) method inline.
#define PYCPP_OBJECT_INLINE_VALID_O(expr)                                                      \
    inline static bool valid(PyObject *o)                                                      \
    {                                                                                          \
        return expr;                                                                           \
    }

#define PYCPP_OBJECT_INLINE_VALID(check)                                                       \
    inline static bool valid(PyObject *o)                                                      \
    {                                                                                          \
        return check(o);                                                                       \
    }

namespace Py
{
// Forward declare
void PYCPP_EXPORT auto_throw();
}

// Include Tuple and Dict for call() and co.
#include "Tuple.h"
#include "Dict.h"

// =============== ObjectPtr ===============
namespace Py
{
// ===== Construct =====
inline ObjectPtr::ObjectPtr(PyObject *object, bool steal) : mp_object(object)
{
    if (!steal)
        Py_XINCREF(object);
}

inline ObjectPtr::ObjectPtr(const ObjectPtr &other) : mp_object(other.mp_object)
{
    Py_XINCREF(mp_object);
}

inline ObjectPtr::ObjectPtr(ObjectPtr &&other) : mp_object(other.mp_object)
{
    other.mp_object = NULL;
}

// ===== Assign =====
inline ObjectPtr &ObjectPtr::operator=(const ObjectPtr &rhs)
{
    set(rhs.mp_object);
    return *this;
}

inline ObjectPtr &ObjectPtr::operator=(ObjectPtr &&rhs)
{
    set(rhs.mp_object, true);
    rhs.mp_object = NULL;
    return *this;
}

inline void ObjectPtr::assign(PyObject *object, bool steal)
{
    set(object);
}

// ===== References =====
inline void ObjectPtr::incref()
{
    Py_XINCREF(mp_object);
}

inline PyObject *ObjectPtr::ptr() const
{
    return mp_object;
}

inline PyObject *ObjectPtr::steal()
{
    PyObject *p = mp_object;
    mp_object = NULL;
    return p;
}

inline PyObject *ObjectPtr::newref() const
{
    Py_XINCREF(mp_object);
    return mp_object;
}

// ===== Type safety =====
inline bool ObjectPtr::valid(PyObject *o)
{
    // ObjectPtr may hold anything
    return true;
}

// ===== Attributes =====
inline bool ObjectPtr::hasAttr(const char *attr) const
{
    return PyObject_HasAttrString(mp_object, attr);
}

inline bool ObjectPtr::hasAttr(const std::string &attr) const
{
    return PyObject_HasAttrString(mp_object, attr.c_str());
}

inline bool ObjectPtr::hasAttr(const Object &attr) const
{
    return PyObject_HasAttr(mp_object, attr.mp_object);
}

inline Object ObjectPtr::getAttr(const char *attr) const
{
    return Object(PyObject_GetAttrString(mp_object, attr), true);
}

inline Object ObjectPtr::getAttr(const std::string &attr) const
{
    return Object(PyObject_GetAttrString(mp_object, attr.c_str()), true);
}

inline Object ObjectPtr::getAttr(const Object &attr) const
{
    return Object(PyObject_GetAttr(mp_object, attr.mp_object), true);
}

inline void ObjectPtr::setAttr(const char *attr, const Object &value)
{
    PyObject_SetAttrString(mp_object, attr, value.mp_object);
    auto_throw();
}

inline void ObjectPtr::setAttr(const std::string &attr, const Object &value)
{
    PyObject_SetAttrString(mp_object, attr.c_str(), value.mp_object);
    auto_throw();
}

inline void ObjectPtr::setAttr(const Object &attr, const Object &value)
{
    PyObject_SetAttr(mp_object, attr.mp_object, value.mp_object);
    auto_throw();
}

inline void ObjectPtr::delAttr(const char *attr)
{
    PyObject_DelAttrString(mp_object, attr);
    auto_throw();
}

inline void ObjectPtr::delAttr(const std::string &attr)
{
    PyObject_DelAttrString(mp_object, attr.c_str());
    auto_throw();
}

inline void ObjectPtr::delAttr(const Object &attr)
{
    PyObject_DelAttr(mp_object, attr.mp_object);
}

// ===== Callables =====
inline Object ObjectPtr::call()
{
    return Object(PyObject_CallObject(mp_object, NULL), true);
}

inline Object ObjectPtr::call(const Tuple &args)
{
    return Object(PyObject_Call(mp_object, args.ptr(), NULL), true);
}

inline Object ObjectPtr::call(const Tuple &args, const Dict &kw)
{
    return Object(PyObject_Call(mp_object, args.ptr(), kw.ptr()), true);
}

// ===== Methods =====
// helper
inline Object __call_method(PyObject *self, const char *method, PyObject *args, PyObject *kw)
{
    PyObject *f = PyObject_GetAttrString(self, method);
    if (!f)
        auto_throw();
    return Object(PyObject_Call(f, args, kw), true);
}

inline Object ObjectPtr::callMethod(const char *method)
{
    return __call_method(mp_object, method, Tuple::empty().ptr(), NULL);
}

inline Object ObjectPtr::callMethod(const char *method, const Tuple &args)
{
    return __call_method(mp_object, method, args.ptr(), NULL);
}

inline Object ObjectPtr::callMethod(const char *method, const Tuple &args, const Dict &kw)
{
    return __call_method(mp_object, method, args.ptr(), kw.ptr());
}

// ===== Identity Check =====
inline bool ObjectPtr::is(PyObject *other) const
{
    return mp_object == other;
}

inline bool ObjectPtr::is(const ObjectPtr &other) const
{
    return mp_object == other.mp_object;
}

// ===== Protocol Checks =====
inline bool ObjectPtr::isCallable() const
{
    return PyCallable_Check(mp_object);
}

inline bool ObjectPtr::isSequence() const
{
    return PySequence_Check(mp_object);
}

inline bool ObjectPtr::isMapping() const
{
    return PyMapping_Check(mp_object);
}

inline bool ObjectPtr::isNumber() const
{
    return PyNumber_Check(mp_object);
}

// ====== Type Checks =====
inline bool ObjectPtr::isNull() const
{
    return mp_object == NULL;
}

inline bool ObjectPtr::isNone() const
{
    return mp_object == Py_None;
}

inline bool ObjectPtr::isLong() const
{
    return PyLong_Check(mp_object);
}

inline bool ObjectPtr::isFloat() const
{
    return PyFloat_Check(mp_object);
}

inline bool ObjectPtr::isType() const
{
    return PyType_Check(mp_object);
}

inline bool ObjectPtr::isString() const
{
    return PyUnicode_Check(mp_object);
}

inline bool ObjectPtr::isBytes() const
{
    return PyBytes_Check(mp_object);
}

inline bool ObjectPtr::isBool() const
{
    return PyBool_Check(mp_object);
}

inline bool ObjectPtr::isTuple() const
{
    return PyTuple_Check(mp_object);
}

inline bool ObjectPtr::isList() const
{
    return PyList_Check(mp_object);
}

inline bool ObjectPtr::isDict() const
{
    return PyDict_Check(mp_object);
}

// ===== Sequence/Mapping protocol =====
inline Object ObjectPtr::getItem(const Object &key) const
{
    return Object(PyObject_GetItem(mp_object, key.mp_object));
}

inline void ObjectPtr::setItem(const Object &key, const Object &value)
{
    PyObject_SetItem(mp_object, key.mp_object, value.mp_object);
    auto_throw();
}

inline void ObjectPtr::delItem(const Object &key)
{
    PyObject_DelItem(mp_object, key.mp_object);
    auto_throw();
}

inline bool ObjectPtr::contains(const Object &key) const
{
    return PySequence_Contains(mp_object, key.mp_object) != 0;
}

// ===== Comparison =====
inline bool ObjectPtr::richCompare(const Object &rhs, int op) const
{
    int res = PyObject_RichCompareBool(mp_object, rhs.mp_object, op);
    auto_throw();
    return res != 0;
}

} // namespace Py

#include "Conv.h"

// =============== Object ===============
namespace Py
{
inline bool Object::valid(PyObject *o)
{
    // Anything but NULL
    return o;
}

// ===== Construct =====
// From Pointer
inline Object::Object(PyObject *object, bool steal) : ObjectPtr(object, steal)
{
    PYCPP_OBJECT_VALIDATE();
}

inline Object::Object() : Object(Py_None)
{
}

PYCPP_OBJECT_IMPL_UPCAST_CONSTRUCTORS(Object, ObjectPtr, inline, )
PYCPP_OBJECT_IMPL_COPYMOVE_CONSTRUCTORS(Object, ObjectPtr, inline, )

// From Value
template <typename T, typename enable_if_non_object<T>::type>
inline Object::Object(T val)
    : ObjectPtr(Py::Conv::newFromValue(val), true)
{
    PYCPP_OBJECT_VALIDATE();
}

// ===== Assign =====
PYCPP_OBJECT_IMPL_UPCAST_ASSIGNMENT(Object, ObjectPtr, inline, )
PYCPP_OBJECT_IMPL_COPYMOVE_ASSIGNMENT(Object, ObjectPtr, inline, )

// From value
template <typename T, typename enable_if_non_object<T>::type>
inline Object &Object::operator=(T val)
{
    set(Conv::newFromValue(val), true);
    PYCPP_OBJECT_VALIDATE();
}

// ===== Callables =====
template <typename... Args>
inline Object Object::operator()(Args... targs)
{
    Tuple args({ targs... });
    return call(args);
}

// ===== Methods =====
template <typename... Args>
inline Object Object::m(const char *method, Args... targs)
{
    Tuple args({ targs... });
    return callMethod(method, args);
}

template <typename... Args>
inline Object Object::m(const std::string &method, Args... targs)
{
    Tuple args({ targs... });
    return callMethod(method.c_str(), args);
}

// ===== Comparison =====
inline bool Object::operator==(const Object &rhs) const
{
    return richCompare(rhs, Py_EQ);
}

inline bool Object::operator!=(const Object &rhs) const
{
    return richCompare(rhs, Py_NE);
}

inline bool Object::operator>=(const Object &rhs) const
{
    return richCompare(rhs, Py_GE);
}

inline bool Object::operator<=(const Object &rhs) const
{
    return richCompare(rhs, Py_LE);
}

inline bool Object::operator<(const Object &rhs) const
{
    return richCompare(rhs, Py_LT);
}

inline bool Object::operator>(const Object &rhs) const
{
    return richCompare(rhs, Py_GT);
}

} // namespace Py
