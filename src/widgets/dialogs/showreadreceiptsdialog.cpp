/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showreadreceiptsdialog.h"
#include "rocketchataccount.h"
#include "showreadreceiptswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myShowReadReceiptsDialogGroupName[] = "ShowReadReceiptsDialog";
}

using namespace Qt::Literals::StringLiterals;
ShowReadReceiptsDialog::ShowReadReceiptsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowReadReceiptsWidget(new ShowReadReceiptsWidget(account, this))
    , mRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Show Read Receipts"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowReadReceiptsWidget->setObjectName(u"mShowReadReceiptsWidget"_s);
    mainLayout->addWidget(mShowReadReceiptsWidget);

    setAttribute(Qt::WA_DeleteOnClose);
    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ShowReadReceiptsDialog::reject);

    readConfig();
}

ShowReadReceiptsDialog::~ShowReadReceiptsDialog()
{
    writeConfig();
}

void ShowReadReceiptsDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowReadReceiptsDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowReadReceiptsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowReadReceiptsDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ShowReadReceiptsDialog::setMessageId(const QByteArray &roomId)
{
    mShowReadReceiptsWidget->setMessageId(roomId);
}

#include "moc_showreadreceiptsdialog.cpp"
