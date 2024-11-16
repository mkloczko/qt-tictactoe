#include "gamemaster.h"

#include "boardstate.h"

GameMaster::GameMaster(QObject *parent)
    : QObject{parent}
{
    m_initiative = Initiative::Finish;
    m_boardState = new BoardState(this);
}

void GameMaster::restart(Initiative newInitiative, bool humanNough, bool humanCross)
{
    auto createPlayer = [this](Player::Team team, bool isHuman) -> Player* {
        if (isHuman)
            return new HumanPlayer(team,this);
        else
            return new ComputerPlayer(team,this);
    };

    if (m_crossPlayer)
        delete m_crossPlayer;
    if (m_noughPlayer)
        delete m_noughPlayer;

    m_crossPlayer = createPlayer(Player::Team::Crosses, humanCross);
    m_noughPlayer = createPlayer(Player::Team::Noughs, humanNough);

    connect(m_crossPlayer, &Player::endTurn, this, &GameMaster::crossMove);
    connect(m_noughPlayer, &Player::endTurn, this, &GameMaster::noughMove);

    m_initiative = newInitiative;

    for(int i = 0; i < 9; i++)
        m_boardState->setFieldAt(i, BoardState::Field::Empty);

    emit restarted();

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
    using Field = BoardState::Field;
    if (m_initiative != who || ix < 0 || ix >= 9)
        return;

    Initiative other = who == Initiative::Cross ? Initiative::Nough : Initiative::Cross;
    Field state = who == Initiative::Cross ? Field::Cross : Field::Nough;

    if (m_boardState->getFieldAt(ix) == Field::Empty) {
        m_boardState->setFieldAt(ix, state);
        emit boardUpdated(ix);

        BoardResult result = m_boardState->checkForEndCondition(ix);
        if (result.isFinished) {
            m_initiative = Initiative::Finish;
            emit finished(result);
        } else {
            m_initiative = other;
        }
    }

    askNext();
}

void GameMaster::askNext()
{
    if (not m_noughPlayer || not m_crossPlayer)
        return;

    switch(m_initiative) {
    case Initiative::Cross:
        m_crossPlayer->startTurn(m_boardState);
        break;
    case Initiative::Nough:
        m_noughPlayer->startTurn(m_boardState);
        break;
    default:
        break;
    }
}
