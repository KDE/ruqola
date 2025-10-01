/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
namespace TextEmoticonsCore
{
class UnicodeEmoticonManager;
}
class RuqolaUnicodeEmoticonManager : public QObject
{
    Q_OBJECT
public:
    explicit RuqolaUnicodeEmoticonManager(QObject *parent = nullptr);
    ~RuqolaUnicodeEmoticonManager() override;

    static RuqolaUnicodeEmoticonManager *self();

private:
    TextEmoticonsCore::UnicodeEmoticonManager *const mUnicodeEmoticonManager;
};
