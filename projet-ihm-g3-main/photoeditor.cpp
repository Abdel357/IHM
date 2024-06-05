#include "photoeditor.h"
#include "photoinfo.h"
#include <QPixmap>
#include <QPainter>

PhotoEditor::PhotoEditor(DatabaseManager* dbManager, QWidget *parent) :
    QWidget(parent),
    m_dbManager(dbManager)
{
    setupUi(this);
}

void PhotoEditor::displayPhoto(const QString& photoPath) {

    m_photoPath = photoPath;

    QLayoutItem *item;
    while ((item = frame_photo->layout()->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QList<QString> photoData = m_dbManager->getPhotoData(photoPath);

    if (photoData.size() == 11) {

        photoInfo = new PhotoInfo();

        photoInfo->id = photoData[0];
        photoInfo->title = photoData[1];
        photoInfo->filePath = photoData[2];
        photoInfo->captureDate = photoData[3];
        photoInfo->color = photoData[4];
        photoInfo->annotation = photoData[5];
        photoInfo->rating = photoData[6];
        photoInfo->width = photoData[7];
        photoInfo->height = photoData[8];
        photoInfo->x = photoData[9];
        photoInfo->y = photoData[10];

        textEditTitle->setText(photoInfo->title);
        textEditAnnotation->setText(photoInfo->annotation);
        textEditRating->setText(photoInfo->rating);
        textEditX->setText(photoInfo->x);
        textEditY->setText(photoInfo->y);
        textEditWidth->setText(photoInfo->width);
        textEditHeight->setText(photoInfo->height);
    }

    QLabel *photoLabel = new QLabel(this);
    QPixmap photo(photoPath);

    if (!photo.isNull()) {
        int w = frame_photo->width();
        int h = frame_photo->height();

        if (w < 834 || h < 595) {
            w = 834;
            h = 595;
        }

        photoLabel->setPixmap(photo.scaled(w, h, Qt::KeepAspectRatio));
        photoLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        frame_photo->layout()->addWidget(photoLabel);
    } else {
        qDebug() << "Failed to load image: " << photoPath;
        delete photoLabel;
    }
}

void PhotoEditor::on_update_clicked()
{
    int id = m_dbManager->getPhotoId(m_photoPath);

    if (id != -1) {

        QString title = textEditTitle->toPlainText();
        QString annotation = textEditAnnotation->toPlainText();
        QString ratingStr = textEditRating->toPlainText();


        bool ok;
        int rating = ratingStr.toInt(&ok);

        if (ok) {

            m_dbManager->updatePhotoInfo(id, title, "", annotation, rating);
            qDebug() << "Photo information updated successfully. Photo ID: " << id;
        } else {
            qDebug() << "Invalid rating value: " << ratingStr;
        }
    } else {
        qDebug() << "Photo id not found !!!";
    }
}




void PhotoEditor::on_addKeyword_clicked()
{
    QString keyword = textEditAddKeyword->toPlainText().trimmed();

    if (!keyword.isEmpty()) {
        int photoId = m_dbManager->getPhotoId(m_photoPath);
        if (photoId != -1) {
            if (m_dbManager->addKeywordToPhoto(photoId, keyword)) {
                qDebug() << "Keyword added successfully. PHOTO ID : " << photoId << " KEYWORD : " << keyword;
                textEditAddKeyword->clear();
            } else {
                qDebug() << "Failed to add keyword.";
            }
        } else {
            qDebug() << "Photo id not found.";
        }
    } else {
        qDebug() << "Keyword is empty.";
    }
}


