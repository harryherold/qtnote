#include "categorydialog.h"
#include "ui_categorydialog.h"

CategoryDialog::CategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryDialog)

{   
    ui->setupUi(this);
    ui->lineEdit->setFocus( Qt::OtherFocusReason );
    connect( ui->buttonBox , SIGNAL(accepted()), this, SLOT( slot_saveCategory() ) );
}

CategoryDialog::~CategoryDialog()
{
    delete ui;
}

void CategoryDialog::slot_saveCategory( void )
{
    emit sig_saveCategory( this );
}

QString CategoryDialog::getCategoryText( void )
{
    return ui->lineEdit->text();
}
