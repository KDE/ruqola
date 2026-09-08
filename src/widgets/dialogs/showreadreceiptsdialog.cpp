/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showreadreceiptsdialog.h"
#include "showreadreceiptswidget.h"
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
const char myShowReadReceiptsDialogGroupName[] = "ShowReadReceiptsDialog";
}

using namespace Qt::Literals::StringLiterals;
ShowReadReceiptsDialog::ShowReadReceiptsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowReadReceiptsWidget(new ShowReadReceiptsWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Show Read Receipts"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowReadReceiptsWidget->setObjectName(u"mShowReadReceiptsWidget"_s);
    mainLayout->addWidget(mShowReadReceiptsWidget);

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
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myShowReadReceiptsDialogGroupName), QSize(800, 600));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myShowReadReceiptsDialogGroupName), 800, 600);
#endif
}

void ShowReadReceiptsDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowReadReceiptsDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

void ShowReadReceiptsDialog::setMessageId(const QByteArray &roomId)
{
    mShowReadReceiptsWidget->setMessageId(roomId);
}

#include "moc_showreadreceiptsdialog.cpp"
