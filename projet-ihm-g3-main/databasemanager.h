#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql>

struct AlbumData {
    int id;
    QString name;
    QString description;
};


class DatabaseManager
{
public:

    DatabaseManager();
    ~DatabaseManager();

    bool openDatabase();
    void closeDatabase();

    int addAlbum(const QString& name, const QString& description);

    int addPhoto(const QString& title, const QString& filePath,
                 const QDateTime& captureDate, const QString& color,
                 const QString& annotation, int rating, int width, int height, int x, int y);

    void updatePhotoInfo(int photoId, const QString& title, const QString& color, const QString& annotation, int rating);
    void updatePhotoCrop(int photoId, int width, int height, int x, int y);
    void updatePhoto(int photoId, const QString& title, const QString& filePath,
                const QDateTime& captureDate, const QString& color,
                const QString& annotation, int rating, int width, int height,
                     int x, int y);
    void updateAlbum(int currentAlbumId, const QString& newName, const QString& newDescription);
    bool removePhotoFromAlbum(int photoId, int albumId);
    bool removeAlbum(int albumId);
    bool addPhotoToAlbum(int albumId, int photoId);
    QList<QString> getPhotoData(const QString& filePath);
    int getPhotoId(const QString& filePath);
    QList<QString> getAllPhotoFilePathsInAlbum(int albumId);
    QMap<int, QString> getAllNamesAndIdsOfAlbum();
    QList<AlbumData> getAllAlbumsData();
    AlbumData getAlbumDataById(int albumId);
    int getKeywordId(const QString& keywordName);
    int addKeyword(const QString& keywordName);
    bool addKeywordToPhoto(int photoId, const QString& keywordName);
    void viewAllAlbums();
    int checkDuplicatePhoto(const QString& filePath);
    QList<QString> searchPhotosByKeyword(const QString& keyword);
    QList<QString> getPhotosInAlbumSortedByTitle(int albumId, bool ascending);
    QList<QString> getPhotosInAlbumSortedByCreationDate(int albumId, bool ascending);
    QList<QString> getPhotosInAlbumSortedByRating(int albumId, bool ascending);

    QList<QString> filterPhotosByKeywordInAlbum(int albumId, const QString& keyword);
    QList<QString> getAllKeywords();

    // QList<QString> DatabaseManager::searchPhotosByKeyword(const QString& keyword);
private:

    void createTables();
    void createAlbumTable();
    void createPhotoTable();
    void createAlbumPhotoTable();
    void createPhotoKeywordTable();
    void createKeywordTable();
    void dropTables();
};

#endif
