#include "utiltool.h"
#include <QString>
#include <QUuid>
#include <QRegExp>
UtilTool::UtilTool()
{

}
string UtilTool::getUuid()
{
    QString uuidString = QUuid::createUuid().toString();
    uuidString.remove(QRegExp("[{}-]"));
    return uuidString.toStdString();
}
