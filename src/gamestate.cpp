#include "gamestate.h"

GameState::GameState(QObject *parent)
    : QObject{parent}
{
    restart(Initiative::Opponent);
}

const std::array<GameState::SlotState,9> & GameState::getSlots() const
{
    return m_slots;
}

void GameState::setSlotAt(int ix, SlotState state)
{

}

GameState::SlotState GameState::getSlotAt(int ix) const
{
    return SlotState::Empty;
}

void GameState::restart(Initiative newInitiative)
{

}
