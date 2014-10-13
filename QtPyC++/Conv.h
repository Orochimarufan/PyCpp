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

#include "QtPyC++Config.h"

#include <PyC++/types.h>

#include <QtCore/QVariant>

namespace Py
{
namespace Qt
{
namespace Conv
{

// To python
QTPYCPP_EXPORT Object toPython(const QVariant &variant);

QTPYCPP_EXPORT Tuple toTuple(const QStringList &list);
QTPYCPP_EXPORT Tuple toTuple(const QVariantList &list);

QTPYCPP_EXPORT Dict toDict(const QVariantHash &map);
QTPYCPP_EXPORT Dict toDict(const QVariantMap &map);

QTPYCPP_EXPORT String toString(const QString &string);

// To Qt
QTPYCPP_EXPORT QVariant toQt(const Object &o);

QTPYCPP_EXPORT QString toQString(const String &str);
QTPYCPP_EXPORT QByteArray toQByteArray(const Bytes &b);

QTPYCPP_EXPORT QStringList toQStringList(const Sequence &t, bool strict=true);
QTPYCPP_EXPORT QVariantList toQVariantList(const Sequence &t);

QTPYCPP_EXPORT QVariantHash toQVariantHash(const Dict &d, bool strict=true);
QTPYCPP_EXPORT QVariantMap toQVariantMap(const Dict &d, bool strict=true);

} // namespace Conv
} // namespace Qt

namespace Conv {
template <>
QTPYCPP_EXPORT PyObject *newFromValue(QString value)
{
    QByteArray utf = value.toUtf8();
    return PyUnicode_FromStringAndSize(utf.constData(), utf.size());
}
}
} // namespace Py
