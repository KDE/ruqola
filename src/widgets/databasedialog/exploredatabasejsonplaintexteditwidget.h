/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <KSyntaxHighlighting/Repository>
#include <QWidget>
class QPlainTextEdit;
class ExploreDatabaseJsonPlainTextEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseJsonPlainTextEditWidget(QWidget *parent = nullptr);
    ~ExploreDatabaseJsonPlainTextEditWidget() override;

    void setPlainText(const QString &str);

private:
    QPlainTextEdit *const mPlainTextEdit;
    KSyntaxHighlighting::Repository mRepo;
};
