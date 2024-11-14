#include "gamemaster.h"

#include "gamestate.h"

GameMaster::GameMaster(QObject *parent)
    : QObject{parent}
{}

void GameMaster::restart(Initiative newInitiative)
{
    m_initiative = newInitiative;
    if (not m_gameState) {
        return;
    }

    for(int i = 0; i < 9; i++){
        m_gameState->setSlotAt(i, GameState::SlotState::Empty);
    }
}

GameMaster::Initiative GameMaster::getInitiative()
{
    return m_initiative;
}


bool GameMaster::playAt(int ix)
{
    if (not m_gameState
        || m_initiative == Initiative::Finish
        || m_gameState->getSlotAt(ix) != GameState::SlotState::Empty){
        return false;
    }

    GameState::SlotState slotType = m_initiative == Initiative::Player ? GameState::SlotState::Cross : GameState::SlotState::Nough;
    m_gameState->setSlotAt(ix, slotType);
    return true;
}
