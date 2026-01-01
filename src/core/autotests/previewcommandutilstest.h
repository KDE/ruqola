/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PreviewCommandUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit PreviewCommandUtilsTest(QObject *parent = nullptr);
    ~PreviewCommandUtilsTest() override = default;

private Q_SLOTS:
    void shouldParsePreviewCommandUtils();
    void shouldParsePreviewCommandUtils_data();
};
