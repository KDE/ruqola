/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showthreadsdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "model/listmessagesfilterproxymodel.h"
#include "rocketchataccount.h"
#include "showtheadscombobox.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>

namespace
{
const char myShowThreadsDialogGroupName[] = "ShowThreadsDialog";
}
ShowThreadsDialog::ShowThreadsDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
    , mShowTheadsComboBox(new ShowTheadsComboBox(this))
{
    setWindowTitle(i18nc("@title:window", "Show Threads Messages"));
    readConfig();
    mShowTheadsComboBox->setObjectName(u"mShowTheadsComboBox"_s);
    addMessageLineWidget(mShowTheadsComboBox);
    connect(mShowTheadsComboBox, &QComboBox::activated, this, [this]() {
        const ShowTheadsComboBox::ThreadType type = mShowTheadsComboBox->currentThreadType();
        ListMessagesFilterProxyModel::FilteringByType filteringByType;
        switch (type) {
        case ShowTheadsComboBox::ThreadType::All:
            filteringByType = ListMessagesFilterProxyModel::FilteringByType::All;
            break;
        case ShowTheadsComboBox::ThreadType::Following:
            filteringByType = ListMessagesFilterProxyModel::FilteringByType::Following;
            break;
        case ShowTheadsComboBox::ThreadType::Unread:
            filteringByType = ListMessagesFilterProxyModel::FilteringByType::Unread;
            break;
        }
        setFilteringByType(filteringByType);
    });
}

ShowThreadsDialog::~ShowThreadsDialog()
{
    writeConfig();
}

void ShowThreadsDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowThreadsDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowThreadsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowThreadsDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_showthreadsdialog.cpp"
