/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiview.h"
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiView::AutoGenerateInteractionUiView(QObject *parent)
    : QObject(parent)
{
}

AutoGenerateInteractionUiView::~AutoGenerateInteractionUiView()
{
    delete mCloseButton;
    delete mSubmitButton;
}

void AutoGenerateInteractionUiView::clear()
{
    if (mCloseButton) {
        mCloseButton->deleteLater();
        mCloseButton = nullptr;
    }
    if (mSubmitButton) {
        mSubmitButton->deleteLater();
        mSubmitButton = nullptr;
    }
    if (mBlocks) {
        mBlocks->deleteLater();
        mBlocks = nullptr;
    }
}

void AutoGenerateInteractionUiView::parseView(const QJsonObject &json)
{
    if (json.contains("close"_L1)) {
        mCloseButton = new AutoGenerateInteractionUiViewButtonElement;
        mCloseButton->parse(json["close"_L1].toObject());
    }
    if (json.contains("submit"_L1)) {
        mSubmitButton = new AutoGenerateInteractionUiViewButtonElement;
        mSubmitButton->parse(json["submit"_L1].toObject());
    }
    mShowIcon = json["showIcon"_L1].toBool();
    mId = json["id"_L1].toString().toLatin1();
    mAppId = json["appId"_L1].toString().toLatin1();
    mType = json["type"_L1].toString().toLatin1();
    mTitle.parse(json["title"_L1].toObject());
    if (!mBlocks) {
        mBlocks = new AutoGenerateInteractionUiViewBlocks(this);
    }
    mBlocks->parse(json["blocks"_L1].toArray());
}

QByteArray AutoGenerateInteractionUiView::id() const
{
    return mId;
}

void AutoGenerateInteractionUiView::setId(const QByteArray &newId)
{
    mId = newId;
}

bool AutoGenerateInteractionUiView::operator==(const AutoGenerateInteractionUiView &other) const
{
    return other.id() == id() && other.showIcon() == showIcon() && *other.closeButton() == *closeButton() && *other.submitButton() == *submitButton()
        && other.title() == title() && other.blocks() == blocks() && other.appId() == appId() && other.type() == type();
}

bool AutoGenerateInteractionUiView::showIcon() const
{
    return mShowIcon;
}

void AutoGenerateInteractionUiView::setShowIcon(bool newShowIcon)
{
    mShowIcon = newShowIcon;
}

AutoGenerateInteractionUiViewButtonElement *AutoGenerateInteractionUiView::closeButton() const
{
    return mCloseButton;
}

void AutoGenerateInteractionUiView::setCloseButton(AutoGenerateInteractionUiViewButtonElement *newCloseButton)
{
    mCloseButton = newCloseButton;
}

AutoGenerateInteractionUiViewButtonElement *AutoGenerateInteractionUiView::submitButton() const
{
    return mSubmitButton;
}

void AutoGenerateInteractionUiView::setSubmitButton(AutoGenerateInteractionUiViewButtonElement *newSubmitButton)
{
    mSubmitButton = newSubmitButton;
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiView::title() const
{
    return mTitle;
}

void AutoGenerateInteractionUiView::setTitle(const AutoGenerateInteractionUiViewText &newTitle)
{
    mTitle = newTitle;
}

QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> AutoGenerateInteractionUiView::createStateInfos() const
{
    return mBlocks->createStateInfos();
}

void AutoGenerateInteractionUiView::assignState(const QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> &map)
{
    mBlocks->assignState(map);
}

AutoGenerateInteractionUiViewBlocks *AutoGenerateInteractionUiView::blocks() const
{
    return mBlocks;
}

void AutoGenerateInteractionUiView::setBlocks(AutoGenerateInteractionUiViewBlocks *newBlocks)
{
    mBlocks = newBlocks;
}

void AutoGenerateInteractionUiView::slotActionChanged(const QByteArray &blockId, const QByteArray &actionId, const QString &value)
{
    const AutoGenerateInteractionUtil::ViewBlockActionUserInfo info{
        .actionId = actionId,
        .blockIdPayload = blockId,
        .valuePayload = value,
        .idContainer = mId,
        .triggerId = QUuid::createUuid().toByteArray(QUuid::Id128),
    };
    Q_EMIT actionChanged(std::move(info));
}

void AutoGenerateInteractionUiView::generateWidget(QWidget *widget)
{
    widget->setWindowTitle(mTitle.generateText(true));
    // TODO get icon too ?
    // widget->setWindowIcon()
    auto mainLayout = new QVBoxLayout(widget);
    connect(mBlocks, &AutoGenerateInteractionUiViewBlocks::actionChanged, this, &AutoGenerateInteractionUiView::slotActionChanged);
    mBlocks->generateWidget(widget);
    if (mCloseButton || mSubmitButton) {
        auto buttonDialog = new QDialogButtonBox(widget);
        if (mCloseButton) {
            auto closeButton = buttonDialog->addButton(QDialogButtonBox::Close);
            closeButton->setText(mCloseButton->text().generateText());
            connect(closeButton, &QPushButton::clicked, this, [this]() {
                const QJsonObject payload = serialize();
                const QString appId = QString::fromLatin1(mAppId);
                Q_EMIT closeButtonClicked(payload, appId);
            });
        }
        if (mSubmitButton) {
            auto okButton = buttonDialog->addButton(QDialogButtonBox::Ok);
            okButton->setText(mSubmitButton->text().generateText());
            connect(okButton, &QPushButton::clicked, this, [this]() {
                const QJsonObject payload = serialize(true);
                const QString appId = QString::fromLatin1(mAppId);
                Q_EMIT submitButtonClicked(payload, appId);
            });
        }
        mainLayout->addStretch(1);
        mainLayout->addWidget(buttonDialog);
    }
}

QJsonObject AutoGenerateInteractionUiView::serializeState() const
{
    return mBlocks->serializeState();
}

QJsonObject AutoGenerateInteractionUiView::serialize(bool generateState) const
{
    QJsonObject o;
    o["showIcon"_L1] = mShowIcon;
    o["blocks"_L1] = mBlocks->serialize();
    o["title"_L1] = mTitle.serialize();
    if (mSubmitButton) {
        o["submit"_L1] = mSubmitButton->serialize();
    }
    if (mCloseButton) {
        o["close"_L1] = mCloseButton->serialize();
    }
    o["id"_L1] = QString::fromLatin1(mId);
    o["type"_L1] = QString::fromLatin1(mType);
    o["appId"_L1] = QString::fromLatin1(mAppId);
    if (generateState) {
        o["state"_L1] = serializeState();
    }
    return o;
}

QByteArray AutoGenerateInteractionUiView::type() const
{
    return mType;
}

void AutoGenerateInteractionUiView::setType(const QByteArray &newType)
{
    mType = newType;
}

QByteArray AutoGenerateInteractionUiView::appId() const
{
    return mAppId;
}

void AutoGenerateInteractionUiView::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

void AutoGenerateInteractionUiView::setErrorMessages(const QMap<QString, QString> &map)
{
    mBlocks->setErrorMessages(map);
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiView &t)
{
    d.space() << "id:" << t.id();
    d.space() << "type:" << t.type();
    d.space() << "appId:" << t.appId();
    d.space() << "showIcon:" << t.showIcon();
    if (t.closeButton()) {
        d.space() << "closeButton:" << *t.closeButton();
    }
    if (t.submitButton()) {
        d.space() << "submitButton:" << *t.submitButton();
    }
    d.space() << "title:" << t.title();
    if (t.blocks()) {
        d.space() << "blocks:" << *t.blocks();
    }
    return d;
}

#include "moc_autogenerateinteractionuiview.cpp"
