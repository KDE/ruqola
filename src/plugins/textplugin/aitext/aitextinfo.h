/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QDebug>
class AiTextInfo
{
public:
    AiTextInfo();
    ~AiTextInfo();

    [[nodiscard]] bool enabled() const;
    void setEnabled(bool newEnabled);

    [[nodiscard]] QString requestText() const;
    void setRequestText(const QString &newRequestText);

private:
    QString mRequestText;
    bool mEnabled = true;
};
Q_DECLARE_TYPEINFO(AiTextInfo, Q_RELOCATABLE_TYPE);
QDebug operator<<(QDebug d, const AiTextInfo &t);
