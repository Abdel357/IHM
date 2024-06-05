#include "databasemanager.h"
#include <QDebug>

QSqlDatabase DB_Connection;

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
}

void DatabaseManager::createTables() {
    createAlbumTable();
    createPhotoTable();
    createAlbumPhotoTable();
    createKeywordTable();
    createPhotoKeywordTable();
}

void DatabaseManager::createAlbumTable() {

    QSqlQuery query(DB_Connection);

    if (query.exec("CREATE TABLE IF NOT EXISTS Album ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "name TEXT NOT NULL, "
                   "description TEXT)")) {
        qDebug() << "Table 'Album' created successfully.";
    } else {
        qDebug() << "Failed to create table 'Album': " << query.lastError().text();
    }
}

void DatabaseManager::createPhotoTable() {
    QSqlQuery query(DB_Connection);

    if (query.exec("CREATE TABLE IF NOT EXISTS Photo ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "title TEXT NOT NULL, "
                   "file_path TEXT NOT NULL, "
                   "capture_date DATETIME NOT NULL, "
                   "color TEXT, "
                   "annotation TEXT, "
                   "rating INTEGER, "
                   "width INTEGER, "
                   "height INTEGER, "
                   "x INTEGER, "
                   "y INTEGER)")) {
        qDebug() << "Table 'Photo' created successfully.";
    } else {
        qDebug() << "Failed to create table 'Photo': " << query.lastError().text();
    }
}

void DatabaseManager::createKeywordTable() {

    QSqlQuery query(DB_Connection);

    if (query.exec("CREATE TABLE IF NOT EXISTS Keyword ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "name TEXT NOT NULL)")) {
        qDebug() << "Table 'Keyword' created successfully.";
    } else {
        qDebug() << "Failed to create table 'Keyword': " << query.lastError().text();
    }
}

void DatabaseManager::createPhotoKeywordTable() {

    QSqlQuery query(DB_Connection);

    if (query.exec("CREATE TABLE IF NOT EXISTS PhotoKeyword ("
                   "photo_id INTEGER, "
                   "keyword_id INTEGER, "
                   "PRIMARY KEY (photo_id, keyword_id), "
                   "FOREIGN KEY (photo_id) REFERENCES Photo(id) ON DELETE CASCADE, "
                   "FOREIGN KEY (keyword_id) REFERENCES Keyword(id) ON DELETE CASCADE)")) {
        qDebug() << "Table 'PhotoKeyword' created successfully.";
    } else {
        qDebug() << "Failed to create table 'PhotoKeyword': " << query.lastError().text();
    }
}

void DatabaseManager::createAlbumPhotoTable() {

    QSqlQuery query(DB_Connection);

    if (query.exec("CREATE TABLE IF NOT EXISTS AlbumPhoto ("
                   "album_id INTEGER, "
                   "photo_id INTEGER, "
                   "PRIMARY KEY (album_id, photo_id), "
                   "FOREIGN KEY (album_id) REFERENCES Album(id) ON DELETE CASCADE, "
                   "FOREIGN KEY (photo_id) REFERENCES Photo(id) ON DELETE CASCADE)")) {
        qDebug() << "Table 'AlbumPhoto' created successfully.";
    } else {
        qDebug() << "Failed to create table 'AlbumPhoto': " << query.lastError().text();
    }
}

void DatabaseManager::dropTables() {

    QSqlQuery query(DB_Connection);

    if (query.exec("DROP TABLE IF EXISTS AlbumPhoto")) {
        qDebug() << "Table 'AlbumPhoto' dropped successfully.";
    } else {
        qDebug() << "Failed to drop table 'AlbumPhoto': " << query.lastError().text();
    }

    if (query.exec("DROP TABLE IF EXISTS PhotoKeyword")) {
        qDebug() << "Table 'PhotoKeyword' dropped successfully.";
    } else {
        qDebug() << "Failed to drop table 'PhotoKeyword': " << query.lastError().text();
    }

    if (query.exec("DROP TABLE IF EXISTS Keyword")) {
        qDebug() << "Table 'Keyword' dropped successfully.";
    } else {
        qDebug() << "Failed to drop table 'Keyword': " << query.lastError().text();
    }

    if (query.exec("DROP TABLE IF EXISTS Photo")) {
        qDebug() << "Table 'Photo' dropped successfully.";
    } else {
        qDebug() << "Failed to drop table 'Photo': " << query.lastError().text();
    }

    if (query.exec("DROP TABLE IF EXISTS Album")) {
        qDebug() << "Table 'Album' dropped successfully.";
    } else {
        qDebug() << "Failed to drop table 'Album': " << query.lastError().text();
    }
}

bool DatabaseManager::openDatabase()
{
    DB_Connection = QSqlDatabase::addDatabase("QSQLITE");
    DB_Connection.setDatabaseName(QCoreApplication::applicationDirPath() + "/photos.db");

    if (DB_Connection.open())
    {
        qDebug() << "Database is connected";
        // dropTables();
        createTables();

        return true;
    }
    else
    {
        qDebug() << "Error : " << DB_Connection.lastError();
        return false;
    }
}

int DatabaseManager::addPhoto(const QString& title, const QString& filePath,
                              const QDateTime& captureDate, const QString& color,
                              const QString& annotation, int rating, int width, int height, int x, int y) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("INSERT INTO Photo (title, file_path, capture_date, color, annotation, rating, width, height, x, y) "
                  "VALUES (:title, :file_path, :capture_date, :color, :annotation, :rating, :width, :height, :x, :y)");

    qDebug() << "title:" << title;
    qDebug() << "file_path:" << filePath;
    qDebug() << "capture_date:" << captureDate.toString(Qt::ISODate);
    qDebug() << "color:" << color;
    qDebug() << "annotation:" << annotation;
    qDebug() << "rating:" << rating;
    qDebug() << "width:" << width;
    qDebug() << "height:" << height;
    qDebug() << "x:" << x;
    qDebug() << "y:" << y;

    query.bindValue(":title", title);
    query.bindValue(":file_path", filePath);
    query.bindValue(":capture_date", captureDate.toString(Qt::ISODate));
    query.bindValue(":color", color);
    query.bindValue(":annotation", annotation);
    query.bindValue(":rating", rating);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.bindValue(":x", x);
    query.bindValue(":y", y);

    if (!query.exec()) {
        qDebug() << "Failed to insert photo:" << query.lastError().text();
        DB_Connection.rollback();
        return -1;
    }

    int photoId = query.lastInsertId().toInt();

    QSqlDatabase::database().commit();
    DB_Connection.close();

    qDebug() << "Photo added to Photo table successfully. Photo ID: " << photoId;

    return photoId;
}

int DatabaseManager::getPhotoId(const QString& filePath) {
    int photoId = -1;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT id FROM Photo WHERE file_path = :file_path");
    query.bindValue(":file_path", filePath);

    if (query.exec() && query.next()) {
        photoId = query.value("id").toInt();
    } else {
        qDebug() << "Failed to retrieve photo ID:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return photoId;
}

int DatabaseManager::addAlbum(const QString& name, const QString& description) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("INSERT INTO Album (name, description) VALUES (:name, :description)");
    query.bindValue(":name", name);
    query.bindValue(":description", description);

    if (!query.exec()) {
        qDebug() << "Failed to insert album:" << query.lastError().text();
        DB_Connection.rollback();
        return -1;
    }

    int albumId = query.lastInsertId().toInt();

    QSqlDatabase::database().commit();
    DB_Connection.close();

    qDebug() << "Album added successfully. Album ID: " << albumId;

    return albumId;
}


void DatabaseManager::updateAlbum(int currentAlbumId, const QString& newName, const QString& newDescription) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    QString updateQuery = "UPDATE Album SET ";

    if (!newName.isNull()) {
        updateQuery += "name = :name, ";
    }
    if (!newDescription.isNull()) {
        updateQuery += "description = :description, ";
    }

    updateQuery.chop(2);

    updateQuery += " WHERE id = :album_id";

    query.prepare(updateQuery);

    if (!newName.isNull()) {
        query.bindValue(":name", newName);
    }
    if (!newDescription.isNull()) {
        query.bindValue(":description", newDescription);
    }

    query.bindValue(":album_id", currentAlbumId);

    if (!query.exec()) {
        qDebug() << "Failed to update album:" << query.lastError().text();
        DB_Connection.rollback();
    } else {
        qDebug() << "Album updated successfully. Album ID: " << currentAlbumId;
        QSqlDatabase::database().commit();
    }

    DB_Connection.close();
}


void DatabaseManager::updatePhoto(int photoId, const QString& title, const QString& filePath,
                                  const QDateTime& captureDate, const QString& color,
                                  const QString& annotation, int rating, int width, int height,
                                  int x, int y) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    QString updateQuery = "UPDATE Photo SET ";

    if (!title.isNull()) {
        updateQuery += "title = :title, ";
    }
    if (!filePath.isNull()) {
        updateQuery += "file_path = :file_path, ";
    }
    if (!captureDate.isNull()) {
        updateQuery += "capture_date = :capture_date, ";
    }
    if (!color.isNull()) {
        updateQuery += "color = :color, ";
    }
    if (!annotation.isNull()) {
        updateQuery += "annotation = :annotation, ";
    }
    if (rating != -1) {
        updateQuery += "rating = :rating, ";
    }
    if (width != -1) {
        updateQuery += "width = :width, ";
    }
    if (height != -1) {
        updateQuery += "height = :height, ";
    }
    if (x != -1) {
        updateQuery += "x = :x, ";
    }
    if (y != -1) {
        updateQuery += "y = :y, ";
    }

    updateQuery.chop(2);

    updateQuery += " WHERE id = :photo_id";

    query.prepare(updateQuery);

    if (!title.isNull()) {
        query.bindValue(":title", title);
    }
    if (!filePath.isNull()) {
        query.bindValue(":file_path", filePath);
    }
    if (!captureDate.isNull()) {
        query.bindValue(":capture_date", captureDate.toString(Qt::ISODate));
    }
    if (!color.isNull()) {
        query.bindValue(":color", color);
    }
    if (!annotation.isNull()) {
        query.bindValue(":annotation", annotation);
    }
    if (rating != -1) {
        query.bindValue(":rating", rating);
    }
    if (width != -1) {
        query.bindValue(":width", width);
    }
    if (height != -1) {
        query.bindValue(":height", height);
    }
    if (x != -1) {
        query.bindValue(":x", x);
    }
    if (y != -1) {
        query.bindValue(":y", y);
    }

    query.bindValue(":photo_id", photoId);

    if (!query.exec()) {
        qDebug() << "Failed to update photo:" << query.lastError().text();
        DB_Connection.rollback();
    } else {
        qDebug() << "Photo updated successfully. Photo ID: " << photoId;
        QSqlDatabase::database().commit();
    }

    DB_Connection.close();
}

void DatabaseManager::updatePhotoCrop(int photoId, int width, int height, int x, int y) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    QString updateQuery = "UPDATE Photo SET ";

    if (width != -1) {
        updateQuery += "width = :width, ";
    }
    if (height != -1) {
        updateQuery += "height = :height, ";
    }
    if (x != -1) {
        updateQuery += "x = :x, ";
    }
    if (y != -1) {
        updateQuery += "y = :y, ";
    }

    updateQuery.chop(2);

    updateQuery += " WHERE id = :photo_id";

    query.prepare(updateQuery);

    if (width != -1) {
        query.bindValue(":width", width);
    }
    if (height != -1) {
        query.bindValue(":height", height);
    }
    if (x != -1) {
        query.bindValue(":x", x);
    }
    if (y != -1) {
        query.bindValue(":y", y);
    }

    query.bindValue(":photo_id", photoId);

    if (!query.exec()) {
        qDebug() << "Failed to update photo crop:" << query.lastError().text();
        DB_Connection.rollback();
    } else {
        qDebug() << "Photo crop updated successfully. Photo ID: " << photoId;
        QSqlDatabase::database().commit();
    }

    DB_Connection.close();
}


void DatabaseManager::updatePhotoInfo(int photoId, const QString& title, const QString& color, const QString& annotation, int rating) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    QString updateQuery = "UPDATE Photo SET ";

    if (!title.isNull()) {
        updateQuery += "title = :title, ";
    }
    if (!color.isNull()) {
        updateQuery += "color = :color, ";
    }
    if (!annotation.isNull()) {
        updateQuery += "annotation = :annotation, ";
    }
    if (rating != -1) {
        updateQuery += "rating = :rating, ";
    }

    updateQuery.chop(2);

    updateQuery += " WHERE id = :photo_id";

    query.prepare(updateQuery);

    if (!title.isNull()) {
        query.bindValue(":title", title);
    }
    if (!color.isNull()) {
        query.bindValue(":color", color);
    }
    if (!annotation.isNull()) {
        query.bindValue(":annotation", annotation);
    }
    if (rating != -1) {
        query.bindValue(":rating", rating);
    }

    query.bindValue(":photo_id", photoId);

    if (!query.exec()) {
        qDebug() << "Failed to update photo information:" << query.lastError().text();
        DB_Connection.rollback();
    } else {
        qDebug() << "Photo information updated successfully. Photo ID: " << photoId;
        QSqlDatabase::database().commit();
    }

    DB_Connection.close();
}

void DatabaseManager::viewAllAlbums() {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    if (query.exec("SELECT * FROM Album")) {
        while (query.next()) {
            int albumId = query.value("id").toInt();
            QString name = query.value("name").toString();
            QString description = query.value("description").toString();

            qDebug() << "ID: " << albumId << " | Name: " << name << " | Description: " << description;
        }
    } else {
        qDebug() << "Failed to retrieve albums:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();
}

bool DatabaseManager::addKeywordToPhoto(int photoId, const QString& keywordName) {

    int keywordId = getKeywordId(keywordName);


    if (keywordId == -1) {
        keywordId = addKeyword(keywordName);
        if (keywordId == -1) {
            qDebug() << "Failed to add keyword to PhotoKeyword: Unable to add keyword.";
            return false;
        }
    }

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("INSERT INTO PhotoKeyword (photo_id, keyword_id) VALUES (:photo_id, :keyword_id)");
    query.bindValue(":photo_id", photoId);
    query.bindValue(":keyword_id", keywordId);

    if (!query.exec()) {
        qDebug() << "Failed to add keyword to PhotoKeyword:" << query.lastError().text();
        DB_Connection.rollback();
        return false;
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    qDebug() << "Keyword added to PhotoKeyword successfully.";

    return true;
}

int DatabaseManager::getKeywordId(const QString& keywordName) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT id FROM Keyword WHERE name = :name");
    query.bindValue(":name", keywordName);

    if (query.exec() && query.next()) {
        int keywordId = query.value("id").toInt();
        QSqlDatabase::database().commit();
        DB_Connection.close();
        return keywordId;
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();
    return -1;
}

int DatabaseManager::addKeyword(const QString& keywordName) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    qDebug() << "KEYWORD NAME : " << keywordName;
    query.prepare("INSERT INTO Keyword (name) VALUES (:name)");
    query.bindValue(":name", keywordName);

    if (!query.exec()) {
        qDebug() << "Failed to add keyword:" << query.lastError().text();
        DB_Connection.rollback();
        return -1;
    }

    int keywordId = query.lastInsertId().toInt();

    QSqlDatabase::database().commit();
    DB_Connection.close();

    qDebug() << "Keyword added successfully. Keyword ID: " << keywordId;

    return keywordId;
}

QList<QString> DatabaseManager::searchPhotosByKeyword(const QString& keyword) {

    qDebug() << "KEY WORD SEARCH : " << keyword;
    QList<QString> photoPaths;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT file_path FROM Photo "
                  "JOIN PhotoKeyword ON Photo.id = PhotoKeyword.photo_id "
                  "JOIN Keyword ON PhotoKeyword.keyword_id = Keyword.id "
                  "WHERE Keyword.name = :keyword");
    query.bindValue(":keyword", keyword);

    if (query.exec()) {
        while (query.next()) {
            QString filePath = query.value("file_path").toString();
            qDebug() << "Matching Photo Path: " << filePath;
            photoPaths.append(filePath);
        }
    } else {
        qDebug() << "Failed to search photos by keyword:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return photoPaths;
}


QList<QString> DatabaseManager::getAllKeywords() {
    QList<QString> keywords;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT name FROM Keyword");

    if (query.exec()) {
        while (query.next()) {
            keywords.append(query.value("name").toString());
        }
    } else {
        qDebug() << "Failed to retrieve all keywords:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return keywords;
}



bool DatabaseManager::addPhotoToAlbum(int albumId, int photoId) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("INSERT INTO AlbumPhoto (album_id, photo_id) VALUES (:album_id, :photo_id)");
    query.bindValue(":album_id", albumId);
    query.bindValue(":photo_id", photoId);

    if (!query.exec()) {
        qDebug() << "Failed to establish relation in AlbumPhoto:" << query.lastError().text();
        DB_Connection.rollback();
        return false;
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    qDebug() << "Photo added to album successfully.";

    return true;
}

bool DatabaseManager::removePhotoFromAlbum(int photoId, int albumId) {

    if (!DB_Connection.isOpen() && !DB_Connection.open()) {
        qDebug() << "Failed to open database:" << DB_Connection.lastError().text();
        return false;
    }

    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT COUNT(*) FROM AlbumPhoto WHERE photo_id = :photo_id");
    query.bindValue(":photo_id", photoId);

    if (!query.exec()) {
        qDebug() << "Failed to check if photo is associated with other albums:" << query.lastError().text();
        DB_Connection.rollback();
        return false;
    }

    query.next();
    int associationCount = query.value(0).toInt();

    query.prepare("DELETE FROM AlbumPhoto WHERE album_id = :album_id AND photo_id = :photo_id");
    query.bindValue(":album_id", albumId);
    query.bindValue(":photo_id", photoId);

    if (!query.exec()) {
        qDebug() << "Failed to remove photo from album:" << query.lastError().text();
        DB_Connection.rollback();
        return false;
    }

    if (associationCount == 1) {
        query.prepare("DELETE FROM Photo WHERE id = :photo_id");
        query.bindValue(":photo_id", photoId);

        if (!query.exec()) {
            qDebug() << "Failed to remove photo from Photo table:" << query.lastError().text();
            DB_Connection.rollback();
            return false;
        }
    }

    QSqlDatabase::database().commit();

    if (!DB_Connection.isOpen()) {
        DB_Connection.close();
    }

    qDebug() << "Photo removed from album successfully.";

    return true;
}


bool DatabaseManager::removeAlbum(int albumId) {
    qDebug() << "ID :" << albumId;

    if (!DB_Connection.isOpen() && !DB_Connection.open()) {
        qDebug() << "Failed to open database:" << DB_Connection.lastError().text();
        return false;
    }

    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT photo_id FROM AlbumPhoto WHERE album_id = :album_id");
    query.bindValue(":album_id", albumId);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve photo associations for album:" << query.lastError().text();
        DB_Connection.rollback();
        return false;
    }

    while (query.next()) {
        int photoId = query.value("photo_id").toInt();

        if (!removePhotoFromAlbum(photoId, albumId)) {
            qDebug() << "Failed to remove photo association for album.";
            DB_Connection.rollback();
            return false;
        }
    }

    query.prepare("DELETE FROM Album WHERE id = :album_id");
    query.bindValue(":album_id", albumId);

    if (!query.exec()) {
        qDebug() << "Failed to remove album from Album table:" << query.lastError().text();
        DB_Connection.rollback();
        return false;
    }

    QSqlDatabase::database().commit();

    if (!DB_Connection.isOpen()) {
        DB_Connection.close();
    }

    qDebug() << "Album removed successfully.";

    return true;
}

QList<QString> DatabaseManager::getPhotoData(const QString& filePath) {

    QList<QString> photoData;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT id, title, file_path, capture_date, color, annotation, rating, width, height, x, y FROM Photo WHERE file_path = :file_path");
    query.bindValue(":file_path", filePath);

    if (query.exec() && query.next()) {

        photoData.append(query.value("id").toString());
        photoData.append(query.value("title").toString());
        photoData.append(query.value("file_path").toString());
        photoData.append(query.value("capture_date").toString());
        photoData.append(query.value("color").toString());
        photoData.append(query.value("annotation").toString());
        photoData.append(query.value("rating").toString());
        photoData.append(query.value("width").toString());
        photoData.append(query.value("height").toString());
        photoData.append(query.value("x").toString());
        photoData.append(query.value("y").toString());

    } else {
        qDebug() << "Failed to retrieve photo data:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return photoData;
}

QList<QString> DatabaseManager::getAllPhotoFilePathsInAlbum(int albumId) {

    QList<QString> filePaths;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT Photo.file_path FROM Photo "
                  "INNER JOIN AlbumPhoto ON Photo.id = AlbumPhoto.photo_id "
                  "WHERE AlbumPhoto.album_id = :album_id");
    query.bindValue(":album_id", albumId);

    if (query.exec()) {
        while (query.next()) {
            QString filePath = query.value("file_path").toString();
            filePaths.append(filePath);
        }
    } else {
        qDebug() << "Failed to retrieve photo paths:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return filePaths;
}


QMap<int, QString> DatabaseManager::getAllNamesAndIdsOfAlbum() {

    QMap<int, QString> albumMap;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT id, name FROM Album");

    if (query.exec()) {
        while (query.next()) {
            int albumId = query.value("id").toInt();
            QString albumName = query.value("name").toString();
            albumMap.insert(albumId, albumName);
        }
    } else {
        qDebug() << "Failed to retrieve album names and IDs:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return albumMap;
}

QList<AlbumData> DatabaseManager::getAllAlbumsData() {
    QList<AlbumData> albumDataList;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT id, name, description FROM Album");

    if (query.exec()) {
        while (query.next()) {
            AlbumData albumData;
            albumData.id = query.value("id").toInt();
            albumData.name = query.value("name").toString();
            albumData.description = query.value("description").toString();

            albumDataList.append(albumData);
        }
    } else {
        qDebug() << "Failed to retrieve album data:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return albumDataList;
}

AlbumData DatabaseManager::getAlbumDataById(int albumId) {
    AlbumData albumData;

    if (!DB_Connection.open()) {
        qDebug() << "Failed to open database:" << DB_Connection.lastError().text();
        return albumData;
    }

    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT id, name, description FROM Album WHERE id = :album_id");
    query.bindValue(":album_id", albumId);

    if (query.exec() && query.next()) {
        albumData.id = query.value("id").toInt();
        albumData.name = query.value("name").toString();
        albumData.description = query.value("description").toString();
    } else {
        qDebug() << "Failed to retrieve album data by ID:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return albumData;
}


int DatabaseManager::checkDuplicatePhoto(const QString& filePath) {

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    query.prepare("SELECT id FROM Photo WHERE file_path = :file_path");
    query.bindValue(":file_path", filePath);

    if (!query.exec()) {
        qDebug() << "Failed to check for duplicate photo:" << query.lastError().text();
        DB_Connection.rollback();
        return -1;
    }

    if (query.next()) {
        int photoId = query.value("id").toInt();
        qDebug() << "Duplicate photo found with ID:" << photoId;
        QSqlDatabase::database().commit();
        DB_Connection.close();
        return photoId;
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return -1;
}

QList<QString> DatabaseManager::getPhotosInAlbumSortedByTitle(int albumId, bool ascending) {

    QList<QString> filePaths;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    QString sortOrder = ascending ? "ASC" : "DESC";
    query.prepare("SELECT Photo.file_path FROM Photo "
                  "INNER JOIN AlbumPhoto ON Photo.id = AlbumPhoto.photo_id "
                  "WHERE AlbumPhoto.album_id = :album_id ORDER BY Photo.title " + sortOrder);

    query.bindValue(":album_id", albumId);

    if (query.exec()) {
        while (query.next()) {
            QString filePath = query.value("file_path").toString();
            filePaths.append(filePath);
        }
    } else {
        qDebug() << "Failed to retrieve sorted photo paths:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return filePaths;
}

QList<QString> DatabaseManager::getPhotosInAlbumSortedByCreationDate(int albumId, bool ascending) {

    QList<QString> filePaths;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    QString sortOrder = ascending ? "ASC" : "DESC";
    query.prepare("SELECT Photo.file_path FROM Photo "
                  "INNER JOIN AlbumPhoto ON Photo.id = AlbumPhoto.photo_id "
                  "WHERE AlbumPhoto.album_id = :album_id ORDER BY Photo.capture_date " + sortOrder);

    query.bindValue(":album_id", albumId);

    if (query.exec()) {
        while (query.next()) {
            QString filePath = query.value("file_path").toString();
            filePaths.append(filePath);
        }
    } else {
        qDebug() << "Failed to retrieve sorted photo paths:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return filePaths;
}

QList<QString> DatabaseManager::getPhotosInAlbumSortedByRating(int albumId, bool ascending) {

    QList<QString> filePaths;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    QString sortOrder = ascending ? "ASC" : "DESC";
    query.prepare("SELECT Photo.file_path FROM Photo "
                  "INNER JOIN AlbumPhoto ON Photo.id = AlbumPhoto.photo_id "
                  "WHERE AlbumPhoto.album_id = :album_id ORDER BY Photo.rating " + sortOrder);

    query.bindValue(":album_id", albumId);

    if (query.exec()) {
        while (query.next()) {
            QString filePath = query.value("file_path").toString();
            filePaths.append(filePath);
        }
    } else {
        qDebug() << "Failed to retrieve sorted photo paths:" << query.lastError().text();
    }

    QSqlDatabase::database().commit();
    DB_Connection.close();

    return filePaths;
}

QList<QString> DatabaseManager::filterPhotosByKeywordInAlbum(int albumId, const QString& keyword) {

    QList<QString> filePaths;

    DB_Connection.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(DB_Connection);

    QString formattedKeyword = "%" + keyword + "%";
    query.prepare("SELECT Photo.file_path FROM Photo "
                  "INNER JOIN AlbumPhoto ON Photo.id = AlbumPhoto.photo_id "
                  "WHERE AlbumPhoto.album_id = :album_id AND Photo.annotation LIKE :keyword");

    query.bindValue(":album_id", albumId);
    query.bindValue(":keyword", formattedKeyword);

    if (query.exec()) {
        while (query.next()) {
            QString filePath = query.value("file_path").toString();
            filePaths.append(filePath);
        }
    } else {
        qDebug() << "Failed to filter photo paths by keyword:" << query.lastError().text();
    }
    qDebug() << formattedKeyword;
    QSqlDatabase::database().commit();
    DB_Connection.close();


    return filePaths;
}

// QList<QString> DatabaseManager::searchPhotosByKeyword(const QString& keyword) {
//     QList<QString> filePaths;

//     DB_Connection.open();
//     QSqlDatabase::database().transaction();
//     QSqlQuery query(DB_Connection);

//     QString formattedKeyword = "%" + keyword + "%";
//     query.prepare("SELECT Photo.file_path FROM Photo "
//                   "INNER JOIN PhotoKeyword ON Photo.id = PhotoKeyword.photo_id "
//                   "INNER JOIN Keyword ON PhotoKeyword.keyword_id = Keyword.id "
//                   "WHERE Keyword.name LIKE :keyword");

//     query.bindValue(":keyword", formattedKeyword);

//     if (query.exec()) {
//         while (query.next()) {
//             QString filePath = query.value("file_path").toString();
//             filePaths.append(filePath);
//         }
//     } else {
//         qDebug() << "Failed to search photos by keyword:" << query.lastError().text();
//     }

//     qDebug() << "Matching Photo Paths for Keyword '" << keyword << "': " << filePaths;

//     QSqlDatabase::database().commit();
//     DB_Connection.close();

//     return filePaths;
// }


// bool DatabaseManager::updateAlbum(int albumId, const QString& name, const QString& description) {
//     if (!DB_Connection.isOpen() && !openDatabase()) {
//         qDebug() << "Failed to open database for updating album";
//         return false;
//     }

//     QSqlQuery query(DB_Connection);
//     query.prepare("UPDATE Album SET name = :name, description = :description WHERE id = :id");
//     query.bindValue(":name", name);
//     query.bindValue(":description", description);
//     query.bindValue(":id", albumId);
//     bool success = query.exec();

//     if (!success) {
//         qDebug() << "Failed to update album:" << query.lastError().text();
//     }
//     return success;
// }


void DatabaseManager::closeDatabase()
{
    if (DB_Connection.isOpen())
    {
        DB_Connection.close();
        qDebug() << "Database is closed";
    }
}
