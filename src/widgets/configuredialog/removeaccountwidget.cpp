/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeaccountwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>

RemoveAccountWidget::RemoveAccountWidget(QWidget *parent)
    : QWidget{parent}
    , mRemoveLogs(new QCheckBox(i18n("Remove logs stored on disk"), this))
    , mInfo(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mInfo->setObjectName(QStringLiteral("mInfo"));
    mainLayout->addWidget(mInfo);

    mRemoveLogs->setObjectName(QStringLiteral("mRemoveLogs"));
    mainLayout->addWidget(mRemoveLogs);
}

RemoveAccountWidget::~RemoveAccountWidget() = default;

bool RemoveAccountWidget::removeLogs() const
{
    return mRemoveLogs->isChecked();
}

void RemoveAccountWidget::setAccountName(const QString &accountName)
{
    mInfo->setText(i18n("Do you want to remove this account \'%1\'?", accountName));
}

#include "moc_removeaccountwidget.cpp"
