#include "chess.h"

#include <QKeyEvent>
#include <QHeaderView>
#include <QWidget>

#include "ui_chess.h"
#include "game.h"
#include "chess_layout.h"
#include "square.h"
#include "promotion_square.h"
#include "timer.h"
#include "position.h"
#include "piece.h"
#include "notation_model.h"
#include "settings.h"

using namespace std;

Chess::Chess(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::Chess),
    twoPlayer_(true),
    flipBoard_(true),
    startWhiteVsComputer_(true),
    whiteTime_(150000),
    blackTime_(150000),
    whiteIncrement_(5000),
    blackIncrement_(5000),
    primaryCustomColor_(QColorConstants::White),
    secondaryCustomColor_(QColorConstants::DarkGray),
    selectedColorRow_(0)
{
    ui_->setupUi(this);
    connect(ui_->newGame, &QPushButton::released, ui_->winFrame, &QFrame::hide);
    connect(ui_->winNewGame, &QPushButton::released, ui_->winFrame, &QFrame::hide);
    connect(ui_->settingsButton, &QPushButton::released, this, &Chess::openSettings);

    game_ = new Game(this);
    connect(game_, &Game::updateTimerLabels, this, &Chess::updateTimerLabels);
    connect(game_, &Game::gameEnded, this, &Chess::gameOver);
    connect(ui_->newGame, &QPushButton::released, game_, &Game::resetGame);
    connect(ui_->winNewGame, &QPushButton::released, game_, &Game::resetGame);
    connect(this, &Chess::keyPress, game_, &Game::keyPress);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    leftLayout->addWidget(ui_->newGame, 0, Qt::AlignTop);
    leftLayout->addWidget(ui_->settingsButton, 0, Qt::AlignBottom);
    topLayout->addWidget(ui_->topTimerLabel, 0, Qt::AlignLeft);
    bottomLayout->addWidget(ui_->bottomTimerLabel, 0, Qt::AlignRight);

    model_ = new NotationModel(this);
    connect(game_, &Game::notateMove, model_, &NotationModel::addMove);
    connect(game_, &Game::clearNotation, model_, &NotationModel::clearMoves);
    connect(game_, &Game::notationMoveNumber, model_, &NotationModel::setMoveNumber);
    connect(model_, &NotationModel::updateShownMove, game_, &Game::updateShownMove);
    connect(model_, &NotationModel::scrollTable, ui_->table, &QTableView::scrollToBottom);
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

    layout_ = new ChessLayout(ui_->centralWidget, QMargins(5, 5, 5, 5), 5);
    layout_->addLayout(leftLayout, ChessLayout::West);
    layout_->addLayout(topLayout, ChessLayout::North);
    layout_->addLayout(bottomLayout, ChessLayout::South);
    layout_->addTable(ui_->table, ChessLayout::East);
    layout_->addFrame(ui_->winFrame, ChessLayout::EndScreen);

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

    Square::setColors(settingsPresetColors_[0], settingsPresetColors_[1]);

    ui_->winFrame->raise();
    ui_->winFrame->hide();

    whiteTimer_ = new Timer(this, true, whiteTime_, whiteIncrement_);
    blackTimer_ = new Timer(this, false, blackTime_, blackIncrement_);
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

    game_->setTimeControl(150000, 150000, 5000, 5000);

    whiteTimer_->updateText();
    blackTimer_->updateText();

    setMinimumSize(layout_->minimumSize());

    grabKeyboard();

    game_->updatePosition();
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
//    QColor presets[6];
    Settings *settings = new Settings(nullptr, twoPlayer_, flipBoard_, startWhiteVsComputer_,
                                      whiteTime_, blackTime_, whiteIncrement_, blackIncrement_,
                                      settingsPresetColors_, primaryCustomColor_, secondaryCustomColor_,
                                      selectedColorRow_);
    connect(settings, &Settings::closed, this, &Chess::settingsClosed);
    settings->show();
}

void Chess::settingsClosed(bool applied, bool humanVHuman, bool flipBoard, bool startWhite,
                           int whiteTime, int blackTime, int whiteIncrement, int blackIncrement,
                           QColor primary, QColor secondary, QColor primaryCustom,
                           QColor secondaryCustom, int selectedRow)
{
    if (applied) {
        bool reset = false;

        if (twoPlayer_ != humanVHuman)
            reset = true;
        twoPlayer_ = humanVHuman;

        flipBoard_ = flipBoard;

        if (!humanVHuman && startWhiteVsComputer_ != startWhite)
            reset = true;
        startWhiteVsComputer_ = startWhite;

        if (whiteTime_ != whiteTime)
            reset = true;
        whiteTime_ = whiteTime;

        if (whiteIncrement_ != whiteIncrement)
            reset = true;
        whiteIncrement_ = whiteIncrement;

        if (blackTime_ != blackTime)
            reset = true;
        blackTime_ = blackTime;

        if (blackIncrement_ != blackIncrement)
            reset = true;
        blackIncrement_ = blackIncrement;

        primaryCustomColor_ = primaryCustom;
        secondaryCustomColor_ = secondaryCustom;
        selectedColorRow_ = selectedRow;

        Square::setColors(primary, secondary);
        game_->setTimeControl(whiteTime, blackTime, whiteIncrement, blackIncrement);
        game_->setFlipBoard(flipBoard);
        if (reset)
            game_->resetGame();

        game_->updatePosition();
        game_->updateClocks();
    }

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
}
