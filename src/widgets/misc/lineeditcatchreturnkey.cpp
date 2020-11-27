/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "lineeditcatchreturnkey.h"

#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>

LineEditCatchReturnKey::LineEditCatchReturnKey(QLineEdit *lineEdit, QObject *parent)
    : QObject(parent)
    , mLineEdit(lineEdit)
{
    mLineEdit->installEventFilter(this);
}

LineEditCatchReturnKey::~LineEditCatchReturnKey()
{
}

bool LineEditCatchReturnKey::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == mLineEdit) {
        if (event->type() == QEvent::KeyPress) {
            auto e = static_cast<QKeyEvent *>(event);
            if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
                const bool stopEvent = (e->modifiers() == Qt::NoButton
                                        || e->modifiers() == Qt::KeypadModifier);
                if (stopEvent) {
                    Q_EMIT mLineEdit->returnPressed();
                }
                return true;
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
