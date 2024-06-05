#include "albumcollection.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QTextBrowser>


PhotoCollection::PhotoCollection(DatabaseManager* dbManager, QWidget *parent) :
    QWidget(parent),
    sortAscendingTitle(true),
    sortAscendingCreationDate(true),
    sortAscendingRating(true),
    m_dbManager(dbManager)
{
    setupUi(this);

    connect(pushButton_orderTitle, &QPushButton::clicked, this, &PhotoCollection::on_pushButton_orderTitle_clicked);
    connect(pushButton_orderCreationDate, &QPushButton::clicked, this, &PhotoCollection::on_pushButton_orderCreationDate_clicked);
    connect(pushButton_orderRating, &QPushButton::clicked, this, &PhotoCollection::on_pushButton_orderRating_clicked);

    connect(EditTextsortByKeyword->document(), &QTextDocument::contentsChanged, this, &PhotoCollection::onSearchTextChanged);
    connect(EditTextsortByKeyword, &QTextEdit::textChanged, this, &PhotoCollection::onTextChanged);

    onTextChanged();
    // connect(search_keyword_2, &QPushButton::clicked, this, &PhotoCollection::on_searchKeywordButton_clicked);
    // connect(update_album_2, &QPushButton::clicked, this, &PhotoCollection::updateAlbumInfo);
}


void PhotoCollection::displayPhotos(const QStringList &photoFilePaths, int albumId, DatabaseManager* dbManager) {


    m_albumId = albumId;
    AlbumData albumData = dbManager->getAlbumDataById(albumId);

    textEditAlbumName->setText(albumData.name);
    textEditAlbumDesc->setText(albumData.description);

    qDebug() << "DISPLAY PHOTOS !! ";
    qDebug() << "Photo File Paths:";

    PhotoInfo *photoInfo = new PhotoInfo();

    for (const QString &filePath : photoFilePaths) {
        qDebug() << filePath;

        QList<QString> photoData = dbManager->getPhotoData(filePath);

        if (photoData.size() == 11) {

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

            photoInfoMap.insert(filePath, photoInfo);
        }
    }

    QGridLayout *layout = qobject_cast<QGridLayout*>(photo_collection_grid->layout());

    if (!layout) {
        qWarning() << "Layout is not a QGridLayout!";
        return;
    }

    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    int row = 0;
    int col = 0;

    QSignalMapper *signalMapper = new QSignalMapper(this);

    foreach (const QString &filePath, photoFilePaths) {

        QFrame *frame = new QFrame(this);
        frame->setFrameStyle(QFrame::Box);

        int frameMaxHeight = 170;
        frame->setMaximumHeight(frameMaxHeight);

        QPushButton *photoButton = new QPushButton(frame);


        PhotoInfo *photoInfo = new PhotoInfo();
        QList<QString> photoData = dbManager->getPhotoData(filePath);

        if (photoData.size() == 11) {

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

            photoInfoMap.insert(filePath, photoInfo);
        }

        photoButton->setToolTip(QString("Title : %1\nAnnotation : %2\nRating : %3 / 10")
                                    .arg(photoInfo->title)
                                    .arg(photoInfo->annotation)
                                    .arg(photoInfo->rating));

        photoButton->setMaximumHeight(frameMaxHeight);
        photoButton->setCursor(Qt::PointingHandCursor);

        QPixmap originalPixmap(filePath);

        int buttonWidth = photoButton->width();
        int buttonHeight = frameMaxHeight;

        QPixmap scaledPixmap = originalPixmap.scaledToWidth(buttonWidth + 45, Qt::SmoothTransformation);

        if (scaledPixmap.height() > frameMaxHeight) {
            scaledPixmap = originalPixmap.scaledToHeight(buttonHeight, Qt::SmoothTransformation);
        }

        photoButton->setIcon(QIcon(scaledPixmap));
        photoButton->setIconSize(scaledPixmap.size());

        QVBoxLayout *frameLayout = new QVBoxLayout(frame);
        frameLayout->setContentsMargins(0, 0, 0, 0);
        frameLayout->setSpacing(0);

        frameLayout->addWidget(photoButton);
        frame->setLayout(frameLayout);

        connect(photoButton, &QPushButton::clicked, signalMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        signalMapper->setMapping(photoButton, filePath);

        layout->addWidget(frame, row, col);

        if (++col == 5) {
            col = 0;
            ++row;
        }
    }

    connect(signalMapper, &QSignalMapper::mappedString, this, &PhotoCollection::onPhotoButtonClicked);

    while (col < 5) {
        QFrame *emptyFrame = new QFrame(this);
        emptyFrame->setFrameStyle(QFrame::Box);
        emptyFrame->setMaximumHeight(170);

        layout->addWidget(emptyFrame, row, col);
        ++col;
    }
}

void PhotoCollection::onPhotoButtonClicked(const QString &filePath) {

    // Plus opti mais je ne n'arrive pas à le faire fonctionner.

    // PhotoInfo *photoInfo = photoInfoMap.value(filePath);

    // if (photoInfo) {

    //     qDebug() << "Button clicked for photo with ID:" << photoInfo->id;
    //     qDebug() << "Title:" << photoInfo->title;

    // }


    QList<QString> photoData = m_dbManager->getPhotoData(filePath);
    photoPath = filePath;

    PhotoInfo *photoInfo = new PhotoInfo();
    if (photoData.size() == 9) {

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
    }
    if (photoInfo) {

    qDebug() << "Button clicked for photo with ID:" << photoInfo->id;
    qDebug() << "Title:" << photoInfo->title;

    }
    qDebug() << "PHOTO ALBUM CLICKED " << filePath;
}



void PhotoCollection::setCurrentAlbumId(int albumId) {
    currentAlbumId = albumId;
}

// void PhotoCollection::on_textEdit_findKeyword_textChanged(const QString &keyword) {
// }

void PhotoCollection::on_searchKeywordButton_clicked() {

    QString keyword = EditTextsortByKeyword->toPlainText();

    qDebug() << "KEY WORD : " << keyword ;

    if (!keyword.isEmpty()) {

        QList<QString> filteredFilePaths = m_dbManager->filterPhotosByKeywordInAlbum(currentAlbumId, keyword);
        displayPhotos(filteredFilePaths, currentAlbumId, m_dbManager);
    }

    else {

        QList<QString> allFilePaths = m_dbManager->getAllPhotoFilePathsInAlbum(currentAlbumId);
        displayPhotos(allFilePaths, currentAlbumId, m_dbManager);

    }

}




void PhotoCollection::onTextChanged() {

    QString keyword = EditTextsortByKeyword->toPlainText().trimmed();

    if (!keyword.isEmpty()) {
        updateSuggestions(keyword);
        textBrowserKeyword->show();
    } else {
        textBrowserKeyword->clear();
        textBrowserKeyword->hide();
    }
}

void PhotoCollection::onSearchTextChanged() {
    QString keyword = EditTextsortByKeyword->toPlainText();
    updateSuggestions(keyword);
}

void PhotoCollection::updateSuggestions(const QString &searchText) {

    QList<QString> suggestions = m_dbManager->getAllKeywords();
    QStringList filteredSuggestions;

    for (const QString &suggestion : suggestions) {
        if (suggestion.contains(searchText, Qt::CaseInsensitive)) {
            filteredSuggestions << suggestion;
        }
    }

    textBrowserKeyword->clear();
    textBrowserKeyword->setText(filteredSuggestions.join("\n"));
}





void PhotoCollection::on_pushButton_orderTitle_clicked() {

    sortAscendingTitle = !sortAscendingTitle;

    if (sortAscendingTitle) {
        pushButton_orderTitle->setText("Order by title\n(A → Z)");

    } else {
        pushButton_orderTitle->setText("Order by title\n(A ← Z)");
    }

    QStringList filePathList = m_dbManager->getPhotosInAlbumSortedByTitle(currentAlbumId, sortAscendingTitle);

    displayPhotos(filePathList, currentAlbumId, m_dbManager);
}

void PhotoCollection::on_pushButton_orderCreationDate_clicked() {

    sortAscendingCreationDate = !sortAscendingCreationDate;

    if (sortAscendingCreationDate) {
        pushButton_orderCreationDate->setText("Order by creation date\n(recent → last)");

    } else {
        pushButton_orderCreationDate->setText("Order by creation date\n(recent ← last)");
    }
    QStringList filePathList = m_dbManager->getPhotosInAlbumSortedByCreationDate(currentAlbumId, sortAscendingCreationDate);

    displayPhotos(filePathList, currentAlbumId, m_dbManager);
}

void PhotoCollection::on_pushButton_orderRating_clicked() {

    sortAscendingRating = !sortAscendingRating;

    if (sortAscendingRating) {
        pushButton_orderRating->setText("Order by rating\n(0 → 10)");

    } else {
        pushButton_orderRating->setText("Order by rating\n(0 ← 10)");
    }

    QStringList filePathList = m_dbManager->getPhotosInAlbumSortedByRating(currentAlbumId, sortAscendingRating);

    displayPhotos(filePathList, currentAlbumId, m_dbManager);
}


void PhotoCollection::on_search_keyword_2_clicked()
{

    QString keyword = EditTextsortByKeyword->toPlainText();

    qDebug() << "KEY WORD : " << keyword << "currentAlbumId" << currentAlbumId ;

    if (!keyword.isEmpty()) {

        // QList<QString> filteredFilePaths = m_dbManager->filterPhotosByKeywordInAlbum(currentAlbumId, keyword);
        QList<QString> filteredFilePaths = m_dbManager->searchPhotosByKeyword(keyword);
        displayPhotos(filteredFilePaths, currentAlbumId, m_dbManager);

        qDebug() << "FILTERED FILE PATHS:";
        for (const QString &filePath : filteredFilePaths) {
            qDebug() << filePath;
        }
    }

    else {

        QList<QString> allFilePaths = m_dbManager->getAllPhotoFilePathsInAlbum(currentAlbumId);
        displayPhotos(allFilePaths, currentAlbumId, m_dbManager);

    }
}


void PhotoCollection::on_update_album_2_clicked()
{
    qDebug() << "change !!! ";
    QString newName = textEditAlbumName->toPlainText().trimmed();
    QString newDescription = textEditAlbumDesc->toPlainText().trimmed();
    textEditAlbumName->setText(newName);
    textEditAlbumDesc->setText(newDescription);
    if (!newName.isEmpty() && !newDescription.isEmpty()) {
        m_dbManager->updateAlbum(currentAlbumId, newName, newDescription);
        // QMessageBox::information(this, "Success", "Album updated successfully.");
    } else {
        // QMessageBox::critical(this, "Error", "Failed to update album. Make sure the name and description are not empty.");
    }
}


void PhotoCollection::on_button_edit_2_clicked()
{
    emit editButtonClicked(photoPath);
}


void PhotoCollection::on_button_delete_2_clicked()
{
    qDebug() << "deleted";
    int idPhoto = m_dbManager->getPhotoId(photoPath);
    m_dbManager->removePhotoFromAlbum(idPhoto,m_albumId);
    const QStringList &photoFilePaths = m_dbManager->getAllPhotoFilePathsInAlbum(m_albumId);
    displayPhotos(photoFilePaths, m_albumId, m_dbManager);
}

