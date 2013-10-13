#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QString>
#include <QDialog>

namespace Ui {
class CategoryDialog;
}

class CategoryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CategoryDialog(QWidget *parent = 0);
    ~CategoryDialog();
    
    QString getCategoryText(void);
    
public slots:
    void slot_saveCategory( void );

signals:
    void sig_saveCategory( CategoryDialog *cat_dialog );
private:
    Ui::CategoryDialog *ui;
};

#endif // CATEGORYDIALOG_H
