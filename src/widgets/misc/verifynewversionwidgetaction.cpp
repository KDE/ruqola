/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "verifynewversionwidgetaction.h"
#include "needupdateversion/needupdateversionutils.h"
#include "verifynewversionwidget/verifynewversiondialog.h"
#include <KLocalizedString>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>
using namespace Qt::Literals::StringLiterals;
VerifyNewVersionWidgetAction::VerifyNewVersionWidgetAction(QObject *parent)
    : QObject{parent}
{
}

VerifyNewVersionWidgetAction::~VerifyNewVersionWidgetAction() = default;

bool VerifyNewVersionWidgetAction::canVerifyNewVersion() const
{
    return NeedUpdateVersionUtils::canVerifyNewVersion();
}

QAction *VerifyNewVersionWidgetAction::verifyNewVersionAction()
{
    if (!mVerifyNewVersionAction) {
        // TODO add icon
        mVerifyNewVersionAction = new QAction(i18n("Check New Version"), this);
        connect(mVerifyNewVersionAction, &QAction::triggered, this, &VerifyNewVersionWidgetAction::slotVerifyNewVersion);
    }
    return mVerifyNewVersionAction;
}

void VerifyNewVersionWidgetAction::slotVerifyNewVersion()
{
    // TODO add parent
    VerifyNewVersionDialog dlg(nullptr);
    dlg.checkNewVersion();
    dlg.exec();
}

#include "moc_verifynewversionwidgetaction.cpp"
