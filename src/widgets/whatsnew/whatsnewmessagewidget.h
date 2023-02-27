/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <KMessageWidget>

class WhatsNewMessageWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit WhatsNewMessageWidget(QWidget *parent = nullptr);
    ~WhatsNewMessageWidget() override;

private:
    void slotLinkActivated(const QString &contents);
};
