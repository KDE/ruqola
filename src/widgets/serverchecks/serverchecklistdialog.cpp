/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverchecklistdialog.h"

#include "serverchecklistmanager.h"

#include "rocketchataccount.h"
#include "ruqolaserverconfig.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWindow>

using namespace Qt::Literals::StringLiterals;

namespace
{
const char myServerCheckListDialogConfigGroupName[] = "ServerCheckListDialog";
}

ServerCheckListDialog::ServerCheckListDialog(RocketChatAccount *account, const QByteArray &roomId, QWidget *parent)
    : QDialog(parent)
    , mManager(new ServerCheckListManager(account, roomId, this))
    , mTreeWidget(new QTreeWidget(this))
    , mAccount(account)
    , mRoomId(roomId)
{
    setWindowTitle(i18nc("@title:window", "Server Checklist"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    auto explanation = new QLabel(i18n("Runs a series of checks against the current server and reports whether each feature "
                                       "behaves as expected. Temporary test messages are sent to the selected channel and "
                                       "removed afterwards, so pick a scratch channel first."),
                                  this);
    explanation->setObjectName(u"explanation"_s);
    explanation->setWordWrap(true);
    mainLayout->addWidget(explanation);

    mTreeWidget->setObjectName(u"mTreeWidget"_s);
    mTreeWidget->setRootIsDecorated(false);
    mTreeWidget->setColumnCount(2);
    mTreeWidget->setHeaderLabels({i18nc("@title:column", "Check"), i18nc("@title:column", "Result")});
    mTreeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    mainLayout->addWidget(mTreeWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    mRunButton = buttonBox->addButton(i18nc("@action:button", "Run Checks"), QDialogButtonBox::ActionRole);
    mRunButton->setObjectName(u"mRunButton"_s);
    mRunButton->setIcon(QIcon::fromTheme(u"system-run"_s));
    connect(mRunButton, &QPushButton::clicked, this, &ServerCheckListDialog::slotRunChecks);

    mPostButton = buttonBox->addButton(i18nc("@action:button", "Post Results to Channel"), QDialogButtonBox::ActionRole);
    mPostButton->setObjectName(u"mPostButton"_s);
    mPostButton->setIcon(QIcon::fromTheme(u"document-send"_s));
    mPostButton->setToolTip(i18nc("@info:tooltip", "Post the checklist results as a message in the current channel."));
    mPostButton->setEnabled(false);
    connect(mPostButton, &QPushButton::clicked, this, &ServerCheckListDialog::slotPostResults);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &ServerCheckListDialog::reject);
    mainLayout->addWidget(buttonBox);

    connect(mManager, &ServerCheckListManager::checkStarted, this, &ServerCheckListDialog::slotCheckStarted);
    connect(mManager, &ServerCheckListManager::checkFinished, this, &ServerCheckListDialog::slotCheckFinished);
    connect(mManager, &ServerCheckListManager::allFinished, this, &ServerCheckListDialog::slotAllFinished);

    populateChecks();

    if (roomId.isEmpty()) {
        explanation->setText(i18n("Open a channel first: the checks need a room to send their test messages to."));
        mRunButton->setEnabled(false);
    }
    if (mAccount) {
        auto room = mAccount->room(roomId);
        if (room->channelType() == Room::RoomType::Direct) {
            explanation->setText(i18n("Check can not executed in direct channel."));
            mRunButton->setEnabled(false);
        }
    }
    readConfig();
}

ServerCheckListDialog::~ServerCheckListDialog()
{
    writeConfig();
}

void ServerCheckListDialog::populateChecks()
{
    const int count = mManager->checkCount();
    mItems.reserve(count);
    mResults = QList<bool>(count, false);
    for (int i = 0; i < count; ++i) {
        auto item = new QTreeWidgetItem(mTreeWidget);
        item->setText(0, mManager->checkName(i));
        item->setText(1, i18nc("@info:status not run yet", "Not run"));
        mItems.append(item);
    }
}

void ServerCheckListDialog::slotRunChecks()
{
    mRunButton->setEnabled(false);
    mPostButton->setEnabled(false);
    mResults.fill(false);
    for (auto item : std::as_const(mItems)) {
        item->setIcon(1, {});
        item->setText(1, i18nc("@info:status", "Waiting…"));
        item->setToolTip(1, {});
    }
    mManager->start();
}

void ServerCheckListDialog::slotCheckStarted(int index)
{
    auto item = mItems.at(index);
    item->setIcon(1, QIcon::fromTheme(u"chronometer"_s));
    item->setText(1, i18nc("@info:status", "Running…"));
}

void ServerCheckListDialog::slotCheckFinished(int index, bool passed, const QString &details)
{
    mResults[index] = passed;
    auto item = mItems.at(index);
    if (passed) {
        item->setIcon(1, QIcon::fromTheme(u"dialog-ok-apply"_s));
        item->setText(1, i18nc("@info:status", "OK"));
    } else {
        item->setIcon(1, QIcon::fromTheme(u"dialog-error"_s));
        item->setText(1, details.isEmpty() ? i18nc("@info:status", "Failed") : i18nc("@info:status", "Failed: %1", details));
    }
    item->setToolTip(1, details);
}

void ServerCheckListDialog::slotAllFinished()
{
    mRunButton->setEnabled(true);
    mPostButton->setEnabled(!mRoomId.isEmpty() && mAccount);
}

QString ServerCheckListDialog::buildResultMessage() const
{
    QString version;
    if (mAccount) {
        version = mAccount->ruqolaServerConfig()->serverVersion();
    }
    if (version.isEmpty()) {
        version = i18nc("Unknown server version", "unknown");
    }

    QString message = i18n("RC %1\nAutomated check list from ruqola:\n", version);
    const int count = mManager->checkCount();
    for (int i = 0; i < count; ++i) {
        // RC markdown renders these emoji shortcodes as ✅ / ❌.
        const QString status = mResults.at(i) ? u":white_check_mark:"_s : u":x:"_s;
        message += u"- %1 %2\n"_s.arg(mManager->checkName(i), status);
    }
    return message;
}

void ServerCheckListDialog::slotPostResults()
{
    if (!mAccount || mRoomId.isEmpty()) {
        return;
    }
    mAccount->sendMessage(mRoomId, buildResultMessage());
    mPostButton->setEnabled(false);
}

void ServerCheckListDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 350));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myServerCheckListDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ServerCheckListDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myServerCheckListDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_serverchecklistdialog.cpp"
