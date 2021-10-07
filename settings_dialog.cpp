#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include <QApplication>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QColorDialog>

#include "board_color_square.h"
#include "settings.h"

SettingsDialog::SettingsDialog(QWidget *parent, const Settings &settings) :
    QDialog(parent),
    ui_(new Ui::SettingsDialog),
    selectedColorRow_(settings.selectedColorRow)
{
    // TODO Set fixed size for settings window

    ui_->setupUi(this);
    connect(ui_->HvHButton, &QRadioButton::clicked, this, &SettingsDialog::humanVHumanClicked);
    connect(ui_->HvCButton, &QRadioButton::clicked, this, &SettingsDialog::humanVComputerClicked);
    connect(ui_->startWhiteButton, &QRadioButton::clicked, this, &SettingsDialog::startWhiteClicked);
    connect(ui_->startBlackButton, &QRadioButton::clicked, this, &SettingsDialog::startBlackClicked);

    connect(ui_->choosePrimaryButton, &QPushButton::clicked, this, &SettingsDialog::choosePrimaryColor);
    connect(ui_->chooseSecondaryButton, &QPushButton::clicked, this, &SettingsDialog::chooseSecondaryColor);
    connect(ui_->applyButton, &QPushButton::clicked, this, &SettingsDialog::accept);
    connect(ui_->cancelButton, &QPushButton::clicked, this, &SettingsDialog::reject);

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

    QHBoxLayout *boxes[4] = { ui_->boardColorHBox1, ui_->boardColorHBox2,
                              ui_->boardColorHBox3, ui_->boardColorHBox4 };
    BoardColorSquare *colorSquare;
    for (int i = 0; i < 4; i++) {
        int insertIndex = i == 3 ? 0 : 1;

        if (i == 3) {
            colorSquare = new BoardColorSquare(ui_->boardColorFrame,
                                               i, true, settings.primaryCustomColor);
            primaryCustomColorSquare_ = colorSquare;
        } else
            colorSquare = new BoardColorSquare(ui_->boardColorFrame,
                                               i, true, settings.presetColors[2 * i]);
        connect(colorSquare, &BoardColorSquare::clicked, this, &SettingsDialog::colorSquareClicked);
        connect(this, &SettingsDialog::setSelectedRow, colorSquare, &BoardColorSquare::setSelected);
        boxes[i]->insertWidget(insertIndex, colorSquare);

        if (i == 3) {
            colorSquare = new BoardColorSquare(ui_->boardColorFrame,
                                               i, false, settings.secondaryCustomColor);
            secondaryCustomColorSquare_ = colorSquare;
        } else
            colorSquare = new BoardColorSquare(ui_->boardColorFrame,
                                               i, false, settings.presetColors[2 * i + 1]);
        connect(colorSquare, &BoardColorSquare::clicked, this, &SettingsDialog::colorSquareClicked);
        connect(this, &SettingsDialog::setSelectedRow, colorSquare, &BoardColorSquare::setSelected);
        boxes[i]->insertWidget(insertIndex + 1, colorSquare);
    }

    emit setSelectedRow(settings.selectedColorRow);

    if (settings.twoPlayer) {
        ui_->HvHButton->click();
        if (settings.flipBoard)
            ui_->flipBoardCheckBox->setChecked(true);
    } else {
        ui_->HvCButton->click();
        if (settings.startWhiteVsComputer)
            ui_->startWhiteButton->setChecked(true);
        else
            ui_->startBlackButton->setChecked(true);
    }

    int whiteTime = settings.whiteTime / 1000;
    int whiteMinutes = whiteTime / 60;
    int whiteSeconds = whiteTime % 60;
    int adjustedWhiteIncrement = settings.whiteIncrement / 1000;

    int blackTime = settings.blackTime / 1000;
    int blackMinutes = blackTime / 60;
    int blackSeconds = blackTime % 60;
    int adjustedBlackIncrement = settings.blackIncrement / 1000;

    ui_->whiteMinutes->setText(QString::number(whiteMinutes));
    ui_->whiteSeconds->setText(QString::number(whiteSeconds));
    ui_->whiteIncrement->setText(QString::number(adjustedWhiteIncrement));
    ui_->blackMinutes->setText(QString::number(blackMinutes));
    ui_->blackSeconds->setText(QString::number(blackSeconds));
    ui_->blackIncrement->setText(QString::number(adjustedBlackIncrement));

    settings_ = settings;

    setFixedSize(minimumSizeHint());
}

SettingsDialog::~SettingsDialog()
{
    delete ui_;
    delete primaryCustomColorSquare_;
    delete secondaryCustomColorSquare_;
}

const Settings &SettingsDialog::saveSettings()
{
    bool reset = false;

    bool newTwoPlayer = ui_->HvHButton->isChecked();
    if (settings_.twoPlayer != newTwoPlayer)
        reset = true;
    settings_.twoPlayer = newTwoPlayer;

    if (newTwoPlayer) {
        settings_.flipBoard =  ui_->flipBoardCheckBox->isChecked();
        settings_.startWhiteVsComputer = false;
    } else {
        settings_.flipBoard = false;

        bool newStartWhite;
        if (ui_->startWhiteButton->isChecked())
            newStartWhite = true;
        else if (ui_->startBlackButton->isChecked())
            newStartWhite = false;
        else {
            newStartWhite = true;
            ui_->startWhiteButton->click();
        }

        if (settings_.startWhiteVsComputer != newStartWhite)
            reset = true;
        settings_.startWhiteVsComputer = newStartWhite;
    }

    int newWhiteTime = ui_->whiteMinutes->hasAcceptableInput() ?
        ui_->whiteMinutes->text().toInt() * 60 * 1000 : 0;
    newWhiteTime += ui_->whiteSeconds->hasAcceptableInput() ?
        ui_->whiteSeconds->text().toInt() * 1000 : 0;

    int newBlackTime = ui_->blackMinutes->hasAcceptableInput() ?
        ui_->blackMinutes->text().toInt() * 60 * 1000 : 0;
    newBlackTime += ui_->blackSeconds->hasAcceptableInput() ?
        ui_->blackSeconds->text().toInt() * 1000 : 0;

    int newWhiteIncrement = ui_->whiteIncrement->hasAcceptableInput() ?
        ui_->whiteIncrement->text().toInt() * 1000 : 0;
    int newBlackIncrement = ui_->blackIncrement->hasAcceptableInput() ?
        ui_->blackIncrement->text().toInt() * 1000 : 0;

    if (settings_.whiteTime != newWhiteTime)
        reset = true;
    settings_.whiteTime = newWhiteTime;

    if (settings_.blackTime != newBlackTime)
        reset = true;
    settings_.blackTime = newBlackTime;

    if (settings_.whiteIncrement != newWhiteIncrement)
        reset = true;
    settings_.whiteIncrement = newWhiteIncrement;

    if (settings_.blackIncrement != newBlackIncrement)
        reset = true;
    settings_.blackIncrement = newBlackIncrement;

    settings_.selectedColorRow = selectedColorRow_;
    settings_.primaryCustomColor = primaryCustomColorSquare_->color();
    settings_.secondaryCustomColor = secondaryCustomColorSquare_->color();

    settings_.resetGame = reset;

    return settings_;
}

void SettingsDialog::revertSettings()
{
    if (settings_.twoPlayer) {
        ui_->HvHButton->click();
        if (settings_.flipBoard)
            ui_->flipBoardCheckBox->setChecked(true);
    } else {
        ui_->HvCButton->click();
        if (settings_.startWhiteVsComputer)
            ui_->startWhiteButton->setChecked(true);
        else
            ui_->startBlackButton->setChecked(true);
    }

    int whiteTime = settings_.whiteTime / 1000;
    int whiteMinutes = whiteTime / 60;
    int whiteSeconds = whiteTime % 60;
    int adjustedWhiteIncrement = settings_.whiteIncrement / 1000;

    int blackTime = settings_.blackTime / 1000;
    int blackMinutes = blackTime / 60;
    int blackSeconds = blackTime % 60;
    int adjustedBlackIncrement = settings_.blackIncrement / 1000;

    ui_->whiteMinutes->setText(QString::number(whiteMinutes));
    ui_->whiteSeconds->setText(QString::number(whiteSeconds));
    ui_->whiteIncrement->setText(QString::number(adjustedWhiteIncrement));
    ui_->blackMinutes->setText(QString::number(blackMinutes));
    ui_->blackSeconds->setText(QString::number(blackSeconds));
    ui_->blackIncrement->setText(QString::number(adjustedBlackIncrement));

    emit setSelectedRow(settings_.selectedColorRow);
    primaryCustomColorSquare_->setColor(settings_.primaryCustomColor);
    secondaryCustomColorSquare_->setColor(settings_.secondaryCustomColor);
}


void SettingsDialog::humanVHumanClicked()
{
    ui_->startWhiteButton->setChecked(false);
    ui_->startWhiteButton->setEnabled(false);

    ui_->startBlackButton->setChecked(false);
    ui_->startBlackButton->setEnabled(false);

    ui_->flipBoardCheckBox->setEnabled(true);
}

void SettingsDialog::humanVComputerClicked()
{
    ui_->startWhiteButton->setEnabled(true);
    ui_->startBlackButton->setEnabled(true);

    ui_->flipBoardCheckBox->setChecked(false);
    ui_->flipBoardCheckBox->setEnabled(false);
}

void SettingsDialog::startWhiteClicked()
{
    ui_->startWhiteButton->setChecked(true);
    ui_->startBlackButton->setChecked(false);
}

void SettingsDialog::startBlackClicked()
{
    ui_->startBlackButton->setChecked(true);
    ui_->startWhiteButton->setChecked(false);
}

void SettingsDialog::colorSquareClicked(int row)
{
    selectedColorRow_ = row;
    emit setSelectedRow(row);
}

void SettingsDialog::choosePrimaryColor()
{
    QColor newColor = QColorDialog::getColor(primaryCustomColorSquare_->color());
    if (newColor.isValid()) {
        primaryCustomColorSquare_->setColor(newColor);
    }
}

void SettingsDialog::chooseSecondaryColor()
{
    QColor newColor = QColorDialog::getColor(secondaryCustomColorSquare_->color());
    if (newColor.isValid()) {
        secondaryCustomColorSquare_->setColor(newColor);
    }
}

void SettingsDialog::mousePressEvent(QMouseEvent *event)
{
    QWidget *focusedWidget = focusWidget();
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(focusedWidget)) {
        if (!lineEdit->hasAcceptableInput())
            lineEdit->setText("0");
        lineEdit->clearFocus();
    }

    QWidget::mousePressEvent(event);
}
