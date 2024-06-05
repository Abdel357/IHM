#ifndef CREATEALBUMDIALOG_H
#define CREATEALBUMDIALOG_H

#include <QDialog>
#include "databasemanager.h"
#include "albumcollection.h"

namespace Ui {
class CreateAlbumDialog;
}

class CreateAlbumDialog : public QDialog
{
    Q_OBJECT

public:
    CreateAlbumDialog(AlbumCollection* albumCollection, QWidget *parent = nullptr);
    ~CreateAlbumDialog();

private slots:
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::CreateAlbumDialog *ui;
    DatabaseManager dbManager;
    AlbumCollection* m_albumCollection;

};

#endif // CREATEALBUMDIALOG_H
