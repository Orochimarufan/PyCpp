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

#include "PyC++ExConfig.h"
#include <PyC++/Object.h>

namespace Py
{

/**
 * @brief The Class class
 * Subclass this to create new python types.
 *
 * Subclasses MAY have a "static const char *__name__" member.
 * Otherwise, __name__ will be taken from RTTI
 *
 * Defining members:
 * PC_MEMBERS(<Class>)
 * {
 *     // Members
 *     CMD_MEMBER(<member name>[, docstring]);
 *     CMD_READONLY_MEMBER(<member name>[, docstring]);
 *
 *     CMD_MEMBER_TYPE(<member name>, <PyMemberDef type>[, docstring]);
 *     CMD_READONLY_MEMBER_TYPE(<member name>, <PyMemberDef type>[, docstring]);
 *
 *     CMD_MEMBER_NAME(<name>, <member name>[, docstring]);
 *     CMD_READONLY_MEMBER_NAME(<name>, <member name>[, docstring]);
 *
 *     CMD_MEMBER_TYPE_NAME(<name>, <member name>, <PyMemberDef type>[, docstring]);
 *     CMD_READONLY_MEMBER_TYPE_NAME(<name>, <member name>, <PyMemberDef type>[, docstring);
 *
 *     // Properties
 *     CMD_PROPERTY(<name>, <getter method name>, <setter method name>[, docstring[, closure]]);
 *     CMD_READONLY_PROPERTY(<name>, <getter method name>[, docstring[, closure]]);
 *     CMD_WRITEONLY_PROPERTY(<name>, <setter method name>[, docstring[, closure]]);
 *
 *     // Methods
 *     CMD_METHOD(<method name>[, docstring]);
 *     CMD_METHOD_NAME(<name>, <method name>[, docstring]);s
 * }
 *
 * Where "<x> name" is the name of a class' <x> (member, method...)
 * and "name" is the name of the resulting python attribute (where differing)
 *
 * "docstring" is the python documentation string, "closure" is a void pointer for
 *  user data so one getter/setter can be used for multiple properties.
 *
 * PC_MEMBERS must be defined in the root namespace!
 *
 * Different signatures are available for methods:
 *  - Object ()             [METH_NOARGS]
 *  - Object (Object)       [METH_O]
 *  - Object (Tuple)        [METH_VARARGS]
 *  - Object (Tuple, Dict)  [METH_VARARGS|METH_KEYWORDS]
 *
 * Plus all of the above returning void. (Will always return None in that case)
 *
 * Getter and setter methods have the following signatures:
 *  - Object (void *closure)                [getter]
 *  - void   (Object value, void *closure)  [setter]
 *
 * In all signatures, constant references are used (const Object &, etc)
 */
class PYCPPEX_EXPORT Class : public PyObject
{
protected:
    Class();

public:
    static const char *__name__;

    // ===== Virtual Methods =====
    // Initialization. Use the C++ constructor for allocation only (__new__)!
    Class(const Tuple &args, const Dict &kw);
    virtual void __init__(const Tuple &args, const Dict &kw);
    virtual ~Class();

    // Stringify (__str__ calls __repr__ by default)
    virtual String __repr__();
    virtual String __str__();

    // Hash
    virtual Py_hash_t __hash__();

    // Call
    virtual Object operator()(const Tuple &args, const Dict &kw);

    // Attributes
    virtual Object __getattr__(const String &attr);
    virtual void __setattr__(const String &attr, const Object &value);
    virtual void __delattr__(const String &attr);

    // Garbage collection
    virtual int __traverse__(visitproc visit, void *arg);
    virtual int __clear__();

    // Comparison. __richcompare__ default implementation calls the comparison operators.
    // Probably pretty slow.
    virtual Object __richcompare__(const Object &rhs, int op);
    virtual Object operator==(const Object &rhs);
    virtual Object operator!=(const Object &rhs);
    virtual Object operator>(const Object &rhs);
    virtual Object operator>=(const Object &rhs);
    virtual Object operator<(const Object &rhs);
    virtual Object operator<=(const Object &rhs);

    // Iterator interface
    virtual Object __iter__();
    virtual Object __next__();

    // Descriptor interface
    virtual Object __get__(const Object &obj, const Type &type);
    virtual Object __set__(const Object &obj, const Object &value);

    // Print
    virtual void __print__(FILE *file, int flags);
};


/**
 * @brief Must be defined for all indirect subclasses of Class
 *
 * Member types:
 *  base_type -> the Class subclass inheriting from
 *
 * Member vars:
 *  base_tp -> (static) Pointer to the Python base type
 *
 * If base_type is Py::Class, base_tp may be used
 *  to specify a python base class other than 'object'
 * Otherwise, base_tp must be CppType<base_type>::ptr()
 *
 * NULL can be used in base_tp to indicate 'object' (PyBaseObject_Type)
 */
template <typename Cls>
struct PYCPPEX_EXPORT ClassBase
{
    typedef Py::Class base_type;
    static PyTypeObject *base_tp;
};

template <typename Cls>
PyTypeObject *ClassBase<Cls>::base_tp = NULL;

#define PYCPP_INHERIT_FROM(CLS, BASE)                                                          \
    template <>                                                                                \
    struct ClassBase<CLS>                                                                      \
    {                                                                                          \
        typedef BASE base_type;                                                                \
        static PyTypeObject *base_tp = CppType<BASE>::ptr();                                   \
    }

#define PYCPP_INHERIT_FROM_PYTHON(CLS, BASE_TP)                                                \
    template <>                                                                                \
    struct ClassBase<CLS>                                                                      \
    {                                                                                          \
        typedef Py::Class base_type;                                                           \
        static PyTypeObject *base_tp = BASE_TP;                                                \
    }


// forward
namespace detail
{
template <typename Cls>
class ClassMemberDefinition;
}

/**
 * @brief The python type object for a Class
 *
 * ready() should be called before use!
 */
template <typename Cls>
class PYCPPEX_EXPORT CppType : public PyTypeObject
{
private:
    CppType();

    static CppType<Cls> *tp;

    // Slot wrappers
    static void dealloc(PyObject *self);
    static int print(PyObject *self, FILE *f, int flags);
    static PyObject *repr(PyObject *self);
    static Py_hash_t hash(PyObject *self);
    static PyObject *call(PyObject *self, PyObject *args, PyObject *kw);
    static PyObject *str(PyObject *self);
    static PyObject *getattro(PyObject *self, PyObject *attr);
    static int setattro(PyObject *self, PyObject *attr, PyObject *value);
    static int traverse(PyObject *self, visitproc visit, void *arg);
    static int clear(PyObject *self);
    static PyObject *richcompare(PyObject *self, PyObject *rhs, int op);
    static PyObject *iter(PyObject *self);
    static PyObject *iternext(PyObject *self);
    static PyObject *descr_get(PyObject *self, PyObject *obj, PyObject *type);
    static int descr_set(PyObject *self, PyObject *obj, PyObject *value);
    static int init(PyObject *self, PyObject *args, PyObject *kwds);
    static PyObject *new_(PyTypeObject *subtype, PyObject *args, PyObject *kwds);

    // Custom method wrappers
    friend class detail::ClassMemberDefinition<Cls>;
    typedef Object (Cls::*member_method_fn)(const Tuple &);
    typedef Object (Cls::*member_method_kw)(const Tuple &, const Dict &kw);
    typedef Object (Cls::*member_method_obj)(const Object &);
    typedef Object (Cls::*member_method_void)(void);

    typedef void (Cls::*member_method_fn_none)(const Tuple &);
    typedef void (Cls::*member_method_kw_none)(const Tuple &, const Dict &);
    typedef void (Cls::*member_method_obj_none)(const Object &);
    typedef void (Cls::*member_method_void_none)(void);

    typedef Object (Cls::*member_getter_fn)(void *);
    typedef void (Cls::*member_setter_fn)(const Object &, void *);

    template <member_method_fn fn> // METH_VARARGS
    static PyObject *wrap_method(PyObject *self, PyObject *args);

    template <member_method_kw fn> // METH_VARARGS | METH_KEYWORDS
    static PyObject *wrap_method(PyObject *self, PyObject *args, PyObject *kw);

    template <member_method_obj fn> // METH_O
    static PyObject *wrap_method(PyObject *self, PyObject *object);

    template <member_method_void fn> // METH_NOARGS
    static PyObject *wrap_method(PyObject *self, PyObject *);

    template <member_method_fn_none fn> // METH_VARARGS -> None
    static PyObject *wrap_method(PyObject *self, PyObject *args);

    template <member_method_kw_none fn> // METH_VARARGS | METH_KEYWORDS -> None
    static PyObject *wrap_method(PyObject *self, PyObject *args, PyObject *kw);

    template <member_method_obj_none fn> // METH_O -> None
    static PyObject *wrap_method(PyObject *self, PyObject *object);

    template <member_method_void_none fn> // METH_NOARGS -> None
    static PyObject *wrap_method(PyObject *self, PyObject *);

    template <member_getter_fn fn>
    static PyObject *wrap_getter(PyObject *self, void *closure);

    template <member_setter_fn fn>
    static int wrap_setter(PyObject *self, PyObject *value, void *closure);

public:
    static void ready();

    static Type get();
    static PyTypeObject *ptr();
};

template <typename Cls>
CppType<Cls> *CppType<Cls>::tp = nullptr;


/**
 * @brief C++ object reference
 * @param Class the \c Class subclass to reference
 *
 * You should subclass this for your type and provide specialized constructors
 */
template <typename Cls>
class PYCPPEX_EXPORT CppObject : public CppObject<typename ClassBase<Cls>::base_type>
{
public:
    // Member types
    typedef Cls py_object;
    typedef CppObject<Cls> object;
    typedef CppType<Cls> type;

    typedef typename ClassBase<Cls>::base_type base_class;

    typedef base_class base_py_object;
    typedef CppObject<base_class> base_object;
    typedef CppType<base_class> base_type;

    // Constructors
    CppObject() : CppObject(Tuple::empty(), Dict::empty())
    {
    }

    CppObject(const Tuple &args, const Dict &kw)
        : base_object(static_cast<Cls *>(type::ptr()->tp_new(type::ptr(), args.ptr(), kw.ptr())),
                      true)
    {
        Object::callMethod("__init__", args, kw);
    }

    // Construct from pointer
    CppObject(Class *object, bool steal = false) : base_object(object, steal)
    {
    }

    // Copy & Move Assignment
    CppObject(const CppObject<Cls> &o) : base_object(o)
    {
    }

    CppObject(CppObject<Cls> &&o) : base_object(o)
    {
    }

    object &operator=(const object &o)
    {
        Object::operator=(o);
        return *this;
    }

    object &operator=(object &&o)
    {
        Object::operator=(o);
        return *this;
    }

    // Access Type
    py_object *operator->()
    {
        return static_cast<py_object *>(Object::ptr());
    }

    // Templated constructor
    template <typename... Args>
    static object create(Args... targs)
    {
        return object(Tuple{ targs... }, Dict());
    }
};

template <>
class PYCPPEX_EXPORT CppObject<Class> : public Object
{
protected:
    CppObject(Class *object, bool steal = false) : Object(object, steal)
    {
    }

    CppObject(const CppObject<Class> &o) : Object(o)
    {
    }

    CppObject(CppObject<Class> &&o) : Object(o)
    {
    }

    CppObject<Class> &operator=(const CppObject<Class> &o)
    {
        Object::operator=(o);
        return *this;
    }

    CppObject<Class> &operator=(CppObject<Class> &&o)
    {
        Object::operator=(o);
        return *this;
    }
};

} // namespace Py

#include "CppType_impl.h"
