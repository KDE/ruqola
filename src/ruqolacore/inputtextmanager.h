/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#ifndef INPUTTEXTMANAGER_H
#define INPUTTEXTMANAGER_H

#include <QJsonObject>
#include <QObject>

#include "libruqola_private_export.h"
class InputCompleterModel;
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT InputTextManager : public QObject
{
    Q_OBJECT
public:
    explicit InputTextManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~InputTextManager();

    void setInputText(const QString &word);

    Q_REQUIRED_RESULT InputCompleterModel *inputCompleterModel() const;

    void inputTextCompleter(const QJsonObject &obj);

    Q_REQUIRED_RESULT QString replaceWord(const QString &newWord, const QString &str, int position);

    void setInputTextChanged(const QString &str, int position);

    void clearCompleter();

    //Only for autotests
    Q_REQUIRED_RESULT QString searchWord(const QString &text, int position);
private:
    Q_DISABLE_COPY(InputTextManager)
    InputCompleterModel *mInputCompleterModel = nullptr;
    RocketChatAccount *mAccount = nullptr;
};

#endif // INPUTTEXTMANAGER_H
