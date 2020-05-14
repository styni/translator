#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QObject>

class Translation : public QObject
{
    Q_OBJECT
public:
    explicit Translation(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TRANSLATION_H
