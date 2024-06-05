// FileManager.h
// FileManager s'occupe de la gestion des fichiers et des albums photos

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QWidget>
#include <QListWidgetItem>
#include <QFileSystemModel>
#include "databasemanager.h"
#include "ui_filemanager.h"

class FileManager : public QWidget, private Ui::FileManager {
    Q_OBJECT

public:
    explicit FileManager(DatabaseManager* dbManager, QWidget *parent = nullptr);

private slots:
    // Slots pour gérer les interactions avec l'interface utilisateur
    void on_treeView_clicked(const QModelIndex &index);
    void on_listWidget_openFiles_itemClicked(QListWidgetItem *item);
    void on_addPhotoButton_clicked();
    void on_listWidget_openFiles_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_existingAlbum_itemClicked(QListWidgetItem *item);
    void on_listWidget_selectedAlbum_itemDoubleClicked(QListWidgetItem *item);

private:
    // Attributs privés pour stocker les informations nécessaires
    DatabaseManager* m_dbManager; // Gestionnaire de la base de données
    QFileSystemModel *dirModel; // Modèle de système de fichiers pour naviguer dans les dossiers
    void loadExistingAlbums(); // Charge la liste des albums existants
    void showEvent(QShowEvent *event); // Fonction permet d'actualiser la page
    void displayImage(const QString &filePath); // Affiche une image sélectionnée
    int selectedAlbumId; // ID de l'album actuellement sélectionné
    int photoId; // ID de la photo actuellement sélectionnée
};

#endif // FILEMANAGER_H
