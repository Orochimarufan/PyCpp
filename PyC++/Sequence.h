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
#include "Long.h"
#include "Iterator.h"

namespace Py
{

template <typename C, typename E>
class PYCPP_EXPORT CollectionView
{
private:
    C &collection;
    Object index;
    E item;

public:
    CollectionView(C &collection, const Object &index, const E &item)
        : collection(collection), index(index), item(item)
    {
    }

    CollectionView(C &collection, const Object &index)
        : collection(collection), index(index), item(collection.getItem(index))
    {
    }

    E *operator->() const
    {
        return &item;
    }

    CollectionView<C, E> &operator=(const E &item)
    {
        collection.setItem(index, item);
        this->item = item;
    }

    operator const E &() const
    {
        return item;
    }

    operator E &()
    {
        return item;
    }
};

template <typename E>
class PYCPP_EXPORT SequenceT : public Object
{
public:
    typedef Py_ssize_t size_type;
    typedef CollectionView<SequenceT<E>, E> reference;
    typedef const CollectionView<SequenceT<E>, E> const_reference;
    typedef CollectionView<SequenceT<E>, E> *pointer;
    typedef int difference_type;
    typedef E value_type;

    PYCPP_OBJECT_INLINE_VALID(PySequence_Check)
    PYCPP_OBJECT_DEF_DEFAULTS(SequenceT<E>)

    // ===== Size =====
    inline size_type size() const
    {
        return PySequence_Length(ptr());
    }

    inline size_type length() const
    {
        return PySequence_Length(ptr());
    }

    // ===== Access =====
    inline const_reference operator[](const Object &index) const
    {
        return const_reference(*this, index);
    }

    inline reference operator[](const Object &index)
    {
        return reference(*this, index);
    }

    inline const_reference operator[](size_type index) const
    {
        return const_reference(*this, Long(index));
    }

    inline reference operator[](size_type index)
    {
        return reference(*this, Long(index));
    }

    // ===== Repeat & concat =====
    SequenceT<E> repeat(int count) const
    {
        return SequenceT<E>(PySequence_Repeat(ptr(), count), true);
    }

    SequenceT<E> concat(const SequenceT<E> &other)
    {
        return SequenceT<E>(PySequence_Concat(ptr(), other), true);
    }

    // ===== Iterator =====
    typedef IteratorT<E> iterator;

    iterator begin() const
    {
        return iterator::begin(*this);
    }

    iterator end() const
    {
        return iterator::end(*this);
    }
};

PYCPP_OBJECT_IMPL_DEFAULTS_TEMPLATE(SequenceT, Object, template <typename E> inline, <E>)

typedef SequenceT<Object> Sequence;
}
