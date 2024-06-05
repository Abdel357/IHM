#ifndef PHOTOINFO_H
#define PHOTOINFO_H

#include <QObject>

class PhotoInfo : public QObject {
    Q_OBJECT

public:
    QString id;
    QString title;
    QString filePath;
    QString captureDate;
    QString color;
    QString annotation;
    QString rating;
    QString width;
    QString height;
    QString x;
    QString y;

signals:
    void buttonClicked(const PhotoInfo &photoInfo);
};

#endif // PHOTOINFO_H
