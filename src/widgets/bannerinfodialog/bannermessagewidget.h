/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerMessageWidget : public KMessageWidget
{
    Q_OBJECT
public:
    struct BannerInfo {
        QString identifier;
        QString i18nMessage;
        Q_REQUIRED_RESULT bool operator==(const BannerInfo &other) const
        {
            return identifier == other.identifier && i18nMessage == other.i18nMessage;
        }
        Q_REQUIRED_RESULT bool operator!=(const BannerInfo &other) const
        {
            return !operator==(other);
        }
    };

    explicit BannerMessageWidget(QWidget *parent = nullptr);
    ~BannerMessageWidget() override;

    Q_REQUIRED_RESULT const BannerInfo &bannerInfo() const;
    void setBannerInfo(const BannerInfo &newBannerInfo);

Q_SIGNALS:
    void infoWasRead(const QString &identifier);

private:
    void slotReadInfo();
    BannerInfo mBannerInfo;
};
