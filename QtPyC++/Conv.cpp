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

#include "Conv.h"

#include <PyC++/PyC++.h>

namespace Py {
namespace Qt {
namespace Conv {

// To Python


// To Qt
QVariant toQt(const Object &o)
{
    if (o.isBool())
        return QVariant::fromValue(o == True);
    if (o.isBytes())
        return toQByteArray(o);
    if (o.isDict())
        return toQVariantHash(o);
    if (o.isFloat())
        return QVariant::fromValue(PyFloat_AsDouble(o.ptr()));
    if (o.isList())
        return toQVariantList(o);
    if (o.isLong())
        return QVariant::fromValue(PyLong_AsLong(o.ptr()));
    if (o.isNone())
        return QVariant();
    if (o.isString())
        return toQString(o);
    if (o.isTuple())
        return toQVariantList(o);

    throw TypeError("Cannot convert python type.");
}

QString toQString(const String &str)
{
    return QString::fromUtf8(str.asUTF8(), len(str));
}

QStringList toQStringList(const Sequence &seq, bool strict)
{
    QStringList sl;

    if (strict)
        for (String s : seq)
            sl << toQString(s);
    else
        for (Object o : seq)
            sl << toQString(str(o));

    return sl;
}

QVariantList toQVariantList(const Sequence &seq)
{
    QVariantList vl;

    for (Object o : seq)
        vl << toQt(o);

    return vl;
}

QVariantHash toQVariantHash(const Dict &d, bool strict)
{

}

QByteArray toQByteArray(const Bytes &b)
{
}

} // namespace Conv
} // namespace Qt
} // namespace Py
