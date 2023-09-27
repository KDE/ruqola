/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewwidget.h"
#include "whatsnewcomboboxwidget.h"
#include "whatsnewwidgettranslation.h"

#include <KLazyLocalizedString>

#include <QCryptographicHash>
#include <QScrollArea>
#include <QTextEdit>
#include <QVBoxLayout>

WhatsNewWidget::WhatsNewWidget(QWidget *parent)
    : QWidget{parent}
    , mLabelInfo(new QTextEdit(this))
    , mWhatsNewComboBoxWidget(new WhatsNewComboBoxWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mWhatsNewComboBoxWidget->setObjectName(QStringLiteral("mWhatsNewComboBoxWidget"));
    mainLayout->addWidget(mWhatsNewComboBoxWidget);

    mLabelInfo->setObjectName(QStringLiteral("mLabelInfo"));
    mLabelInfo->setReadOnly(true);
    mLabelInfo->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    connect(mWhatsNewComboBoxWidget, &WhatsNewComboBoxWidget::versionChanged, this, &WhatsNewWidget::slotVersionChanged);
    mWhatsNewComboBoxWidget->initializeVersion(currentVersion());
    mainLayout->addWidget(mLabelInfo);
}

WhatsNewWidget::~WhatsNewWidget() = default;

WhatsNewComboBoxWidget::VersionType WhatsNewWidget::currentVersion() const
{
    return WhatsNewComboBoxWidget::Version2_1;
}

// static
QString WhatsNewWidget::newFeaturesMD5()
{
    // TODO use last version
    // => update each time that we change version
    QByteArray str;
    for (int i = 0; i < numRuqolaChanges2_0; ++i) {
        str += ruqolaChangesV2_0[i].untranslatedText();
    }
    for (int i = 0; i < numRuqolaNewFeatures2_0; ++i) {
        str += ruqolaNewFeatures2_0[i].untranslatedText();
    }
    for (int i = 0; i < numRuqolaBugfixing2_0; ++i) {
        str += ruqolaBugfixing2_0[i].untranslatedText();
    }
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(str);
    return QLatin1String(md5.result().toBase64());
}

void WhatsNewWidget::updateInformations()
{
    mLabelInfo->setHtml(createVersionInformations());
}

void WhatsNewWidget::slotVersionChanged(WhatsNewComboBoxWidget::VersionType type)
{
    if (type == WhatsNewComboBoxWidget::Version2_0 || type == WhatsNewComboBoxWidget::AllVersion) {
        mLabelInfo->setHtml(createVersionInformations());
    }
}

QString WhatsNewWidget::createVersionInformations() const
{
    QString message = QStringLiteral("<qt>");
    if (numRuqolaChanges2_0 > 0) {
        message += QStringLiteral("<b>") + i18n("Important changes since last version:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaChanges2_0; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaChangesV2_0[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaNewFeatures2_0 > 0) {
        message += QStringLiteral("<b>") + i18n("Some of the new features in this release of Ruqola include:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaNewFeatures2_0; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaNewFeatures2_0[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaBugfixing2_0 > 0) {
        message += QStringLiteral("<b>") + i18n("Some bug fixing:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaBugfixing2_0; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaBugfixing2_0[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    message += QStringLiteral("</qt>");
    return message;
}

QString WhatsNewWidget::generateVersionHeader(WhatsNewComboBoxWidget::VersionType type) const
{
    switch (type) {
    case WhatsNewComboBoxWidget::VersionType::AllVersion:
        return {};
    case WhatsNewComboBoxWidget::VersionType::Version2_0:
    case WhatsNewComboBoxWidget::VersionType::Version2_1:
        return QStringLiteral("--- %1 ---\n").arg(WhatsNewComboBoxWidget::convertVersionEnumToString(type));
    }
    return {};
}

#include "moc_whatsnewwidget.cpp"
