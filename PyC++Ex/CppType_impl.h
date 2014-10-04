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

#include "CppObject.h"
#include "Exception.h"

#include <typeinfo>

#define check_overridden(method) (typeid(&ClassBase<Class>::base_type::method) != typeid(&Class::method))
#define if_overridden(method, wrap) (check_overridden(method) ? &wrap : NULL)
#define if_overridden_else(method, wrap, x) (check_overridden(method) ? &wrap : &x)

#define catch_raise_return(ret) catch(BaseException &e) { e.raise(); return ret; }
#define try_dispatch_func(expr) try { return static_cast<Cls*>(self)->expr; }
#define try_call_func_return(expr, ret) try { static_cast<Cls*>(self)->expr; return ret; }
#define try_dispatch_member(name, expr) try { return (static_cast<Cls*>(self)->*name)expr; }
#define try_call_member_return(name, expr, ret) try { (static_cast<Cls*>(self)->*name)expr; return ret; }
#define try_call_member_return_none(name, expr) try { (static_cast<Cls*>(self)->*name)expr; Py_RETURN_NONE; }

// need structure of Py*Def, don't want to include <structmember.h>
struct PyMemberDef {
    const char *name;
    int type;
    Py_ssize_t offset;
    int flags;
    char *doc;
};

namespace Py {

// ==== Member definition ====
namespace detail {

template <typename Cls>
class ClassMemberDefinition
{
    template <typename T>
    struct llnode
    {
        T value;
        llnode<T> *next;

        void del()
        {
            if (this)
            {
                next->del();
                delete this;
            }
        }

        unsigned size(unsigned have=0)
        {
            if (!this)
                return have;
            return next->size(++have);
        }
    };

    llnode<PyMemberDef> *members;
    llnode<PyMethodDef> *methods;
    llnode<PyGetSetDef> *getsets;

    template <typename T>
    T *compile(llnode<T> *start)
    {
        if (!start)
            return nullptr;

        T *d = new T[methods->size() + 1];

        int i = 0;
        for (llnode<T> *it = start; it; it = it->next)
        {
            d[i] = it->value;
            ++i;
        }

        d[i] = {NULL, 0, 0, 0};

        return d;
    }

    template <typename T, T fn>
    inline void add_method_g(const char *name, int flags, char *doc)
    {
        methods = new llnode<PyMethodDef>{
            {
                name,
                &CppType<Cls>::template wrap_method<fn>,
                flags,
                doc
            },
            methods
        };
    }

public:
    ClassMemberDefinition() :
        members(nullptr),
        methods(nullptr),
        getsets(nullptr)
    {
    }

    ~ClassMemberDefinition()
    {
        members->del();
        methods->del();
        getsets->del();
    }

    typedef Cls class_type;

    typedef typename CppType<Cls>::member_method_fn member_fn;
    typedef typename CppType<Cls>::member_method_kw member_kw_fn;
    typedef typename CppType<Cls>::member_method_obj member_obj_fn;
    typedef typename CppType<Cls>::member_method_void member_void_fn;

    typedef typename CppType<Cls>::member_method_fn_none member_none_fn;
    typedef typename CppType<Cls>::member_method_kw_none member_none_kw_fn;
    typedef typename CppType<Cls>::member_method_obj_none member_none_obj_fn;
    typedef typename CppType<Cls>::member_method_void_none member_none_void_fn;

    typedef typename CppType<Cls>::member_getter_fn member_get_fn;
    typedef typename CppType<Cls>::member_setter_fn member_set_fn;

    // ==== Add stuff ====
    // Members
    void add_member(const char *name, int type, Py_ssize_t offset, int flags, char *doc)
    {
        members = new llnode<PyMemberDef>{
                {name, type, offset, flags, doc},
                members
        };
    }

    // Methods
    template <member_fn fn>
    void add_method(const char *name, int flags, char *doc)
    {
        add_method_g<member_fn, fn>(name, METH_VARARGS | flags, doc);
    }

    template <member_kw_fn fn>
    void add_method(const char *name, int flags, char *doc)
    {
        add_method_g<member_kw_fn, fn>(name, METH_VARARGS | METH_KEYWORDS | flags, doc);
    }

    template <member_obj_fn fn>
    void add_method(const char *name, int flags, char *doc)
    {
        add_method_g<member_obj_fn, fn>(name, METH_O | flags, doc);
    }

    template <member_void_fn fn>
    void add_method(const char *name, int flags, char *doc)
    {
        add_method_g<member_void_fn, fn>(name, METH_NOARGS | flags, doc);
    }

    template <member_none_fn fn>
    void add_method(const char *name, int flags, char *doc)
    {
        add_method_g<member_none_fn, fn>(name, METH_VARARGS | flags, doc);
    }

    template <member_none_kw_fn fn>
    void add_method(const char *name, int flags, char *doc)
    {
        add_method_g<member_none_kw_fn, fn>(name, METH_VARARGS | METH_KEYWORDS | flags, doc);
    }

    template <member_none_obj_fn fn>
    void add_method(const char *name, int flags, char *doc)
    {
        add_method_g<member_none_obj_fn, fn>(name, METH_O | flags, doc);
    }

    template <member_none_void_fn fn>
    void add_method(const char *name, int flags, char *doc)
    {
        add_method_g<member_none_void_fn, fn>(name, METH_NOARGS | flags, doc);
    }

    // Properties
    template <member_get_fn get_fn, member_set_fn set_fn>
    void add_getset(const char *name, char *doc, void *closure)
    {
        getsets = new llnode<PyGetSetDef>{
            {
                (char*)name,
                &CppType<Cls>::template wrap_getter<get_fn>,
                &CppType<Cls>::template wrap_setter<set_fn>,
                doc,
                closure
            },
            getsets
        };
    }

    template <member_get_fn get_fn>
    void add_getset(const char *name, char *doc, void *closure)
    {
        getsets = new llnode<PyGetSetDef>{
            {
                (char*)name,
                &CppType<Cls>::template wrap_getter<get_fn>,
                NULL,
                doc,
                closure
            },
            getsets
        };
    }

    template <member_set_fn set_fn>
    void add_getset(const char *name, char *doc, void *closure)
    {
        getsets = new llnode<PyGetSetDef>{
            {
                (char*)name,
                NULL,
                &CppType<Cls>::template wrap_setter<set_fn>,
                doc,
                closure
            },
            getsets
        };
    }

    // Returns a null Cls pointer for offset calculation
    static inline Cls *null()
    {
        return reinterpret_cast<Cls*>(0);
    }

    // Collect linked lists into an array
    // Caller is responsible for deleting!
    PyMethodDef *compile_methods()
    {
        return compile<PyMethodDef>(methods);
    }

    PyMemberDef *compile_members()
    {
        return compile<PyMemberDef>(members);
    }

    PyGetSetDef *compile_getsets()
    {
        return compile<PyGetSetDef>(getsets);
    }
};

template <typename Cls>
static void __pc_members(ClassMemberDefinition<Cls> *)
{
}

// === Member Type map ===
// Maps standard types to T_* constants from Python structmembers.h
// int typeid = cmd_struct_typeid<TYPE>::value;
template <typename T>
struct cmd_struct_typeid;

#define DEF_TYPE(type, typeid_) template <> struct cmd_struct_typeid<type> { static int const value = typeid_; }

// See cpython/Include/structmembers.h

DEF_TYPE(short,     0); // T_SHORT
DEF_TYPE(int,       1); // T_INT
DEF_TYPE(long,      2); // T_LONG
DEF_TYPE(float,     3); // T_FLOAT
DEF_TYPE(double,    4); // T_DOUBLE
//DEF_TYPE(char *,    5); // T_STRING [Implies READONLY!] TODO: breaks?
// T_OBJECT = 6
DEF_TYPE(char,      7); // T_CHAR
DEF_TYPE(int8_t,    8); // T_BYTE
DEF_TYPE(uint8_t,   9); // T_UBYTE
DEF_TYPE(unsigned short, 10); // T_USHORT
DEF_TYPE(unsigned int,   11); // T_UINT
DEF_TYPE(unsigned long,  12); // T_ULONG
// T_STRING_INPLACE = 13
// T_BOOL = 14
DEF_TYPE(PyObject*, 16); // T_OBJECT_EX
#if HAVE_LONG_LONG
DEF_TYPE(long long,      17); // T_LONGLONG
DEF_TYPE(unsigned long long, 18); // T_ULONGLONG
#endif
// T_PYSSIZET = 19 :: typedef'd to long, so redefinition.
// T_NONE = 20

// We don't care about const. Users should make sure to make the member READONLY when mapping const members.
template <typename T>
struct cmd_struct_typeid<T const>
{
    static int const value = cmd_struct_typeid<T>::value;
};

#undef DEF_TYPE

template <typename T>
struct remove_pointer
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T*>
{
    typedef T type;
};

// macros
#define PC_MEMBERS(type) namespace Py { namespace detail { template <> void __pc_members<type>(ClassMemberDefinition<type> *); } }\
    template<> void Py::detail::__pc_members<type>(ClassMemberDefinition<type> *__cmd)

#define CMD_CLASS Py::detail::remove_pointer<decltype(__cmd)>::type::class_type

// Members
#define CMD_PYOBJECT_OFFSET ((intptr_t)static_cast<PyObject*>(reinterpret_cast<CMD_CLASS*>(1))-1)
#define CMD_MEMBER_OFFSET(name) (((intptr_t)&(__cmd->null()->name)) - CMD_PYOBJECT_OFFSET)
#define CMD_MEMBER_TYPEID(name) Py::detail::cmd_struct_typeid<decltype(CMD_CLASS::name)>::value

#define CMD_MEMBER_1(name, doc, flags) __cmd->add_member(#name, CMD_MEMBER_TYPEID(name), CMD_MEMBER_OFFSET(name), flags, doc)
#define CMD_MEMBER(...) CMD_MEMBER_1(__VA_ARGS__, 0, 0)

#define CMD_MEMBER_NAME_1(name, member, doc, flags) __cmd->add_member(#name, CMD_MEMBER_TYPEID(member), CMD_MEMBER_OFFSET(member), flags, doc)
#define CMD_MEMBER_NAME(...) CMD_MEMBER_NAME_1(__VA_ARGS__, 0, 0)

#define CMD_MEMBER_TYPE_1(name, type, doc, flags) __cmd->add_member(#name, type, CMD_MEMBER_OFFSET(name), flags, doc)
#define CMD_MEMBER_TYPE(...) CMD_MEMBER_TYPE_1(__VA_ARGS__, 0, 0)

#define CMD_MEMBER_TYPE_NAME_1(name, member, type, doc, flags) __cmd->add_member(#name, type, CMD_MEMBER_OFFSET(member), flags, doc)
#define CMD_MEMBER_TYPE_NAME(...) CMD_MEMBER_TYPE_NAME_1(__VA_ARGS__, 0, 0)

#define CMD_READONLY_MEMBER_1(name, doc) __cmd->add_member(#name, CMD_MEMBER_TYPEID(name), CMD_MEMBER_OFFSET(name), 1, doc)
#define CMD_READONLY_MEMBER(...) CMD_READONLY_MEMBER_1(__VA_ARGS__, NULL)

#define CMD_READONLY_MEMBER_NAME_1(name, member, doc) __cmd->add_member(#name, CMD_MEMBER_TYPEID(member), CMD_MEMBER_OFFSET(member), 1, doc)
#define CMD_READONLY_MEMBER_NAME(...) CMD_READONLY_MEMBER_NAME_1(__VA_ARGS__, NULL)

#define CMD_READONLY_MEMBER_TYPE_1(name, type, doc) __cmd->add_member(#name, type, CMD_MEMBER_OFFSET(name), 1, doc)
#define CMD_READONLY_MEMBER_TYPE(...) CMD_READONLY_MEMBER_TYPE_1(__VA_ARGS__, NULL)

#define CMD_READONLY_MEMBER_TYPE_NAME_1(name, member, type, doc) __cmd->add_member(#name, type, CMD_MEMBER_OFFSET(member), 1, doc)
#define CMD_READONLY_MEMBER_TYPE_NAME(...) CMD_READONLY_MEMBER_TYPE_NAME_1(__VA_ARGS__, NULL)

// Methods
#define CMD_METHOD_1(name, doc, flags) __cmd->add_method<&CMD_CLASS::name>(#name, flags, doc)
#define CMD_METHOD(...) CMD_METHOD_1(__VA_ARGS__, 0, 0)

#define CMD_METHOD_NAME_1(meth, name, doc, flags) __cmd->add_method<&CMD_CLASS::meth>(#name, flags, doc)
#define CMD_METHOD_NAME(...) CMD_METHOD_NAME_1(__VA_ARGS__, 0, 0)

// Properties
#define CMD_PROPERTY_1(name, get_meth, set_meth, doc, closure) __cmd->add_getset<&CMD_CLASS::get_meth, &CMD_CLASS::set_meth>(#name, doc, closure)
#define CMD_PROPERTY(...) CMD_PROPERTY_1(__VA_ARGS__, NULL, NULL)

#define CMD_READONLY_PROPERTY_1(name, get_meth, doc, closure) __cmd->add_getset<&CMD_CLASS::get_meth>(#name, doc, closure)
#define CMD_READONLY_PROPERTY(...) CMD_READONLY_PROPERTY_1(__VA_ARGS__, NULL, NULL)

#define CMD_WRITEONLY_PROPERTY_1(name, set_meth, doc, closure) __cmd->add_getset<CMD_CLASS::set_meth>(#name, doc, closure)
#define CMD_WRITEONLY_PROPERTY(...) CMD_WRITEONLY_PROPERTY_1(__VA_ARGS__, NULL, NULL)

} // namespace detail

// ==== Creation ====
template <typename Class>
CppType<Class>::CppType() :
    PyTypeObject{ PyVarObject_HEAD_INIT(NULL, 0) }
{
    // Name
    if (Class::__name__ == Py::Class::__name__)
    {
        tp_name = typeid(Class).name();
        Class::__name__ = tp_name;
    }
    else
        tp_name = Class::__name__;

    // Size
    tp_basicsize = sizeof(Class);
    tp_itemsize = 0;

    // Dealloc
    tp_dealloc = &dealloc;

    // Print
    tp_print = if_overridden(__print__, print);

    // Repr
    tp_repr = &repr;

    // Additional Protocols
    tp_as_number = NULL;
    tp_as_sequence = NULL;
    tp_as_mapping = NULL;

    // Hash
    tp_hash = if_overridden(__hash__, hash);

    // Call
    tp_call = if_overridden(operator(), call);

    // Str
    tp_str = if_overridden(__str__, str);

    // New Attributes
    //tp_getattro = if_overridden_else(__getattr__, getattro, PyObject_GenericGetAttr);
    //tp_setattro = if_overridden_else(__setattr__, setattro, PyObject_GenericSetAttr);
    tp_getattro = if_overridden(__getattr__, getattro);
    tp_setattro = check_overridden(__setattr__) || check_overridden(__delattr__) ? &setattro : NULL;

    // Buffer Protocol
    tp_as_buffer = NULL;

    // Flags
    tp_flags = 0;

    // Doc
    tp_doc = NULL;

    // GC
    tp_traverse = if_overridden(__traverse__, traverse);
    tp_clear = if_overridden(__clear__, clear);

    // Compare
    tp_richcompare = if_overridden(__richcompare__, richcompare);

    // Weaklistoffset
    tp_weaklistoffset = 0;

    // Iter Protocol
    tp_iter = if_overridden(__iter__, iter);
    tp_iternext = if_overridden(__next__, iternext);

    // Members
    {
        detail::ClassMemberDefinition<Class> cmd;

        detail::__pc_members<Class>(&cmd);

        tp_methods = cmd.compile_methods();
        tp_members = cmd.compile_members();
        tp_getset = cmd.compile_getsets();
    }

    // Base
    tp_base = ClassBase<Class>::base_tp;

    // Dict
    tp_dict = NULL;

    // Descriptor Protocol
    tp_descr_get = if_overridden(__get__, descr_get);
    tp_descr_set = if_overridden(__set__, descr_set);

    // Init
    tp_init = if_overridden(__init__, init);

    // Allocate
    tp_new = &new_;
}

// ==== Slot Wrappers ====
template <typename Cls>
void CppType<Cls>::dealloc(PyObject *self)
{
    freefunc free = Py_TYPE(self)->tp_free;
    static_cast<Cls*>(self)->~Cls();
    free(reinterpret_cast<PyObject*>(static_cast<Cls*>(self)));
}

template <typename Cls>
int CppType<Cls>::print(PyObject *self, FILE *f, int flags)
{
    try_call_func_return(__print__(f, flags), 0) catch_raise_return(-1)
}

template <typename Cls>
PyObject *CppType<Cls>::repr(PyObject *self)
{
    try_dispatch_func(__repr__().steal()) catch_raise_return(NULL)
}

template <typename Cls>
Py_hash_t CppType<Cls>::hash(PyObject *self)
{
    try_dispatch_func(__hash__()) catch_raise_return(-1)
}

template <typename Cls>
PyObject *CppType<Cls>::call(PyObject *self, PyObject *args, PyObject *kw)
{
    try_dispatch_func(operator()(args, kw).steal()) catch_raise_return(NULL)
}

template <typename Cls>
PyObject *CppType<Cls>::str(PyObject *self)
{
    try_dispatch_func(__str__().steal()) catch_raise_return(NULL);
}

template <typename Cls>
PyObject *CppType<Cls>::getattro(PyObject *self, PyObject *attr)
{
    try_dispatch_func(__getattr__(attr).steal()) catch_raise_return(NULL)
}

template <typename Cls>
int CppType<Cls>::setattro(PyObject *self, PyObject *attr, PyObject *value)
{
    if (value)
    {
        try_call_func_return(__setattr__(attr, value), 0) catch_raise_return(-1)
    }
    else
    {
        try_call_func_return(__delattr__(attr), 0) catch_raise_return(-1)
    }
}

template <typename Cls>
int CppType<Cls>::traverse(PyObject *self, visitproc visit, void *arg)
{
    try_dispatch_func(__traverse__(visit, arg)) catch_raise_return(-1)
}

template <typename Cls>
int CppType<Cls>::clear(PyObject *self)
{
    try_dispatch_func(__clear__()) catch_raise_return(-1)
}

template <typename Cls>
PyObject *CppType<Cls>::richcompare(PyObject *self, PyObject *rhs, int op)
{
    try_dispatch_func(__richcompare__(rhs, op).steal()) catch_raise_return(NULL)
}

template <typename Cls>
PyObject *CppType<Cls>::iter(PyObject *self)
{
    try_dispatch_func(__iter__().steal()) catch_raise_return(NULL)
}

template <typename Cls>
PyObject *CppType<Cls>::iternext(PyObject *self)
{
    try_dispatch_func(__next__().steal()) catch_raise_return(NULL)
}

template <typename Cls>
PyObject *CppType<Cls>::descr_get(PyObject *self, PyObject *obj, PyObject *type)
{
    try_dispatch_func(__get__(obj, type).steal()) catch_raise_return(NULL)
}

template <typename Cls>
int CppType<Cls>::descr_set(PyObject *self, PyObject *obj, PyObject *value)
{
    try_call_func_return(__set__(obj, value), 0) catch_raise_return(-1)
}

template <typename Cls>
int CppType<Cls>::init(PyObject *self, PyObject *args, PyObject *kwds)
{
    try_call_func_return(__init__(args, kwds ? kwds : Dict::empty()), 0) catch_raise_return(-1)
}

template <typename Cls>
PyObject *CppType<Cls>::new_(PyTypeObject *subtype, PyObject *args, PyObject *kwds)
{
    void *mem;
    PyObject *py;
    Cls *self;

    // Allocate
    mem = reinterpret_cast<void*>(subtype->tp_alloc(subtype, 0));
    if (!mem)
        return NULL;

    self = reinterpret_cast<Cls*>(mem);
    py = static_cast<PyObject*>(self);

    // Initialize
    if (!PyObject_Init(py, subtype))
    {
        subtype->tp_free(mem);
        return NULL;
    }

    // Construct
    try {
        self = new(mem) Cls(args, kwds ? kwds : Dict::empty());
    } catch (BaseException &e) {
        subtype->tp_free(mem);
        e.raise();
        return NULL;
    } catch (...) {
        subtype->tp_free(mem);
        throw;
    }

    // Return
    return self;
}

// ==== member wrapper ====
// Member methods
template <typename Cls>
template <Object (Cls::*fn)(const Tuple &)>
PyObject *CppType<Cls>::wrap_method(PyObject *self, PyObject *args)
{
    try_dispatch_member(fn, (args).steal()) catch_raise_return(NULL)
}

template <typename Cls>
template <Object (Cls::*fn)(const Tuple &, const Dict &)>
PyObject *CppType<Cls>::wrap_method(PyObject *self, PyObject *args, PyObject *kw)
{
    try_dispatch_member(fn, (args, kw ? kw : Dict::empty()).steal()) catch_raise_return(NULL)
}

template <typename Cls>
template <Object (Cls::*fn)(const Object &)>
PyObject *CppType<Cls>::wrap_method(PyObject *self, PyObject *object)
{
    try_dispatch_member(fn, (object).steal()) catch_raise_return(NULL)
}

template <typename Cls>
template <Object (Cls::*fn)(void)>
PyObject *CppType<Cls>::wrap_method(PyObject *self, PyObject *)
{
    try_dispatch_member(fn, ().steal()) catch_raise_return(NULL)
}

// Member methods -> None
template <typename Cls>
template <void (Cls::*fn)(const Tuple &)>
PyObject *CppType<Cls>::wrap_method(PyObject *self, PyObject *args)
{
    try_call_member_return_none(fn, (args)) catch_raise_return(NULL)
}

template <typename Cls>
template <void (Cls::*fn)(const Tuple &, const Dict &)>
PyObject *CppType<Cls>::wrap_method(PyObject *self, PyObject *args, PyObject *kw)
{
    try_call_member_return_none(fn, (args, kw ? kw : Dict::empty())) catch_raise_return(NULL)
}

template <typename Cls>
template <void (Cls::*fn)(const Object &)>
PyObject *CppType<Cls>::wrap_method(PyObject *self, PyObject *object)
{
    try_call_member_return_none(fn, (object)) catch_raise_return(NULL)
}

template <typename Cls>
template <void (Cls::*fn)(void)>
PyObject *CppType<Cls>::wrap_method(PyObject *self, PyObject *)
{
    try_call_member_return_none(fn, ()) catch_raise_return(NULL)
}

// Properties
template <typename Cls>
template <Object (Cls::*fn)(void *closure)>
PyObject *CppType<Cls>::wrap_getter(PyObject *self, void *closure)
{
    try_dispatch_member(fn, (closure).steal()) catch_raise_return(NULL)
}

template <typename Cls>
template <void (Cls::*fn)(const Object &value, void *closure)>
int CppType<Cls>::wrap_setter(PyObject *self, PyObject *value, void *closure)
{
    try_call_member_return(fn, (value, closure), 0) catch_raise_return(-1)
}

// ==== public ====
template <typename Cls>
void CppType<Cls>::ready()
{
    PyTypeObject *x = ptr();

    PyType_Ready(x);
}

template <typename Cls>
Type CppType<Cls>::get()
{
    return ptr();
}

template <typename Cls>
PyTypeObject *CppType<Cls>::ptr()
{
    if (!tp)
        tp = new CppType<Cls>();
    return tp;
}

} // namespace Py

#undef check_overridden
#undef if_overridden
#undef if_overridden_else
#undef try_dispatch_func
#undef try_call_func_return
#undef try_dispatch_member
#undef try_call_member_return
#undef try_call_member_return_none
#undef catch_raise_return
