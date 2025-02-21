/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "actionbutton.h"
#include "libruqolacore_export.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT ActionButtonsManager : public QObject
{
    Q_OBJECT
public:
    explicit ActionButtonsManager(QObject *parent = nullptr);
    ~ActionButtonsManager() override;

    [[nodiscard]] QList<ActionButton> actionButtons() const;
    void setActionButtons(const QList<ActionButton> &newActionButtons);

    void parseActionButtons(const QJsonArray &array);

Q_SIGNALS:
    void actionButtonsChanged();

private:
    QList<ActionButton> mActionButtons;
};
