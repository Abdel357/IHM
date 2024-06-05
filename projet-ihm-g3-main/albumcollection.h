#ifndef ALBUMCOLLECTION_H
#define ALBUMCOLLECTION_H

#include "ui_albumcollection.h"
#include "databasemanager.h"
#include "photocollection.h"
#include <QWidget>
#include <QMap>
#include <QPushButton>

class AlbumCollection : public QWidget, private Ui::AlbumCollection
{
    Q_OBJECT

public:
    void displayAlbumButtons(const QList<AlbumData>& albumDataList);
    explicit AlbumCollection(DatabaseManager* dbManager, PhotoCollection* photoCollection, QWidget *parent);
    void updateAlbums();

private :
    DatabaseManager* m_dbManager;
    PhotoCollection* m_photoCollection;
    Ui::AlbumCollection *ui;

    QPushButton* newAlbum(const AlbumData& albumData);
    QList<AlbumData> getAllAlbumsData();
    void applyGradientStyle(QPushButton* button, int id);
    void clearAlbumButtons();

private slots:
    void onAlbumButtonClicked(int albumId);
    void onCustomContextMenuRequested(const QPoint &pos);

signals:
    void albumClicked(int albumId);

};

#endif // ALBUMCOLLECTION_H
