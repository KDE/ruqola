/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasejsonwidget.h"
#include "ruqolawidgets_debug.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>
#include <QPlainTextEdit>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseJsonWidget::ExploreDatabaseJsonWidget(QWidget *parent)
    : QWidget{parent}
    , mPlainTextEdit(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mPlainTextEdit->setObjectName(u"mPlainTextEdit"_s);
    mPlainTextEdit->setReadOnly(true);
    mainLayout->addWidget(mPlainTextEdit);

    const KSyntaxHighlighting::Definition def = mRepo.definitionForName(u"Json"_s);
    if (!def.isValid()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Invalid definition name";
    }

    auto hl = new KSyntaxHighlighting::SyntaxHighlighter(mPlainTextEdit->document());
    hl->setTheme((palette().color(QPalette::Base).lightness() < 128) ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                     : mRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    hl->setDefinition(def);
}

ExploreDatabaseJsonWidget::~ExploreDatabaseJsonWidget() = default;

void ExploreDatabaseJsonWidget::slotLoadedMessages(QList<Message> listMessages)
{
    QString str;
    std::sort(listMessages.begin(), listMessages.end(), [](const Message &lhs, const Message &rhs) {
        return lhs.timeStamp() < rhs.timeStamp();
    });
    for (const auto &m : listMessages) {
        str.append(QString::fromUtf8(m.serialize(m, false)));
    }
    mPlainTextEdit->setPlainText(str);
}

#include "moc_exploredatabasejsonwidget.cpp"
