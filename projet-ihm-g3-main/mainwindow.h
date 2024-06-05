#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtSql>
#include "photoeditor.h"
#include "ui_mainwindow.h"
#include "albumcollection.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QSqlDatabase DB_Connection;
    PhotoCollection *photocollectionWidget;
    AlbumCollection *albumcollectionWidget;
    DatabaseManager *databaseManager;
    PhotoEditor *photoeditorWidget;


private slots:
    void on_menuAlbum_triggered();
    void on_menuFiles_triggered();
    void on_menuPhotoEditor_triggered();
    void on_menuPhotos_triggered();
    void on_menuDatabase_triggered();
    void showCreateAlbumDialog();
    void on_albums_clicked();
    void on_files_clicked();
    void on_newAlbum_clicked();
    void on_quit_clicked();

    void onAlbumClicked(int albumId);
    void onPhotoEditorClicked(const QString& path);
    void on_albumPhoto_clicked();
};

#endif // MAINWINDOW_H
