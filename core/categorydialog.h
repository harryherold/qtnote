#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QString>
#include <QDialog>

#include <util.h>

namespace Ui {
class CategoryDialog;
}

class CategoryDialog : public QDialog
{
    Q_OBJECT
    
public:

    save_t save_mode;
    
    explicit CategoryDialog(QWidget *parent = 0);
    ~CategoryDialog();
    
    QString getCategoryText(void);
    void setCategoryDesc( QString cat_desc );
    
public slots:
    void slot_saveCategory( void );

signals:
    void sig_saveCategory( CategoryDialog *cat_dialog );
private:
    Ui::CategoryDialog *ui;
};

#endif // CATEGORYDIALOG_H
