/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

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
    explicit SyntaxHighlightingManager(QObject *parent = nullptr);
    ~SyntaxHighlightingManager() override = default;

    static SyntaxHighlightingManager *self();

    Q_REQUIRED_RESULT bool syntaxHighlightingInitialized() const;

    Q_REQUIRED_RESULT KSyntaxHighlighting::Definition def(const QString &name) const;
    Q_REQUIRED_RESULT KSyntaxHighlighting::Definition defaultDef() const;

    KSyntaxHighlighting::Repository &repo() const;

private:
    Q_DISABLE_COPY(SyntaxHighlightingManager)
    void initialize();
    mutable KSyntaxHighlighting::Repository mRepo;
    KSyntaxHighlighting::Definition mDefaultDef;
    QVector<QString> mDefinitions;
    bool mSyntaxHighlightingInitialized = false;
};

