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
    explicit BannerMessageWidget(QWidget *parent = nullptr);
    ~BannerMessageWidget() override;

    Q_REQUIRED_RESULT const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

private:
    void slotReadInfo();
    QString mIdentifier;
};
