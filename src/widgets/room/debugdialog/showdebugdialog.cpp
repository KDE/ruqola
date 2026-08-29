/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showdebugdialog.h"
#include "showdebugwidget.h"
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
const char myShowDebugDialogGroupName[] = "ShowDebugDialog";
}
using namespace Qt::Literals::StringLiterals;
ShowDebugDialog::ShowDebugDialog(QWidget *parent)
    : QDialog(parent)
    , mShowDebugWidget(new ShowDebugWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Debug"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowDebugWidget->setObjectName(u"mShowDebugWidget"_s);
    mainLayout->addWidget(mShowDebugWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ShowDebugDialog::reject);
    readConfig();
}

ShowDebugDialog::~ShowDebugDialog()
{
    writeConfig();
}

void ShowDebugDialog::setPlainText(const QString &text)
{
    mShowDebugWidget->setPlainText(text);
}

void ShowDebugDialog::readConfig()
{
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myShowDebugDialogGroupName), 400, 300);
}

void ShowDebugDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowDebugDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_showdebugdialog.cpp"
