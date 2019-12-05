#include "savefile.h"

SaveFile::SaveFile(QObject *parent) : QObject(parent)
{

}

void SaveFile::read()
{
    QByteArray data;
    QFile file("in.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    // You could use readAll() here, too.
    while (!in.atEnd()) {
        QString line = in.readLine();
        data.append(line);
    }

    file.close();
    return data;
}

void SaveFile::write(QByteArray data) const
{
    QFile file("out.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << data;
    file.close();

}
