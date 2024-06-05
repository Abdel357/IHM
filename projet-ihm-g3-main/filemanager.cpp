#include "filemanager.h"
#include <QFileSystemModel>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QDesktopServices>
#include <QMimeDatabase>

// Constructeur de la classe FileManager qui initialise l'interface utilisateur et la connexion à la base de données

FileManager::FileManager(DatabaseManager* dbManager, QWidget *parent)
    : QWidget(parent), m_dbManager(dbManager), selectedAlbumId(-1) {
    setupUi(this);

    // Configuration du modèle de système de fichiers pour afficher uniquement les fichiers d'images

    dirModel = new QFileSystemModel(this);
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    dirModel->setRootPath(desktopPath);
    dirModel->setNameFilters(QStringList() << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp");
    dirModel->setNameFilterDisables(false);
    treeView->setModel(dirModel);

    // Masquage de toutes les colonnes sauf la colonne 'name' dans le QTreeView

    for (int i = 1; i < dirModel->columnCount(); ++i) {
        treeView->setColumnHidden(i, true);
    }

    treeView->header()->setStretchLastSection(true);
    treeView->setRootIndex(dirModel->index(desktopPath));

    connect(treeView, &QTreeView::clicked, this, &FileManager::on_treeView_clicked);
    connect(listWidget_selectedFiles, &QListWidget::itemClicked, this, &FileManager::on_listWidget_openFiles_itemClicked);
    connect(addPhoto, &QPushButton::clicked, this, &FileManager::on_addPhotoButton_clicked);
    connect(listWidget_selectedFiles, &QListWidget::itemDoubleClicked, this, &FileManager::on_listWidget_openFiles_itemDoubleClicked);
    connect(listWidget_existingAlbum, &QListWidget::itemClicked, this, &FileManager::on_listWidget_existingAlbum_itemClicked);
    connect(listWidget_selectedAlbum, &QListWidget::itemDoubleClicked, this, &FileManager::on_listWidget_selectedAlbum_itemDoubleClicked);

    loadExistingAlbums();
}

// Utilisateur clique sur un élément dans le QTreeView

void FileManager::on_treeView_clicked(const QModelIndex &index) {

    QFileInfo fileInfo = dirModel->fileInfo(index);
    QMimeDatabase mimeDb;
    QMimeType mimeType = mimeDb.mimeTypeForFile(fileInfo.filePath());

    // Vérification si il s'agit bien d'une image
    if (mimeType.name().startsWith("image/")) {

        QList<QListWidgetItem*> items = listWidget_selectedFiles->findItems(fileInfo.fileName(), Qt::MatchExactly);
        if (items.isEmpty()) {
            QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
            item->setData(Qt::UserRole, fileInfo.absoluteFilePath());
            listWidget_selectedFiles->addItem(item);
        }
    }
}

// Utilisateur clique sur un élément dans la liste des fichiers sélectionnés

void FileManager::on_listWidget_openFiles_itemClicked(QListWidgetItem *item) {

    QString filePath = item->data(Qt::UserRole).toString();
    displayImage(filePath);
}

// Utilisateur double-clique sur un élément dans la liste des fichiers sélectionnés

void FileManager::on_listWidget_openFiles_itemDoubleClicked(QListWidgetItem *item) {

    delete item;
}

// Fonction pour afficher l'image dans le QLabel

void FileManager::displayImage(const QString &filePath) {
    QPixmap pixmap(filePath);
    label_photo->setPixmap(pixmap.scaled(label_photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// Fonction permet d'actualiser la page

void FileManager::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    listWidget_selectedFiles->clear();
    listWidget_selectedAlbum->clear();
    label_photo->clear();

    loadExistingAlbums();
}

//Fonction pour charger et afficher les albums existants depuis la base de données

void FileManager::loadExistingAlbums() {

    listWidget_existingAlbum->clear();

    auto albums = m_dbManager->getAllAlbumsData();
    for (const auto &album : albums) {
        QListWidgetItem *item = new QListWidgetItem(album.name);
        item->setData(Qt::UserRole, album.id);
        listWidget_existingAlbum->addItem(item);
    }
}

// Utilisateur clique sur un album dans la liste des albums existants

void FileManager::on_listWidget_existingAlbum_itemClicked(QListWidgetItem *item) {

    QList<QListWidgetItem*> items = listWidget_selectedAlbum->findItems(item->text(), Qt::MatchExactly);

    if (items.empty()) {
        listWidget_selectedAlbum->addItem(new QListWidgetItem(*item));
    }
}

// Utilisateur double-clique sur un album dans la liste des albums sélectionnés

void FileManager::on_listWidget_selectedAlbum_itemDoubleClicked(QListWidgetItem *item) {

    int row = listWidget_selectedAlbum->row(item);
    item = listWidget_selectedAlbum->takeItem(row);
    delete item;
}

// Utilisateur appuie sur le bouton pour ajouter des photos à un ou plusieurs album(s)

void FileManager::on_addPhotoButton_clicked() {

    bool photosAdded = false;

    QList<QListWidgetItem *> selectedPhotos;
    for (int i = 0; i < listWidget_selectedFiles->count(); ++i) {
        QListWidgetItem *item = listWidget_selectedFiles->item(i);
        selectedPhotos.append(item);
    }

    QList<QListWidgetItem *> selectedAlbums;
    for (int i = 0; i < listWidget_selectedAlbum->count(); ++i) {
        QListWidgetItem *item = listWidget_selectedAlbum->item(i);
        selectedAlbums.append(item);
    }

    if (selectedPhotos.isEmpty()) {
        QMessageBox::warning(this, "No Photo Selected", "Please select at least one photo first.");
        return;
    }

    if (selectedAlbums.isEmpty()) {
        QMessageBox::warning(this, "No Album Selected", "Please select at least one album first.");
        return;
    }


    foreach(QListWidgetItem *photoItem, selectedPhotos) {
        QString filePath = photoItem->data(Qt::UserRole).toString();


        int photoId = m_dbManager->checkDuplicatePhoto(filePath);

        if (photoId == -1) {

            QDateTime captureDate = QDateTime::currentDateTime();
            QString title = QFileInfo(filePath).baseName();
            photoId = m_dbManager->addPhoto(title, filePath, captureDate, "#333222", "", 10, -1, -1, -1, -1);
        }


        if (photoId == -1) {
            QMessageBox::warning(this, "Error", "Failed to add photo to database.");
            continue;
        }

        photosAdded = true;

        foreach(QListWidgetItem *albumItem, selectedAlbums) {
            int albumId = albumItem->data(Qt::UserRole).toInt();
            m_dbManager->addPhotoToAlbum(albumId, photoId);
        }
    }

    if (photosAdded) {

        QMessageBox::information(this, "Success", "Selected photos have been added to the albums successfully.");

        listWidget_selectedFiles->clearSelection();
        listWidget_selectedFiles->clear();
        listWidget_selectedAlbum->clearSelection();
        listWidget_selectedAlbum->clear();
        label_photo->clear();

        loadExistingAlbums();

    } else {
        QMessageBox::warning(this, "No Photos Added", "No new photos were added to the albums.");
    }
}
