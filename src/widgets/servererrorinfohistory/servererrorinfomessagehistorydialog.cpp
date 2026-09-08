/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorydialog.h"

#include "servererrorinfomessagehistorywidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myServerErrorInfoMessageHistoryDialogConfigGroupName[] = "ServerErrorInfoMessageHistoryDialog";
}
using namespace Qt::Literals::StringLiterals;
ServerErrorInfoMessageHistoryDialog::ServerErrorInfoMessageHistoryDialog(QWidget *parent)
    : QDialog(parent)
    , mServerErrorInfoMessageHistoryWidget(new ServerErrorInfoMessageHistoryWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Server Error Information"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mServerErrorInfoMessageHistoryWidget->setObjectName(u"mServerErrorInfoMessageHistoryWidget"_s);
    mainLayout->addWidget(mServerErrorInfoMessageHistoryWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ServerErrorInfoMessageHistoryDialog::reject);
    readConfig();
}

ServerErrorInfoMessageHistoryDialog::~ServerErrorInfoMessageHistoryDialog()
{
    writeConfig();
}

void ServerErrorInfoMessageHistoryDialog::addServerList(const QList<AccountManager::AccountDisplayInfo> &infos)
{
    mServerErrorInfoMessageHistoryWidget->addServerList(infos);
}

void ServerErrorInfoMessageHistoryDialog::readConfig()
{
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myServerErrorInfoMessageHistoryDialogConfigGroupName), QSize(400, 300));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myServerErrorInfoMessageHistoryDialogConfigGroupName), 400, 300);
#endif
}

void ServerErrorInfoMessageHistoryDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myServerErrorInfoMessageHistoryDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

#include "moc_servererrorinfomessagehistorydialog.cpp"
