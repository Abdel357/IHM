#ifndef PHOTOCOLLECTION_H
#define PHOTOCOLLECTION_H

#include "ui_photocollection.h"
#include "databasemanager.h"
#include "photoinfo.h"

class PhotoCollection : public QWidget, private Ui::PhotoCollection
{
    Q_OBJECT

public:
    explicit PhotoCollection(DatabaseManager* dbManager, QWidget *parent = nullptr);
    void displayPhotos(const QStringList &photoFilePaths, int albumId, DatabaseManager* dbManager);
    void setCurrentAlbumId(int albumId);
    bool sortAscendingTitle;
    bool sortAscendingCreationDate;
    bool sortAscendingRating;

private slots:
    void on_searchKeywordButton_clicked();

    void on_search_keyword_2_clicked();

    void on_update_album_2_clicked();

    void on_button_edit_2_clicked();

    void on_button_delete_2_clicked();

    // void onSearchTextChanged(const QString &text);
    void onSearchTextChanged();

private:
    void on_textEdit_findKeyword_textChanged(const QString &keyword);
    void on_pushButton_orderTitle_clicked();
    void on_pushButton_orderCreationDate_clicked();
    void on_pushButton_orderRating_clicked();
    void onPhotoButtonClicked(const QString &filePath);
    void on_button_edit_clicked();

    void on_button_delete_clicked();

    void on_search_keyword_clicked();

    void on_update_album_clicked();

private:
    DatabaseManager* m_dbManager;
    int currentAlbumId;
    QMap<QString, PhotoInfo*> photoInfoMap;
    QString photoPath;
    int m_albumId;
    void updateAlbumInfo();

    void updateSuggestions(const QString &searchText);
    void onTextChanged();

signals:
    void editButtonClicked(const QString& path);
};

#endif // PHOTOCOLLECTION_H
