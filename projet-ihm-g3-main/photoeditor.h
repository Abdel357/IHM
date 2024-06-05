#ifndef PHOTOEDITOR_H
#define PHOTOEDITOR_H

#include "ui_photoeditor.h"
#include "databasemanager.h"
#include "photoinfo.h"

class PhotoEditor : public QWidget, private Ui::PhotoEditor
{
    Q_OBJECT

public:
    explicit PhotoEditor(DatabaseManager* dbManager, QWidget *parent = nullptr);
    void displayPhoto(const QString& photoPath) ;

private slots:
    void on_update_clicked();

    void on_addKeyword_clicked();

private:
    DatabaseManager* m_dbManager;
    PhotoInfo *photoInfo = new PhotoInfo();
    QString m_photoPath;
};

#endif // PHOTOEDITOR_H
