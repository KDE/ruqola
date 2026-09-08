/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showthreadsdialog.h"

#include "model/listmessagesfilterproxymodel.h"
#include "rocketchataccount.h"
#include "showtheadscombobox.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myShowThreadsDialogGroupName[] = "ShowThreadsDialog";
}
using namespace Qt::Literals::StringLiterals;
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
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myShowThreadsDialogGroupName), QSize(800, 600));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myShowThreadsDialogGroupName), 800, 600);
#endif
}

void ShowThreadsDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowThreadsDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

#include "moc_showthreadsdialog.cpp"
