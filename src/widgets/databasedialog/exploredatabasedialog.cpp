/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasedialog.h"
#include "databasedialog/exploredatabasejsonwidget.h"

#include "exploredatabasewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWindow>

using namespace Qt::Literals::StringLiterals;
namespace
{
const char myExploreDatabaseDialogConfigGroupName[] = "ExploreDatabaseDialog";
}
ExploreDatabaseDialog::ExploreDatabaseDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mExploreDatabaseWidget(new ExploreDatabaseWidget(account, this))
    , mExploreDatabaseJsonWidget(new ExploreDatabaseJsonWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Database Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    auto tabWidget = new QTabWidget(this);
    tabWidget->setObjectName(u"tabWidget"_s);
    mainLayout->addWidget(tabWidget);

    mExploreDatabaseWidget->setObjectName(u"mExploreDatabaseWidget"_s);
    tabWidget->addTab(mExploreDatabaseWidget, i18n("Messages"));

    mExploreDatabaseJsonWidget->setObjectName(u"mExploreDatabaseJsonWidget"_s);
    tabWidget->addTab(mExploreDatabaseJsonWidget, i18n("Json"));

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExploreDatabaseDialog::reject);

    connect(mExploreDatabaseWidget, &ExploreDatabaseWidget::messagesLoaded, mExploreDatabaseJsonWidget, &ExploreDatabaseJsonWidget::slotLoadedMessages);

    readConfig();
}

ExploreDatabaseDialog::~ExploreDatabaseDialog()
{
    writeConfig();
}

void ExploreDatabaseDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreDatabaseDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExploreDatabaseDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreDatabaseDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_exploredatabasedialog.cpp"
