/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannermessagewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqolautils.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QAction>
#include <QUrl>

BannerMessageWidget::BannerMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setWordWrap(true);
    setPosition(KMessageWidget::Header);

    auto readAction = new QAction(i18nc("@action", "Mark as Read"), this);
    readAction->setObjectName(u"read_action"_s);
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

const QList<BannerInfos::UnreadInformation> &BannerMessageWidget::bannerInfos() const
{
    return mBannerInfos;
}

void BannerMessageWidget::setBannerInfos(const QList<BannerInfos::UnreadInformation> &newBannerInfo)
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
    RuqolaUtils::self()->openUrl(QUrl(url));
}

#include "moc_bannermessagewidget.cpp"
