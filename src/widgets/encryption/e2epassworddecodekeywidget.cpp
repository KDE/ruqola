/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeywidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>
#include <kpasswordlineedit.h>

E2ePasswordDecodeKeyWidget::E2ePasswordDecodeKeyWidget(QWidget *parent)
    : QWidget{parent}
    , mPassword(new KPasswordLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mPassword->setObjectName(QStringLiteral("mPassword"));
    mainLayout->addWidget(mPassword);
}

E2ePasswordDecodeKeyWidget::~E2ePasswordDecodeKeyWidget() = default;

#include "moc_e2epassworddecodekeywidget.cpp"
