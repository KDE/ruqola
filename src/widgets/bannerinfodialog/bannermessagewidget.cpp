/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannermessagewidget.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>

BannerMessageWidget::BannerMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setWordWrap(true);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    setPosition(KMessageWidget::Header);
#endif

    auto readAction = new QAction(i18n("Mark as Read"), this);
    readAction->setObjectName(QStringLiteral("read_action"));
    connect(readAction, &QAction::triggered, this, &BannerMessageWidget::slotReadInfo);
    addAction(readAction);
    connect(this, &BannerMessageWidget::linkActivated, this, &BannerMessageWidget::slotOpenUrl);
}

BannerMessageWidget::~BannerMessageWidget() = default;

void BannerMessageWidget::slotReadInfo()
{
    if (mBannerInfos.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "banner info is empty! It's a bug";
    } else {
        const auto info = mBannerInfos.takeFirst();

        Q_EMIT infoWasRead(info.identifier);
        updateInfo();
    }
}

const QVector<BannerInfos::UnreadInformation> &BannerMessageWidget::bannerInfos() const
{
    return mBannerInfos;
}

void BannerMessageWidget::setBannerInfos(const QVector<BannerInfos::UnreadInformation> &newBannerInfo)
{
    if (mBannerInfos != newBannerInfo) {
        mBannerInfos = newBannerInfo;
        updateInfo();
    }
}

void BannerMessageWidget::updateInfo()
{
    if (mBannerInfos.isEmpty()) {
        animatedHide();
    } else {
        const auto info = mBannerInfos.first();
        setText(info.i18nMessage);
        animatedShow();
    }
}

void BannerMessageWidget::slotOpenUrl(const QString &url)
{
    QDesktopServices::openUrl(QUrl(url));
}

#include "moc_bannermessagewidget.cpp"
