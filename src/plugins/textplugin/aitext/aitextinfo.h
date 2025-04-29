/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

class AiTextInfo
{
public:
    AiTextInfo();
    ~AiTextInfo();

    [[nodiscard]] bool enabled() const;
    void setEnabled(bool newEnabled);

private:
    bool mEnabled = true;
};
