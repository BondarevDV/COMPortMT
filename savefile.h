#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class SaveFile : public QObject
{
    Q_OBJECT
public:
    explicit SaveFile(QObject *parent = nullptr);
    void read();
    QByteArray data() const ;

    public slots:
       // void read(); // This would read into the internal variable  for instance
       void write(QByteArray data) const;



};

#endif // SAVEFILE_H
