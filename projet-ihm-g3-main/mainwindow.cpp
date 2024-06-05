#include "mainwindow.h"
#include "albumcollection.h"
#include "photocollection.h"
#include "filemanager.h"
#include "photoeditor.h"
#include "databaseviewer.h"
#include "databasemanager.h"
#include "createalbumdialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    MainWindow::setStyleSheet("stylesheet.qss");

    DatabaseManager databaseManager;
    databaseManager.openDatabase();

    setupUi(this);

    photocollectionWidget = new PhotoCollection(&databaseManager);
    albumcollectionWidget = new AlbumCollection(&databaseManager, photocollectionWidget, this);
    FileManager *filemanagerWidget = new FileManager(&databaseManager);
    photoeditorWidget = new PhotoEditor(&databaseManager);
    DatabaseViewer *databaseviewerWidget = new DatabaseViewer(&databaseManager);

    stackedWidget->addWidget(albumcollectionWidget);
    stackedWidget->addWidget(photocollectionWidget);
    stackedWidget->addWidget(filemanagerWidget);
    stackedWidget->addWidget(photoeditorWidget);
    stackedWidget->addWidget(databaseviewerWidget);

    connect(actionAlbum, &QAction::triggered, this, &MainWindow::on_menuAlbum_triggered);
    connect(actionFiles, &QAction::triggered, this, &MainWindow::on_menuFiles_triggered);
    connect(actionPhoto_editor, &QAction::triggered, this, &MainWindow::on_menuPhotoEditor_triggered);
    connect(actionPhotos, &QAction::triggered, this, &MainWindow::on_menuPhotos_triggered);
    connect(actionDatabase, &QAction::triggered, this, &MainWindow::on_menuDatabase_triggered);
    connect(actionCreer_Album, &QAction::triggered, this, &MainWindow::showCreateAlbumDialog);

    connect(albumcollectionWidget, &AlbumCollection::albumClicked, this, &MainWindow::onAlbumClicked);
    connect(photocollectionWidget, &PhotoCollection::editButtonClicked, this, &MainWindow::onPhotoEditorClicked);

}

void MainWindow::on_menuAlbum_triggered()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_menuPhotos_triggered()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_menuFiles_triggered()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_menuPhotoEditor_triggered()
{
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_menuDatabase_triggered()
{
    stackedWidget->setCurrentIndex(4);
}

void MainWindow::showCreateAlbumDialog() {
    CreateAlbumDialog dialog(albumcollectionWidget, this);
    dialog.exec();
}

void MainWindow::on_albums_clicked()
{
    qDebug("OK---------->APP");
    albumcollectionWidget->displayAlbumButtons(databaseManager->getAllAlbumsData());
    stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_files_clicked()
{
    stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_newAlbum_clicked()
{
    CreateAlbumDialog dialog(albumcollectionWidget, this);
    dialog.exec();
}

void MainWindow::on_quit_clicked()
{
    QCoreApplication::quit();
}


void MainWindow::onAlbumClicked(int albumId) {

    qDebug() << " onAlbumClicked -> " << albumId;
    photocollectionWidget->setCurrentAlbumId(albumId);
    stackedWidget->setCurrentIndex(1);

    QStringList photoFilePaths = databaseManager->getAllPhotoFilePathsInAlbum(albumId);
    photocollectionWidget->displayPhotos(photoFilePaths, albumId, databaseManager);

}

void MainWindow::onPhotoEditorClicked(const QString& path)
{
    qDebug() << " onPhotoEditorClicked -> " << path;
    photoeditorWidget->displayPhoto(path);
    stackedWidget->setCurrentIndex(3);

}

void MainWindow::on_albumPhoto_clicked()
{
    stackedWidget->setCurrentIndex(1);
}

