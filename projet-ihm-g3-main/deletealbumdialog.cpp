#include "deletealbumdialog.h"
#include "ui_deletealbumdialog.h"
#include <QMessageBox>

DeleteAlbumDialog::DeleteAlbumDialog(QWidget *parent, int albumId) :
    QDialog(parent),
    ui(new Ui::DeleteAlbumDialog),
    albumId(albumId)
{
    ui->setupUi(this);
//    connect(ui->deleteButton, &QPushButton::clicked, this, &DeleteAlbumDialog::onDeleteConfirmed);
    connect(ui->cancelButton, &QPushButton::clicked, this, &DeleteAlbumDialog::onDeletionCancelled);
}

DeleteAlbumDialog::~DeleteAlbumDialog()
{
    delete ui;
}

void DeleteAlbumDialog::onDeleteConfirmed()
{
    bool albumIsDeleted = dbManager.removeAlbum(albumId);
    qDebug() << "TEST SI C'EST APP ?? 222 - album ID " << albumId << "DB OK : " << albumIsDeleted;

    if(albumId != -1 && albumIsDeleted) {

        QMessageBox msgBox(QMessageBox::Information, "Album Deleted", "The album has been successfully deleted.", QMessageBox::Ok, this);
        QAbstractButton* okButton = msgBox.button(QMessageBox::Ok);

        if (okButton) {
            okButton->setCursor(Qt::PointingHandCursor);
        }

        msgBox.exec();
        accept();
    } else {
        QMessageBox::warning(this, "Error", "Unable to delete the album.");
    }
}


void DeleteAlbumDialog::onDeletionCancelled()
{
    reject();
}
