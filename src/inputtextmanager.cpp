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

#include "inputtextmanager.h"
#include "rocketchataccount.h"
#include "model/inputcompletermodel.h"

InputTextManager::InputTextManager(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
    mInputCompleterModel = new InputCompleterModel(this);
}

InputTextManager::~InputTextManager()
{
}

QString InputTextManager::replaceWord(const QString &newWord, const QString &str, int position)
{
    return str;
}

void InputTextManager::setInputTextChanged(const QString &text, int position)
{
    if (text.isEmpty()) {
        clear();
        return;
    }
    if (mAccount) {
        QString word; //TODO search it.
        int start = 0;
        for (int i = position; i >= 0; i--) {
            if (!text.at(i).isSpace()) {
                continue;
            }
            start = i + 1;
            break;
        }
        int end = text.length() - 1;
        for (int i = position; i < text.length(); i++) {
            if (!text.at(i).isSpace()) {
                continue;
            }
            end = i;
            break;
        }

        word = text.mid(start, end - start + 1);
        qDebug() << " text " << text << " position : " << position << " word: "<<word;
        //TODO check last word
        const QString str = word.right(word.length()-1);
        qDebug() << str;
        if (!word.isEmpty()) {
            if (word.startsWith(QLatin1Char('@'))) {
                //FIXME word without @ ? and exception!
                mAccount->inputUserAutocomplete(str, QString());
            } else if (word.startsWith(QLatin1Char('#'))) {
                //FIXME word without @ ? and exception!
                mAccount->inputChannelAutocomplete(str, QString());
            }
        } else {
            clear();
        }
    }
}

void InputTextManager::clear()
{
    mInputCompleterModel->setStringList(QStringList());
}

InputCompleterModel *InputTextManager::inputCompleterModel() const
{
    return mInputCompleterModel;
}

void InputTextManager::setCompleterList(const QStringList &lst)
{
    mInputCompleterModel->setStringList(lst);
}

void InputTextManager::inputTextCompleter(const QJsonObject &obj)
{
    mInputCompleterModel->parseInputTextCompleter(obj);
}
