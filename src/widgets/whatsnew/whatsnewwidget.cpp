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
    QByteArray str;
    for (int i = 0; i < numRuqolaNewFeatures2_1; ++i) {
        str += ruqolaNewFeatures2_1[i].untranslatedText();
    }
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(str);
    return QLatin1String(md5.result().toBase64());
}

void WhatsNewWidget::updateInformations()
{
    slotVersionChanged(currentVersion());
}

QString WhatsNewWidget::generateStartEndHtml(const QString &str) const
{
    QString message = QStringLiteral("<qt>");
    message += str;
    message += QStringLiteral("</qt>");
    return message;
}

void WhatsNewWidget::slotVersionChanged(WhatsNewComboBoxWidget::VersionType type)
{
    if (type == WhatsNewComboBoxWidget::Version2_0) {
        const QString message = generateStartEndHtml(createVersionInformationsV2_0());
        mLabelInfo->setHtml(message);
    } else if (type == WhatsNewComboBoxWidget::Version2_1) {
        const QString message = generateStartEndHtml(createVersionInformationsV2_1());
        mLabelInfo->setHtml(message);
    } else if (type == WhatsNewComboBoxWidget::AllVersion) {
        QString message = generateVersionHeader(WhatsNewComboBoxWidget::Version2_0);
        message += createVersionInformationsV2_0();
        message += generateVersionHeader(WhatsNewComboBoxWidget::Version2_1);
        message += createVersionInformationsV2_1();
        mLabelInfo->setHtml(generateStartEndHtml(message));
    }
}

QString WhatsNewWidget::createVersionInformationsV2_0() const
{
    QString message;
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
    return message;
}

QString WhatsNewWidget::createVersionInformationsV2_1() const
{
    QString message;
    if (numRuqolaNewFeatures2_1 > 0) {
        message += QStringLiteral("<b>") + i18n("Some of the new features in this release of Ruqola include:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaNewFeatures2_1; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaNewFeatures2_1[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    return message;
}

QString WhatsNewWidget::generateVersionHeader(WhatsNewComboBoxWidget::VersionType type) const
{
    switch (type) {
    case WhatsNewComboBoxWidget::VersionType::AllVersion:
        return {};
    case WhatsNewComboBoxWidget::VersionType::Version2_0:
    case WhatsNewComboBoxWidget::VersionType::Version2_1:
        return QStringLiteral("<h1><i> %1 </i><hr/></h1><br>").arg(WhatsNewComboBoxWidget::convertVersionEnumToString(type));
    }
    return {};
}

#include "moc_whatsnewwidget.cpp"
