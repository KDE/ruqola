/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showdebugwidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
ShowDebugWidget::ShowDebugWidget(QWidget *parent)
    : QWidget{parent}
    , mJsonPlainTextEditWidget(new JsonPlainTextEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mJsonPlainTextEditWidget->setObjectName(u"mJsonPlainTextEditWidget"_s);
    mainLayout->addWidget(mJsonPlainTextEditWidget);
}

ShowDebugWidget::~ShowDebugWidget() = default;

void ShowDebugWidget::setPlainText(const QString &text)
{
    mJsonPlainTextEditWidget->setPlainText(text);
}

#include "moc_showdebugwidget.cpp"
