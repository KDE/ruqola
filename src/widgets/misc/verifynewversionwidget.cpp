/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "verifynewversionwidget.h"
#include "needupdateversion/needupdateversionutils.h"
#include <KLocalizedString>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>
using namespace Qt::Literals::StringLiterals;
VerifyNewVersionWidget::VerifyNewVersionWidget(QObject *parent)
    : QObject{parent}
{
}

VerifyNewVersionWidget::~VerifyNewVersionWidget() = default;

bool VerifyNewVersionWidget::canVerifyNewVersion() const
{
    return NeedUpdateVersionUtils::canVerifyNewVersion();
}

QAction *VerifyNewVersionWidget::verifyNewVersionAction()
{
    if (!mVerifyNewVersionAction) {
        // TODO add icon
        mVerifyNewVersionAction = new QAction(i18n("Check New Version"), this);
        connect(mVerifyNewVersionAction, &QAction::triggered, this, &VerifyNewVersionWidget::slotVerifyNewVersion);
    }
    return mVerifyNewVersionAction;
}

void VerifyNewVersionWidget::slotVerifyNewVersion()
{
    const QUrl url = NeedUpdateVersionUtils::newVersionUrl();
    if (!url.isEmpty()) {
        QDesktopServices::openUrl(url);
    }
}

#include "moc_verifynewversionwidget.cpp"
