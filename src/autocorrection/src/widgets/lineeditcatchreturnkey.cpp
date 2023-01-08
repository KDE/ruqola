/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "lineeditcatchreturnkey.h"

#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
using namespace TextAutoCorrection;

LineEditCatchReturnKey::LineEditCatchReturnKey(QLineEdit *lineEdit, QObject *parent)
    : QObject(parent)
    , mLineEdit(lineEdit)
{
    mLineEdit->installEventFilter(this);
}

LineEditCatchReturnKey::~LineEditCatchReturnKey() = default;

bool LineEditCatchReturnKey::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == mLineEdit) {
        if (event->type() == QEvent::KeyPress) {
            auto e = static_cast<QKeyEvent *>(event);
            if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
                const bool stopEvent = (e->modifiers() == Qt::NoButton || e->modifiers() == Qt::KeypadModifier);
                if (stopEvent) {
                    Q_EMIT mLineEdit->returnPressed();
                }
                return true;
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
