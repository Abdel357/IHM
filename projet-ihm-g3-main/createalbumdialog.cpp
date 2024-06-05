#include "createalbumdialog.h"
#include "ui_createalbumdialog.h"
#include <QMessageBox>

CreateAlbumDialog::CreateAlbumDialog(AlbumCollection* albumCollection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateAlbumDialog),
    m_albumCollection(albumCollection)
{
    ui->setupUi(this);

    connect(ui->validateButton, &QPushButton::clicked, this, &CreateAlbumDialog::on_acceptButton_clicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &CreateAlbumDialog::on_cancelButton_clicked);
}

CreateAlbumDialog::~CreateAlbumDialog()
{
    delete ui;
}

void CreateAlbumDialog::on_acceptButton_clicked()
{
    qDebug()<<"TEST";
    QString albumDesc = "";
    QString albumName = ui->albumNameEdit->text().trimmed();
    albumDesc = ui->descNameEdit->text().trimmed();

    if(albumName.isEmpty()) {
        QMessageBox::warning(this, "Invalid Data", "Le nom de l'album ne peut pas être vide.");
        return;
    }

    int albumId = dbManager.addAlbum(albumName, albumDesc);
    if(albumId == -1) {
        QMessageBox::critical(this, "Database Error", "L'ajout de l'album dans la base de donnée à échoué.");
    } else {

        if (m_albumCollection) {
            qDebug() << "TEST 2";
            m_albumCollection->displayAlbumButtons(dbManager.getAllAlbumsData());
        }

        QMessageBox msgBox(QMessageBox::Information, "Album Created", "L'album a été créé avec succès.", QMessageBox::NoButton, this);
        QPushButton* okButton = msgBox.addButton(QMessageBox::Ok);

        if (okButton) {
            okButton->setCursor(Qt::PointingHandCursor);
        }

        msgBox.exec();
        accept();
    }
}

void CreateAlbumDialog::on_cancelButton_clicked()
{
    reject();
}
