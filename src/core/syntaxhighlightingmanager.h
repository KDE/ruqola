/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <QObject>

class SyntaxHighlightingManager : public QObject
{
    Q_OBJECT
public:
    ~SyntaxHighlightingManager() override = default;

    static SyntaxHighlightingManager *self();

    [[nodiscard]] bool syntaxHighlightingInitialized() const;

    [[nodiscard]] KSyntaxHighlighting::Definition def(const QString &name) const;
    [[nodiscard]] KSyntaxHighlighting::Definition defaultDef() const;

    KSyntaxHighlighting::Repository &repo() const;

private:
    explicit SyntaxHighlightingManager(QObject *parent = nullptr);
    void initialize();
    mutable KSyntaxHighlighting::Repository mRepo;
    KSyntaxHighlighting::Definition mDefaultDef;
    QList<QString> mDefinitions;
    bool mSyntaxHighlightingInitialized = false;
};
