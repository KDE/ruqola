/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasejsonwidget.h"
#include <QVBoxLayout>
#include <qplaintextedit.h>
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseJsonWidget::ExploreDatabaseJsonWidget(QWidget *parent)
    : QWidget{parent}
    , mPlainTextEdit(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mPlainTextEdit->setObjectName(u"mPlainTextEdit"_s);
    mPlainTextEdit->setReadOnly(true);
    mainLayout->addWidget(mPlainTextEdit);
}

ExploreDatabaseJsonWidget::~ExploreDatabaseJsonWidget() = default;

#include "moc_exploredatabasejsonwidget.cpp"
