/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannermessagewidget.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QAction>

BannerMessageWidget::BannerMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    setMessageType(Information);
    setWordWrap(true);

    auto readAction = new QAction(i18n("Read"), this);
    readAction->setObjectName(QStringLiteral("read_action"));
    connect(readAction, &QAction::triggered, this, &BannerMessageWidget::slotReadInfo);
    addAction(readAction);
}

BannerMessageWidget::~BannerMessageWidget() = default;

void BannerMessageWidget::slotReadInfo()
{
    if (mBannerInfo.identifier.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << " identifier is empty. It's a bug";
    } else {
        Q_EMIT infoWasRead(mBannerInfo.identifier);
        // TODO emit signal for inform server that we read identifier
        animatedHide();
    }
}

const BannerInfos::UnreadInformation &BannerMessageWidget::bannerInfo() const
{
    return mBannerInfo;
}

void BannerMessageWidget::setBannerInfo(const BannerInfos::UnreadInformation &newBannerInfo)
{
    if (mBannerInfo != newBannerInfo) {
        mBannerInfo = newBannerInfo;
        animatedShow();
    }
}
