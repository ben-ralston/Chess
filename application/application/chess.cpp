#include "chess.h"
#include "ui_chess.h"

#include <QKeyEvent>
#include <QHeaderView>
#include <QWidget>

#include "game.h"
#include "chess_layout.h"
#include "square.h"
#include "promotion_square.h"
#include "timer.h"
#include "chess/piece.h"
#include "notation_model.h"
#include "settings/settings_dialog.h"
#include "settings/settings.h"

using namespace std;

Chess::Chess(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::Chess)
{
    ui_->setupUi(this);
    connect(ui_->newGame, &QPushButton::clicked, ui_->winFrame, &QFrame::hide);
    connect(ui_->winNewGame, &QPushButton::clicked, ui_->winFrame, &QFrame::hide);
    connect(ui_->settingsButton, &QPushButton::clicked, this, &Chess::openSettings);

    Settings settings = initialSettings();
    settingsDialog_ = new SettingsDialog(this, settings);
    connect(settingsDialog_, &SettingsDialog::finished, this, &Chess::updateSettings);

    game_ = new Game(this, settings.whiteTime, settings.blackTime, settings.whiteIncrement, settings.blackIncrement);
    connect(game_, &Game::gameEnded, this, &Chess::gameOver);
    connect(game_, &Game::updateTimerLabels, this, &Chess::updateTimerLabels);
    connect(ui_->newGame, &QPushButton::clicked, game_, &Game::reset);
    connect(ui_->winNewGame, &QPushButton::clicked, game_, &Game::reset);
    connect(this, &Chess::keyPress, game_, &Game::keyPress);

    model_ = new NotationModel(this);
    connect(model_, &NotationModel::updateShownMove, game_, &Game::updateShownMove);
    connect(model_, &NotationModel::scrollTable, ui_->table, &QTableView::scrollToBottom);
    connect(game_, &Game::notateMove, model_, &NotationModel::addMove);
    connect(game_, &Game::clearNotation, model_, &NotationModel::clearMoves);
    connect(game_, &Game::notationMoveNumber, model_, &NotationModel::setMoveNumber);
    connect(ui_->table, &QTableView::clicked, model_, &NotationModel::clickedCell);

    ui_->table->setModel(model_);
    ui_->table->setShowGrid(false);
    ui_->table->horizontalHeader()->hide();
    ui_->table->verticalHeader()->hide();
    ui_->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui_->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->table->setFocusPolicy(Qt::NoFocus);
    ui_->table->setSelectionMode(QAbstractItemView::NoSelection);
    ui_->table->show();

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(ui_->newGame, 0, Qt::AlignTop);
    leftLayout->addWidget(ui_->settingsButton, 0, Qt::AlignBottom);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(ui_->topTimerLabel, 0, Qt::AlignLeft);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(ui_->bottomTimerLabel, 0, Qt::AlignRight);

    layout_ = new ChessLayout(ui_->centralWidget, QMargins(5, 5, 5, 5), 5);
    layout_->addLayout(leftLayout, ChessLayout::West);
    layout_->addLayout(topLayout, ChessLayout::North);
    layout_->addLayout(bottomLayout, ChessLayout::South);
    layout_->addWidget(ui_->table, ChessLayout::East);
    layout_->addWidget(ui_->winFrame, ChessLayout::EndScreen);

    Square::setColors(Settings::presetColors[0], Settings::presetColors[1]);

    Square *square;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            square = new Square(ui_->centralWidget, row, col);
            layout_->addWidget(square, ChessLayout::Board, row, col);

            connect(square, &Square::clicked, game_, &Game::mousePress);
            connect(game_, &Game::setPiece, square, &Square::setPiece);
            connect(game_, &Game::highlightSquare, square, &Square::setHighlight);
        }
    }

    PromotionSquare *promoSquare;
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            promoSquare = new PromotionSquare(ui_->centralWidget, row, col);
            layout_->addWidget(promoSquare, ChessLayout::Promotion, row, col);

            connect(promoSquare, &PromotionSquare::promotionPiece, game_, &Game::completePromotion);
            connect(game_, &Game::setPromotionColor, promoSquare, &PromotionSquare::setColor);
            connect(game_, &Game::setPromotionVisibilty, promoSquare, &PromotionSquare::setVisibility);
        }
    }

    ui_->winFrame->raise();
    ui_->winFrame->hide();

    // TODO move Timers inside Game class
    whiteTimer_ = new Timer(this, true);
    blackTimer_ = new Timer(this, false);

    connect(game_, &Game::startTimer, whiteTimer_, &Timer::start);
    connect(game_, &Game::pauseTimer, whiteTimer_, &Timer::pause);
    connect(game_, &Game::resetTimer, whiteTimer_, &Timer::reset);
    connect(whiteTimer_, &Timer::currentTimeText, game_, &Game::updateTimerText);
    connect(whiteTimer_, &Timer::expiredTime, game_, &Game::expiredTime);

    connect(game_, &Game::startTimer, blackTimer_, &Timer::start);
    connect(game_, &Game::pauseTimer, blackTimer_, &Timer::pause);
    connect(game_, &Game::resetTimer, blackTimer_, &Timer::reset);
    connect(blackTimer_, &Timer::currentTimeText, game_, &Game::updateTimerText);
    connect(blackTimer_, &Timer::expiredTime, game_, &Game::expiredTime);

    setMinimumSize(layout_->minimumSize());
    grabKeyboard();

    game_->reset();
}

Chess::~Chess()
{
    delete ui_;
    delete game_;
    delete layout_;
    delete whiteTimer_;
    delete blackTimer_;
}

void Chess::updateTimerLabels(const QString &text, bool top)
{
    if (top)
        ui_->topTimerLabel->setText(text);
    else
        ui_->bottomTimerLabel->setText(text);
}

void Chess::gameOver(const QString &color, const QString &victoryType)
{
    ui_->winColorLabel->setText(color);
    ui_->winTypeLabel->setText(victoryType);
    ui_->winFrame->show();
}

void Chess::openSettings()
{
    releaseKeyboard();
    settingsDialog_->open();
}

void Chess::updateSettings(int result)
{
    if (result) {
        Settings settings = settingsDialog_->saveSettings();

        QColor primaryColor;
        QColor secondaryColor;
        if (settings.selectedColorRow < 3) {
            primaryColor = settings.presetColors[2 * settings.selectedColorRow];
            secondaryColor = settings.presetColors[2 * settings.selectedColorRow + 1];
        } else {
            primaryColor = settings.primaryCustomColor;
            secondaryColor = settings.secondaryCustomColor;
        }

        Square::setColors(primaryColor, secondaryColor);

        game_->setTimeControl(settings.whiteTime, settings.blackTime,
                              settings.whiteIncrement, settings.blackIncrement);
        game_->setFlipBoard(settings.flipBoard);

        if (settings.resetGame)
            game_->reset();
        else
            game_->updateGame();
    } else
        settingsDialog_->revertSettings();

    grabKeyboard();
}

void Chess::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down)
        emit keyPress(event->key());
    else if (event->key() == Qt::Key_Left)
        emit keyPress(event->key());
    else if (event->key() == Qt::Key_Right)
        emit keyPress(event->key());
    else if (event->key() == Qt::Key_Up)
        emit keyPress(event->key());

    QWidget::keyPressEvent(event);
}

Settings Chess::initialSettings()
{
    Settings settings = Settings();
    settings.twoPlayer = true;
    settings.flipBoard = true;
    settings.startWhiteVsComputer = true;
    settings.whiteTime = 150000;
    settings.blackTime = 150000;
    settings.whiteIncrement = 5000;
    settings.blackIncrement = 5000;
    settings.selectedColorRow = 0;
    settings.primaryCustomColor = QColorConstants::White;
    settings.secondaryCustomColor = QColorConstants::DarkGray;
    settings.resetGame = false;

    return settings;
}
