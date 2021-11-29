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
    if (!spy.isValid()) {
        return QStringLiteral("THE SIGNALSPY IS INVALID!");
    }
    QString str;
    for (int i = 0; i < spy.count(); ++i) {
        str += spy.at(i).at(1).toString() + QLatin1Char(',') + spy.at(i).at(2).toString();
        if (i + 1 < spy.count()) {
            str += QLatin1Char(';');
        }
    }
    return str;
}
}

