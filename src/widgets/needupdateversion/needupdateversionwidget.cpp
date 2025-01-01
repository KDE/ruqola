/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
  code based on kdenlive
*/

#include "needupdateversionwidget.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>
using namespace Qt::Literals::StringLiterals;
NeedUpdateVersionWidget::NeedUpdateVersionWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setPosition(KMessageWidget::Header);
    setCloseButtonVisible(true);
    setTextFormat(Qt::PlainText);
    auto action = new QAction(i18nc("@action", "Disable version check"), this);
    addAction(action);
    connect(action, &QAction::triggered, this, &NeedUpdateVersionWidget::slotDisableVersionCheck);
    if (NeedUpdateVersionUtils::canVerifyNewVersion()) {
        action = new QAction(i18nc("@action", "Check new Version"), this);
        addAction(action);
        connect(action, &QAction::triggered, this, &NeedUpdateVersionWidget::slotCheckNewVersion);
    }
}

NeedUpdateVersionWidget::~NeedUpdateVersionWidget() = default;

void NeedUpdateVersionWidget::setObsoleteVersion(NeedUpdateVersionUtils::ObsoleteVersion obsolete)
{
    switch (obsolete) {
    case NeedUpdateVersionUtils::ObsoleteVersion::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "Unknown value: seems to be a bug";
        break;
    case NeedUpdateVersionUtils::ObsoleteVersion::OlderThan6Months:
        setMessageType(KMessageWidget::Information);
        setText(i18n("Your version is older than 6 months, we encourage you to upgrade."));
        animatedShow();
        break;
    case NeedUpdateVersionUtils::ObsoleteVersion::OlderThan12Months:
        setMessageType(KMessageWidget::Warning);
        setText(i18n("Your version is older than 1 year, we strongly encourage you to upgrade."));
        animatedShow();
        break;
    case NeedUpdateVersionUtils::ObsoleteVersion::NotObsoleteYet:
        // It's ok
        break;
    }
}

void NeedUpdateVersionWidget::slotDisableVersionCheck()
{
    NeedUpdateVersionUtils::disableCheckVersion();
    animatedHide();
}

void NeedUpdateVersionWidget::slotCheckNewVersion()
{
    const QUrl url = NeedUpdateVersionUtils::newVersionUrl();
    if (!url.isEmpty()) {
        QDesktopServices::openUrl(url);
        animatedHide();
    }
}

#include "moc_needupdateversionwidget.cpp"
