/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSharedData>
#include <QString>

class MessageExtra : public QSharedData
{
public:
    MessageExtra();

    [[nodiscard]] QString localTranslation() const;
    void setLocalTranslation(const QString &newLocalTranslation);

private:
    // Local translation
    QString mLocalTranslation;
};
