#include "gamestate.h"

GameState::GameState(QObject *parent)
    : QObject{parent}
{
}

const std::array<GameState::SlotState,9> & GameState::getSlots() const
{
    return m_slots;
}

void GameState::setSlotAt(int ix, SlotState state)
{
    Q_ASSERT(ix >= 0 && ix < 9);

    m_slots[ix] = state;
}

GameState::SlotState GameState::getSlotAt(int ix) const
{
    Q_ASSERT(ix >= 0 && ix < 9);

    return m_slots[ix];
}
