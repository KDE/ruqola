/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageUrlTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageUrlTest(QObject *parent = nullptr);
    ~MessageUrlTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSerializeData();
    void shouldTestPreviewUrl();

    void shouldGenerateHtmlDescription();
    void shouldGenerateHtmlDescription_data();

    void shouldGenerateBuildImageUrl();
    void shouldGenerateBuildImageUrl_data();

    void shouldContentTypeConvert();
};
