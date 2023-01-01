/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class ImportExportAutocorrectionLibreOfficeTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportExportAutocorrectionLibreOfficeTest(QObject *parent = nullptr);
    ~ImportExportAutocorrectionLibreOfficeTest() override = default;
private Q_SLOTS:
    void shouldImportExport();
    void shouldImportExport_data();
};
