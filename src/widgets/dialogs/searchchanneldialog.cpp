/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchchanneldialog.h"
#include "searchchannelwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char mySearchChannelDialogGroupName[] = "SearchChannelDialog";
}
SearchChannelDialog::SearchChannelDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mSearchChannelWidget(new SearchChannelWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Search Channel"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchChannelWidget->setObjectName(QStringLiteral("mSearchChannelWidget"));
    mainLayout->addWidget(mSearchChannelWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);

    connect(button, &QDialogButtonBox::rejected, this, &SearchChannelDialog::reject);
    readConfig();
}

SearchChannelDialog::~SearchChannelDialog()
{
    writeConfig();
}

void SearchChannelDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), mySearchChannelDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SearchChannelDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), mySearchChannelDialogGroupName);
    group.writeEntry("Size", size());
}
