/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] const QList<BannerInfos::UnreadInformation> &bannerInfos() const;
    void setBannerInfos(const QList<BannerInfos::UnreadInformation> &newBannerInfo);

Q_SIGNALS:
    void infoWasRead(const QString &identifier);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotReadInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenUrl(const QString &url);
    QList<BannerInfos::UnreadInformation> mBannerInfos;
};
