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

namespace Py
{

class Object;
class Type;
class String;
class Tuple;
class Dict;

// =============== ObjectPtr ===============
/**
 * @brief The ObjectPtr class
 * Like \c Object, but also allows NULL
 */
class PYCPP_EXPORT ObjectPtr
{
private:
    PyObject *mp_object;

protected:
    void set(PyObject *object, bool steal = false);
    void unset();
    void invalid();

public:
    // ===== Construct =====
    ObjectPtr(PyObject *object = NULL, bool steal = false);
    ObjectPtr(const ObjectPtr &other);
    ObjectPtr(ObjectPtr &&other);

    // ===== Assign =====
    ObjectPtr &operator=(const ObjectPtr &rhs);
    ObjectPtr &operator=(ObjectPtr &&rhs);

    /// @brief Assign a new pointer to this object
    void assign(PyObject *object, bool steal = false);

    // ===== Destroy =====
    virtual ~ObjectPtr();

    // ===== References =====
    /// @brief Increase Refcount. Use with caution
    void incref();

    /// @brief Decrease Refcount. Use with caution
    void decref();

    /// @brief Retrieve Refcount. Not very useful
    Py_ssize_t refcnt() const;

    /// @brief Borrows the reference
    PyObject *ptr() const;

    /// @brief Steals the reference
    PyObject *steal();

    /// @brief Creates a new reference
    PyObject *newref() const;

    // ===== Type safety =====
    /// @brief Check If a PyObject fits into this C++ class
    static bool valid(PyObject *o);

    // ===== Attributes =====
    bool hasAttr(const char *attr) const;
    bool hasAttr(const std::string &attr) const;
    bool hasAttr(const Object &attr) const;

    Object getAttr(const char *attr) const;
    Object getAttr(const std::string &attr) const;
    Object getAttr(const Object &attr) const;

    void setAttr(const char *attr, const Object &value);
    void setAttr(const std::string &attr, const Object &value);
    void setAttr(const Object &attr, const Object &value);

    void delAttr(const char *attr);
    void delAttr(const std::string &attr);
    void delAttr(const Object &attr);

    // ===== Callables =====
    Object call();
    Object call(const Tuple &args);
    Object call(const Tuple &args, const Dict &kw);

    // ===== Methods =====
    Object callMethod(const char *method);
    Object callMethod(const char *method, const Tuple &args);
    Object callMethod(const char *method, const Tuple &args, const Dict &kw);

    // ===== Identity Check =====
    bool is(PyObject *other) const;
    bool is(const ObjectPtr &other) const;

    // ===== Protocol Checks =====
    bool isCallable() const;
    bool isSequence() const;
    bool isMapping() const;
    bool isNumber() const;

    // ====== Type Checks =====
    bool isNull() const;
    bool isNone() const;
    bool isLong() const;
    bool isFloat() const;
    bool isType() const;
    bool isString() const;
    bool isBytes() const;
    bool isBool() const;
    bool isTuple() const;
    bool isList() const;
    bool isDict() const;

    // ===== Sequence/Mapping protocol =====
    Object getItem(const Object &key) const;

    void setItem(const Object &key, const Object &value);

    void delItem(const Object &key);

    bool contains(const Object &key) const;

    // ===== Comparison =====
    bool richCompare(const Object &rhs, int op) const;
};

template <typename T>
using enable_if_non_object = typename std::enable_if<!std::is_base_of<ObjectPtr, typename std::remove_const<typename std::remove_pointer<T>::type>::type>::value, int>;

// =============== Object ===============
class PYCPP_EXPORT Object : public ObjectPtr
{
public:
    // Type
    static bool valid(PyObject *o);

    // ===== Construct =====
    // From pointer
    Object(PyObject *object, bool steal = false);
    explicit Object();

    // From other (upcast)
    Object(const ObjectPtr &other);
    Object(ObjectPtr &&other);

    /// @brief Construct an Object from value
    /// @param val A value that Py::Conv::newFromValue can convert.
    /// Only available on the actual Object class.
    template <typename T, typename enable_if_non_object<T>::type = 0>
    Object(T val);

    // Copy/Move
    Object(const Object &from);
    Object(Object &&from);

    // ===== Assign =====
    // From other (upcast)
    Object &operator=(const ObjectPtr &rhs);
    Object &operator=(ObjectPtr &&rhs);

    // From value
    template <typename T, typename enable_if_non_object<T>::type = 0>
    Object &operator=(T val);

    // Copy/Move
    Object &operator=(const Object &rhs);
    Object &operator=(Object &&rhs);

    // ===== Callables =====
    template <typename... Args>
    Object operator()(Args... targs);

    // ===== Methods =====
    // TODO(Taeyeon) Decide wether this is a good interface...
    template <typename... Args>
    Object m(const char *method, Args... targs);

    template <typename... Args>
    Object m(const std::string &method, Args... targs);

    // ===== Comparison =====
    bool operator==(const Object &rhs) const;
    bool operator!=(const Object &rhs) const;
    bool operator>=(const Object &rhs) const;
    bool operator<=(const Object &rhs) const;
    bool operator<(const Object &rhs) const;
    bool operator>(const Object &rhs) const;
};

} // namespace Py

#include "Object_impl.h"
