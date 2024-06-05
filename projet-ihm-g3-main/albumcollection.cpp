#include "albumcollection.h"
#include "QtWidgets/qmenu.h"
#include "deletealbumdialog.h"
#include "ui_albumcollection.h"
#include <QRandomGenerator>
#include <QIcon>

AlbumCollection::AlbumCollection(DatabaseManager* dbManager, PhotoCollection* photoCollection, QWidget *parent) :
    QWidget(parent),
    m_dbManager(dbManager),
    m_photoCollection(photoCollection),
    ui(new Ui::AlbumCollection)
{
    ui->setupUi(this);

    displayAlbumButtons(m_dbManager->getAllAlbumsData());
}


QString getColor(int id) {
    QStringList colors = {
        "#E57373", "#F48FB1", "#E1BEE7", "#B39DDB", "#9FA8DA", "#90CAF9",
        "#81D4FA", "#80DEEA", "#80CBC4", "#A5D6A7", "#C5E1A5", "#E6EE9C",
        "#FFF59D", "#FFE082", "#FFCC80", "#FFAB91", "#E57373", "#F48FB1",
        "#E1BEE7", "#B39DDB", "#9FA8DA", "#90CAF9", "#81D4FA", "#80DEEA",
        "#80CBC4", "#A5D6A7", "#C5E1A5", "#E6EE9C", "#FFF59D", "#FFE082",
        "#FFCC80", "#FFAB91"
    };

    if (id % 31 > 15) {
        return colors[id % 31];
    }

    return colors[id];
}

QString getColor2(int id) {
    QStringList colors = {
        "#F48FB1", "#E1BEE7", "#B39DDB", "#9FA8DA", "#90CAF9", "#81D4FA",
        "#80DEEA", "#80CBC4", "#A5D6A7", "#C5E1A5", "#E6EE9C", "#FFF59D",
        "#FFE082", "#FFCC80", "#FFAB91", "#E57373", "#F48FB1", "#E1BEE7",
        "#B39DDB", "#9FA8DA", "#90CAF9", "#81D4FA", "#80DEEA", "#80CBC4",
        "#A5D6A7", "#C5E1A5", "#E6EE9C", "#FFF59D", "#FFE082", "#FFCC80",
        "#FFAB91"
    };

    if (id % 31 > 15) {
        return colors[id % 31];
    }

    return colors[id];
}

void AlbumCollection::applyGradientStyle(QPushButton* button, int id) {
    QString color = getColor(id);
    QString color2 = getColor2(id);
    QString gradientStyle = QString("background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 %1, stop: 1 %2);").arg(color, color2);
    button->setStyleSheet(gradientStyle);
}

QPushButton* AlbumCollection::newAlbum(const AlbumData& albumData) {
    QPushButton* albumButton = new QPushButton(albumData.name, this);
    albumButton->setProperty("albumId", albumData.id);
    albumButton->setCursor(Qt::PointingHandCursor);

    applyGradientStyle(albumButton, albumData.id);

    connect(albumButton, &QPushButton::clicked, [this, albumId = albumData.id](){ onAlbumButtonClicked(albumId); });
    return albumButton;
}

void AlbumCollection::clearAlbumButtons() {

    QLayoutItem *child;
    while ((child = ui->gridLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
}

void AlbumCollection::displayAlbumButtons(const QList<AlbumData>& albumDataList) {

    clearAlbumButtons();

    int numColumns = 3;
    int row = 0;
    int col = 0;

    for (const AlbumData& albumData : albumDataList) {
        QPushButton* albumButton = newAlbum(albumData);
        qDebug() << "Album ID --> " << albumData.id;
        ui->gridLayout->addWidget(albumButton, row, col);
        col = (col + 1) % numColumns;
        if (col == 0) {
            row++;
        }

        albumButton->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(albumButton, &QWidget::customContextMenuRequested, this, &AlbumCollection::onCustomContextMenuRequested);
    }
}


void AlbumCollection::updateAlbums() {
    qDebug() << "TEST 3";
    displayAlbumButtons(m_dbManager->getAllAlbumsData());
}

void AlbumCollection::onAlbumButtonClicked(int albumId) {
    qDebug() << "Album button clicked with ID:" << albumId;
//    QStringList photoFilePaths = m_dbManager->getAllPhotoFilePathsInAlbum(albumId);
//    m_photoCollection->displayPhotos(photoFilePaths);
    emit albumClicked(albumId);
}

void AlbumCollection::onCustomContextMenuRequested(const QPoint &pos) {

    qDebug() << "TEST SI C'EST APP ??";
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    int albumId = clickedButton->property("albumId").toInt();

    QMenu contextMenu;
    QAction* deleteAction = contextMenu.addAction("delete album");
    QAction* selectedAction = contextMenu.exec(clickedButton->mapToGlobal(pos));

    if (selectedAction == deleteAction) {
        DeleteAlbumDialog deleteDialog(this, albumId);
        if (deleteDialog.exec() == QDialog::Accepted) {
//            m_dbManager->removeAlbum(albumId);
            qDebug() << "UPDATE ALBUM !";
            updateAlbums();
        }
    }
}




