/*
    SPDX-FileCopyrightText: 2015 Klarälvdalens Datakonsult AB a KDAB Group company, info@kdab.com
    Authors: David Faure <david.faure@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSignalSpy>

#include <QString>

namespace TestModelHelpers
{
inline QString rowSpyToText(const QSignalSpy &spy)
{
    using namespace Qt::Literals::StringLiterals;
    if (!spy.isValid()) {
        return u"THE SIGNALSPY IS INVALID!"_s;
    }
    QString str;
    for (int i = 0; i < spy.count(); ++i) {
        str += spy.at(i).at(1).toString() + u',' + spy.at(i).at(2).toString();
        if (i + 1 < spy.count()) {
            str += u';';
        }
    }
    return str;
}
}
