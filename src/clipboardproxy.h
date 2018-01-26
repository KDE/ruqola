/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>
   based on code found https://stackoverflow.com/questions/40092352/passing-qclipboard-to-qml

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef CLIPBOARDPROXY_H
#define CLIPBOARDPROXY_H

#include <QObject>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT ClipboardProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ dataText WRITE setDataText NOTIFY dataChanged)
    Q_PROPERTY(QString selectionText READ selectionText WRITE setSelectionText NOTIFY selectionChanged)
public:
    explicit ClipboardProxy(QObject *parent = nullptr);
    ~ClipboardProxy();

    void setDataText(const QString &text);
    QString dataText() const;

    void setSelectionText(const QString &text);
    QString selectionText() const;

Q_SIGNALS:
    void dataChanged();
    void selectionChanged();
};

#endif // CLIPBOARDPROXY_H
