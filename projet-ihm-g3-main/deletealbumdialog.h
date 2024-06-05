#ifndef DELETEALBUMDIALOG_H
#define DELETEALBUMDIALOG_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class DeleteAlbumDialog;
}

class DeleteAlbumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteAlbumDialog(QWidget *parent = nullptr, int albumId = -1);
    ~DeleteAlbumDialog();

private slots:
    void onDeleteConfirmed();
    void onDeletionCancelled();

private:
    Ui::DeleteAlbumDialog *ui;
    DatabaseManager dbManager;
    int albumId;
};

#endif // DELETEALBUMDIALOG_H
