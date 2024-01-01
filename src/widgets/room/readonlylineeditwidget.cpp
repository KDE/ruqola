/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "readonlylineeditwidget.h"
#include <QLabel>
#include <QVBoxLayout>
ReadOnlyLineEditWidget::ReadOnlyLineEditWidget(QWidget *parent)
    : QWidget(parent)
    , mLabel(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mLabel->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(mLabel);
    QFont font = mLabel->font();
    font.setBold(true);
    font.setItalic(true);
    mLabel->setFont(font);
}

ReadOnlyLineEditWidget::~ReadOnlyLineEditWidget() = default;

void ReadOnlyLineEditWidget::setMessage(const QString &str)
{
    mLabel->setText(str);
}

#include "moc_readonlylineeditwidget.cpp"
