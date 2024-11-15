#include "gamemaster.h"

#include "gamestate.h"

GameMaster::GameMaster(QObject *parent)
    : QObject{parent}
{
    m_gameState = new GameState(this);
    m_crossPlayer = new HumanPlayer(Player::Team::Crosses, this);
    m_noughPlayer = new Player(Player::Team::Noughs, this);

    connect(m_crossPlayer, &Player::endTurn, this, &GameMaster::crossMove);
    connect(m_noughPlayer, &Player::endTurn, this, &GameMaster::noughMove);
}

void GameMaster::restart(Initiative newInitiative)
{
    m_initiative = newInitiative;
    m_lastMove = -1;

    for(int i = 0; i < 9; i++){
        m_gameState->setSlotAt(i, GameState::SlotState::Empty);
    }

    askNext();
}

HumanPlayer * GameMaster::getCurrentHumanPlayer()
{
    switch(m_initiative) {
    case Initiative::Cross:
        return dynamic_cast<HumanPlayer*>(m_crossPlayer);
    case Initiative::Nough:
        return dynamic_cast<HumanPlayer*>(m_noughPlayer);
    default:
        return nullptr;
    }
}

void GameMaster::noughMove(int ix)
{
    performMove(Initiative::Nough, ix);
}

void GameMaster::crossMove(int ix)
{
    performMove(Initiative::Cross, ix);
}

void GameMaster::performMove(Initiative who, int ix)
{
    using SlotState = GameState::SlotState;
    if (m_initiative != who)
        return;

    Initiative other = who == Initiative::Cross ? Initiative::Nough : Initiative::Cross;
    SlotState state = who == Initiative::Cross ? SlotState::Cross : SlotState::Nough;

    if (m_gameState->getSlotAt(ix) == GameState::SlotState::Empty) {
        m_gameState->setSlotAt(ix, state);
        if (checkForEndCondition(ix))
            m_initiative = Initiative::Finish;
        else
            m_initiative = other;

        emit boardUpdated();
    }

    askNext();
}

void GameMaster::askNext()
{
    switch(m_initiative) {
    case Initiative::Cross:
        m_crossPlayer->startTurn(m_gameState, m_lastMove);
        break;
    case Initiative::Nough:
        m_noughPlayer->startTurn(m_gameState, m_lastMove);
        break;
    default:
        break;
    }
}

bool GameMaster::checkForEndCondition(int ix)
{
    static constexpr int diagonalTopLeft[3] = {0,4,8};
    static constexpr int diagonalTopRight[3] = {2,4,6};

    const GameState::SlotState state = m_gameState->getSlotAt(ix);

    int columnId = ix % 3;
    int rowId = ix / 3;

    bool columnWin = true;
    bool rowWin = true;
    bool diagonalTopLeftWin = ix == diagonalTopLeft[0] || ix == diagonalTopLeft[1] || ix == diagonalTopLeft[2];
    bool diagonalTopRightWin = ix == diagonalTopRight[0] || ix == diagonalTopRight[1] || ix == diagonalTopRight[2];

    for (int i = 0; i < 3 && columnWin; i++) {
        columnWin = m_gameState->getSlotAt(i*3 + columnId) == state;
    }

    for (int i = 0; i < 3 && rowWin; i++) {
        rowWin = m_gameState->getSlotAt(rowId*3 + i) == state;
    }

    for (int i = 0; i < 3 && diagonalTopLeftWin; i++) {
        diagonalTopLeftWin = m_gameState->getSlotAt(diagonalTopLeft[i]) == state;
    }

    for (int i = 0; i < 3 && diagonalTopRightWin; i++) {
        diagonalTopRightWin = m_gameState->getSlotAt(diagonalTopRight[i]) == state;
    }

    return columnWin || rowWin || diagonalTopLeftWin || diagonalTopRightWin;
}
