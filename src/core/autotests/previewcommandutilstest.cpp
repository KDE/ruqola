/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewcommandutilstest.h"
#include "commands/previewcommand.h"
#include "commands/previewcommandutils.h"
#include "ruqola_autotest_helper.h"
#include <QDebug>
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(PreviewCommandUtilsTest)
PreviewCommandUtilsTest::PreviewCommandUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void PreviewCommandUtilsTest::shouldParsePreviewCommandUtils_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QList<PreviewCommand>>("expectedPreviewCommands");
    {
        QList<PreviewCommand> commands;
        {
            PreviewCommand command;
            command.setId("xUPGcJeEA1aYVxPRbG"_L1);
            command.setValue(
                "https://media3.giphy.com/media/v1.Y2lkPWM1NTZmMmE3Nzd5eHRtNXp6eWFicWd6ZzQzcG5xeHUwOG0wbXF4bnNrcnhjbnA4eiZlcD12MV9naWZzX3NlYXJjaCZjdD1n/xUPGcJeEA1aYVxPRbG/100.gif"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        {
            PreviewCommand command;
            command.setId("l3V0x6kdXUW9M4ONq"_L1);
            command.setValue(
                "https://media1.giphy.com/media/v1.Y2lkPWM1NTZmMmE3Nzd5eHRtNXp6eWFicWd6ZzQzcG5xeHUwOG0wbXF4bnNrcnhjbnA4eiZlcD12MV9naWZzX3NlYXJjaCZjdD1n/l3V0x6kdXUW9M4ONq/100.gif"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        {
            PreviewCommand command;
            command.setId("MXLOctGsM4ppUjev31"_L1);
            command.setValue(
                "https://media4.giphy.com/media/v1.Y2lkPWM1NTZmMmE3Nzd5eHRtNXp6eWFicWd6ZzQzcG5xeHUwOG0wbXF4bnNrcnhjbnA4eiZlcD12MV9naWZzX3NlYXJjaCZjdD1n/MXLOctGsM4ppUjev31/100.gif"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        {
            PreviewCommand command;
            command.setId("xUA7b2S7SxhM1cGdsQ"_L1);
            command.setValue(
                "https://media3.giphy.com/media/xUA7b2S7SxhM1cGdsQ/100.gif?cid=c556f2a777yxtm5zzyabqgzg43pnqxu08m0mqxnskrxcnp8z&ep=v1_gifs_search&rid=100.gif&ct=g"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        {
            PreviewCommand command;
            command.setId("RE5iREBNhI0Ok"_L1);
            command.setValue(
                "https://media2.giphy.com/media/RE5iREBNhI0Ok/100.gif?cid=c556f2a777yxtm5zzyabqgzg43pnqxu08m0mqxnskrxcnp8z&ep=v1_gifs_search&rid=100.gif&ct=g"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        {
            PreviewCommand command;
            command.setId("3o85xsGXVuYh8lM3EQ"_L1);
            command.setValue(
                "https://media2.giphy.com/media/3o85xsGXVuYh8lM3EQ/100.gif?cid=c556f2a777yxtm5zzyabqgzg43pnqxu08m0mqxnskrxcnp8z&ep=v1_gifs_search&rid=100.gif&ct=g"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        {
            PreviewCommand command;
            command.setId("Lsvn3qrOAo8RKdziTf"_L1);
            command.setValue(
                "https://media3.giphy.com/media/Lsvn3qrOAo8RKdziTf/100.gif?cid=c556f2a777yxtm5zzyabqgzg43pnqxu08m0mqxnskrxcnp8z&ep=v1_gifs_search&rid=100.gif&ct=g"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        {
            PreviewCommand command;
            command.setId("13SogtMD6cSqas"_L1);
            command.setValue(
                "https://media2.giphy.com/media/13SogtMD6cSqas/100.gif?cid=c556f2a777yxtm5zzyabqgzg43pnqxu08m0mqxnskrxcnp8z&ep=v1_gifs_search&rid=100.gif&ct=g"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        {
            PreviewCommand command;
            command.setId("y1GgSNQgd1Jx6"_L1);
            command.setValue(
                "https://media3.giphy.com/media/y1GgSNQgd1Jx6/100.gif?cid=c556f2a777yxtm5zzyabqgzg43pnqxu08m0mqxnskrxcnp8z&ep=v1_gifs_search&rid=100.gif&ct=g"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        {
            PreviewCommand command;
            command.setId("riwAaDlaA3zQQ"_L1);
            command.setValue(
                "https://media0.giphy.com/media/riwAaDlaA3zQQ/100.gif?cid=c556f2a777yxtm5zzyabqgzg43pnqxu08m0mqxnskrxcnp8z&ep=v1_gifs_search&rid=100.gif&ct=g"_L1);
            command.setType(PreviewCommand::TypePreview::Image);
            commands.append(std::move(command));
        }
        QTest::addRow("test1") << u"test1"_s << commands;
    }
    {
        QList<PreviewCommand> commands;
        QTest::addRow("empty") << u"empty"_s << commands;
    }
}

void PreviewCommandUtilsTest::shouldParsePreviewCommandUtils()
{
    QFETCH(QString, name);
    QFETCH(QList<PreviewCommand>, expectedPreviewCommands);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/previewcommandutil/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    QList<PreviewCommand> previewCommands = PreviewCommandUtils::parsePreviewJson(obj);
    const bool previewCommandsEqual = (previewCommands == expectedPreviewCommands);
    if (!previewCommandsEqual) {
        qDebug() << "previewCommands " << previewCommands;
        qDebug() << "expectedPreviewCommands " << expectedPreviewCommands;
    }
    QVERIFY(previewCommandsEqual);
}

#include "moc_previewcommandutilstest.cpp"
