/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "bannerinfo/bannerinfos.h"
#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerMessageWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit BannerMessageWidget(QWidget *parent = nullptr);
    ~BannerMessageWidget() override;

    Q_REQUIRED_RESULT const QVector<BannerInfos::UnreadInformation> &bannerInfos() const;
    void setBannerInfos(const QVector<BannerInfos::UnreadInformation> &newBannerInfo);

Q_SIGNALS:
    void infoWasRead(const QString &identifier);

private:
    void slotReadInfo();
    void updateInfo();
    void slotOpenUrl(const QString &url);
    QVector<BannerInfos::UnreadInformation> mBannerInfos;
};
