#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Settings)
{
    ui_->setupUi(this);
}

Settings::~Settings()
{
    delete ui_;
}
