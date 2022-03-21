/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizewidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

MessageMaximumSizeWidget::MessageMaximumSizeWidget(QWidget *parent)
    : QWidget{parent}
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mFileName->setObjectName(QStringLiteral("mFileName"));
    mainLayout->addWidget(mFileName);

    mDescription->setObjectName(QStringLiteral("mDescription"));
    mainLayout->addWidget(mDescription);
}

MessageMaximumSizeWidget::~MessageMaximumSizeWidget()
{
}
