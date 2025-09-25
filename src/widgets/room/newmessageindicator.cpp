/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "newmessageindicator.h"
#include <KLocalizedString>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
NewMessageIndicator::NewMessageIndicator(QWidget *parent)
    : QWidget{parent}
{
    setObjectName(u"NewMessageIndicator"_s);
    setFocusPolicy(Qt::NoFocus);
    auto mainLayout = new QVBoxLayout(this);
}

NewMessageIndicator::~NewMessageIndicator() = default;

#include "moc_newmessageindicator.cpp"
