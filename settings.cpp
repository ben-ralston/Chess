#include "settings.h"
#include "ui_settings.h"

#include <QApplication>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QColorDialog>

#include "board_color_square.h"

Settings::Settings(QWidget *parent, bool twoPlayer, bool flipBoard, bool startWhiteVsComputer,
                   int whiteTime, int blackTime, int whiteIncrement, int blackIncrement,
                   const QColor presetColors[6], QColor primaryCustomColor, QColor secondaryCustomColor,
                   int selectedColorRow) :
    QWidget(parent),
    ui_(new Ui::Settings),
    selectedColorRow_(selectedColorRow),
    applied_(false)
{
    // TODO Set fixed size for settings window

    ui_->setupUi(this);
    connect(ui_->HvHButton, &QRadioButton::clicked, this, &Settings::humanVHumanClicked);
    connect(ui_->HvCButton, &QRadioButton::clicked, this, &Settings::humanVComputerClicked);
    connect(ui_->startWhiteButton, &QRadioButton::clicked, this, &Settings::startWhiteClicked);
    connect(ui_->startBlackButton, &QRadioButton::clicked, this, &Settings::startBlackClicked);
    connect(ui_->applyButton, &QPushButton::clicked, this, &Settings::apply);
    connect(ui_->cancelButton, &QPushButton::clicked, this, &Settings::close);

    QIntValidator *minuteValidator = new QIntValidator(0, 999);
    QIntValidator *secondValidator = new QIntValidator(0, 59);
    QIntValidator *incrementValidator = new QIntValidator(0, 999);

    ui_->whiteMinutes->setValidator(minuteValidator);
    ui_->blackMinutes->setValidator(minuteValidator);
    ui_->whiteSeconds->setValidator(secondValidator);
    ui_->blackSeconds->setValidator(secondValidator);
    ui_->whiteIncrement->setValidator(incrementValidator);
    ui_->blackIncrement->setValidator(incrementValidator);

    connect(ui_->whiteMinutes, &QLineEdit::editingFinished, ui_->whiteMinutes, &QLineEdit::clearFocus);
    connect(ui_->blackMinutes, &QLineEdit::editingFinished, ui_->blackMinutes, &QLineEdit::clearFocus);
    connect(ui_->whiteSeconds, &QLineEdit::editingFinished, ui_->whiteSeconds, &QLineEdit::clearFocus);
    connect(ui_->blackSeconds, &QLineEdit::editingFinished, ui_->blackSeconds, &QLineEdit::clearFocus);
    connect(ui_->whiteIncrement, &QLineEdit::editingFinished, ui_->whiteIncrement, &QLineEdit::clearFocus);
    connect(ui_->blackIncrement, &QLineEdit::editingFinished, ui_->blackIncrement, &QLineEdit::clearFocus);

    connect(ui_->choosePrimaryButton, &QPushButton::clicked, this, &Settings::choosePrimaryColor);
    connect(ui_->chooseSecondaryButton, &QPushButton::clicked, this, &Settings::chooseSecondaryColor);

    QHBoxLayout *boxes[4] = { ui_->boardColorHBox1, ui_->boardColorHBox2,
                              ui_->boardColorHBox3, ui_->boardColorHBox4};
    BoardColorSquare *colorSquare;
    for (int i = 0; i < 4; i++) {
        int insertIndex = i == 3 ? 0 : 1;

        if (i == 3) {
            colorSquare = new BoardColorSquare(ui_->boardColorFrame,
                                               i, true, primaryCustomColor);
            customPrimaryColor_ = colorSquare;
        } else
            colorSquare = new BoardColorSquare(ui_->boardColorFrame,
                                               i, true, presetColors[2 * i]);
        connect(colorSquare, &BoardColorSquare::clicked, this, &Settings::colorSquareClicked);
        connect(colorSquare, &BoardColorSquare::selectedColor, this, &Settings::setColor);
        connect(this, &Settings::setSelectedColor, colorSquare, &BoardColorSquare::setSelected);
        boxes[i]->insertWidget(insertIndex, colorSquare);

        if (i == 3) {
            colorSquare = new BoardColorSquare(ui_->boardColorFrame,
                                               i, false, secondaryCustomColor);
            customSecondaryColor_ = colorSquare;
        } else
            colorSquare = new BoardColorSquare(ui_->boardColorFrame,
                                               i, false, presetColors[2 * i + 1]);
        connect(colorSquare, &BoardColorSquare::clicked, this, &Settings::colorSquareClicked);
        connect(colorSquare, &BoardColorSquare::selectedColor, this, &Settings::setColor);
        connect(this, &Settings::setSelectedColor, colorSquare, &BoardColorSquare::setSelected);
        boxes[i]->insertWidget(insertIndex + 1, colorSquare);
    }

    emit setSelectedColor(selectedColorRow);

    if (twoPlayer) {
        ui_->HvHButton->click();
        if (flipBoard)
            ui_->flipBoardCheckBox->setChecked(true);
    } else {
        ui_->HvCButton->click();
        if (startWhiteVsComputer)
            ui_->startWhiteButton->setChecked(true);
        else
            ui_->startBlackButton->setChecked(true);
    }

    whiteTime /= 1000;
    int whiteMinutes = whiteTime / 60;
    int whiteSeconds = whiteTime % 60;
    int adjustedWhiteIncrement = whiteIncrement / 1000;

    blackTime /= 1000;
    int blackMinutes = blackTime / 60;
    int blackSeconds = blackTime % 60;
    int adjustedBlackIncrement = blackIncrement / 1000;

    ui_->whiteMinutes->setText(QString::number(whiteMinutes));
    ui_->whiteSeconds->setText(QString::number(whiteSeconds));
    ui_->whiteIncrement->setText(QString::number(adjustedWhiteIncrement));
    ui_->blackMinutes->setText(QString::number(blackMinutes));
    ui_->blackSeconds->setText(QString::number(blackSeconds));
    ui_->blackIncrement->setText(QString::number(adjustedBlackIncrement));
}

Settings::~Settings()
{
    delete ui_;
}

void Settings::humanVHumanClicked()
{
    ui_->startWhiteButton->setChecked(false);
    ui_->startWhiteButton->setEnabled(false);

    ui_->startBlackButton->setChecked(false);
    ui_->startBlackButton->setEnabled(false);

    ui_->flipBoardCheckBox->setEnabled(true);
}

void Settings::humanVComputerClicked()
{
    ui_->startWhiteButton->setEnabled(true);
    ui_->startBlackButton->setEnabled(true);

    ui_->flipBoardCheckBox->setChecked(false);
    ui_->flipBoardCheckBox->setEnabled(false);
}

void Settings::startWhiteClicked()
{
    ui_->startWhiteButton->setChecked(true);
    ui_->startBlackButton->setChecked(false);
}

void Settings::startBlackClicked()
{
    ui_->startBlackButton->setChecked(true);
    ui_->startWhiteButton->setChecked(false);
}

void Settings::colorSquareClicked(int row)
{
    selectedColorRow_ = row;
    emit setSelectedColor(row);
}

void Settings::setColor(QColor color, bool primary)
{
    if (primary)
        primaryColor_ = color;
    else
        secondaryColor_ = color;
}

void Settings::choosePrimaryColor()
{
    QColor newColor = QColorDialog::getColor(customPrimaryColor_->color());
    if (newColor.isValid()) {
        customPrimaryColor_->setColor(newColor);
    }
}

void Settings::chooseSecondaryColor()
{
    QColor newColor = QColorDialog::getColor(customSecondaryColor_->color());
    if (newColor.isValid()) {
        customSecondaryColor_->setColor(newColor);
    }
}

void Settings::apply()
{
    applied_ = true;
    close();
}

void Settings::mousePressEvent(QMouseEvent *event)
{
    QWidget *focusedWidget = focusWidget();
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(focusedWidget)) {
        if (!lineEdit->hasAcceptableInput())
            lineEdit->setText("0");
        lineEdit->clearFocus();
    }

    QWidget::mousePressEvent(event);
}

void Settings::closeEvent(QCloseEvent *event)
{
    if (applied_) {
        bool humanVHuman = ui_->HvHButton->isChecked();

        bool flipBoard;
        bool startWhite;
        if (humanVHuman) {
            flipBoard = ui_->flipBoardCheckBox->isChecked();
            startWhite = false;
        } else {
            flipBoard = false;
            startWhite = ui_->startWhiteButton->isChecked();
            if (!startWhite && !ui_->startBlackButton->isChecked())
                startWhite = true;
        }

        int whiteTime, blackTime;
        whiteTime = ui_->whiteMinutes->hasAcceptableInput() ?
                        ui_->whiteMinutes->text().toInt() * 60 * 1000 : 0;
        blackTime = ui_->blackMinutes->hasAcceptableInput() ?
                        ui_->blackMinutes->text().toInt() * 60 * 1000 : 0;
        whiteTime += ui_->whiteSeconds->hasAcceptableInput() ?
                        ui_->whiteSeconds->text().toInt() * 1000 : 0;
        blackTime += ui_->blackSeconds->hasAcceptableInput() ?
                        ui_->blackSeconds->text().toInt() * 1000 : 0;

        int whiteIncrement, blackIncrement;
        whiteIncrement = ui_->whiteIncrement->hasAcceptableInput() ?
                             ui_->whiteIncrement->text().toInt() * 1000 : 0;
        blackIncrement = ui_->blackIncrement->hasAcceptableInput() ?
                             ui_->blackIncrement->text().toInt() * 1000 : 0;

        emit closed(true, humanVHuman, flipBoard, startWhite, whiteTime, blackTime,
                    whiteIncrement, blackIncrement, primaryColor_, secondaryColor_,
                    customPrimaryColor_->color(), customSecondaryColor_->color(),
                    selectedColorRow_);
    } else
        emit closed(false);

    QWidget::closeEvent(event);
}

bool Settings::applied() const
{
    return applied_;
}
