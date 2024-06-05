#ifndef DATABASEVIEWER_H
#define DATABASEVIEWER_H

#include <QWidget>
#include "databasemanager.h"

namespace Ui {
class DatabaseViewer;
}

class DatabaseViewer : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseViewer(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~DatabaseViewer();

private slots:
    void on_run_clicked();

    void on_addPhoto_clicked();

    void on_showPohtosInAlbum_clicked();

    void on_showAlbums_clicked();

private:
    Ui::DatabaseViewer *ui;
    DatabaseManager* m_dbManager;
};

#endif // DATABASEVIEWER_H
