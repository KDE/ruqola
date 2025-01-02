/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

class GrabScreenPluginSettings
{
public:
    GrabScreenPluginSettings();
    ~GrabScreenPluginSettings();

    [[nodiscard]] int delay() const;
    void setDelay(int newDelay);

private:
    // Delay in ms
    int mDelay = 2000;
};
