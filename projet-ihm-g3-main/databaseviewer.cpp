#include "databaseviewer.h"
#include "ui_databaseviewer.h"

DatabaseViewer::DatabaseViewer(DatabaseManager* dbManager, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::DatabaseViewer),
    m_dbManager(dbManager)
{
    ui->setupUi(this);
}


DatabaseViewer::~DatabaseViewer()
{
    delete ui;
}

void DatabaseViewer::on_run_clicked()
{
    QString name = ui->input_1->text();
    QString desc = ui->input_2->text();

    qDebug() << "name : " << name << " desc : " << desc;
    m_dbManager->addAlbum(name, desc);

}

void DatabaseViewer::on_addPhoto_clicked()
{
    QString title = ui->input_1->text();
    QString filePath = "filepath/photo.jpg";
    QDateTime captureDate = QDateTime::currentDateTime();
    QString color = "#333222";
    QString annotation = "???";
    int rating = 10;
    int width = -1;
    int height = -1;
    int x = -1;
    int y = -1;

    qDebug() << "title : " << title;

    int photoId = m_dbManager->addPhoto(title, filePath, captureDate, color, annotation, rating, width, height, x, y);
    int albumId = 1;
    m_dbManager->addPhotoToAlbum(albumId, photoId);
}

void DatabaseViewer::on_showPohtosInAlbum_clicked()
{

    int albumId = ui->input_1->text().toInt();
    qDebug() << "album ID = " << albumId;

    QList<QString> photoFilePaths = m_dbManager->getAllPhotoFilePathsInAlbum(albumId);

    qDebug() << "Photo paths in Album ID " << albumId << ":";

    for (const QString& filePath : photoFilePaths) {
        qDebug() << filePath;
    }
}


void DatabaseViewer::on_showAlbums_clicked()
{
    m_dbManager->viewAllAlbums();
}

