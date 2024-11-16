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

BoardResult GameState::checkForEndCondition(int ix)
{
    static constexpr int diagonalTopLeft[3] = {0,4,8};
    static constexpr int diagonalTopRight[3] = {2,4,6};

    const SlotState state = getSlotAt(ix);

    int columnId = ix % 3;
    int rowId = ix / 3;

    bool columnWin = true;
    bool rowWin = true;
    bool diagonalTopLeftWin = ix == diagonalTopLeft[0] || ix == diagonalTopLeft[1] || ix == diagonalTopLeft[2];
    bool diagonalTopRightWin = ix == diagonalTopRight[0] || ix == diagonalTopRight[1] || ix == diagonalTopRight[2];
    bool allChecked = true;

    for (int i = 0; i < 3 && columnWin; i++) {
        columnWin = getSlotAt(i*3 + columnId) == state;
    }

    for (int i = 0; i < 3 && rowWin; i++) {
        rowWin = getSlotAt(rowId*3 + i) == state;
    }

    for (int i = 0; i < 3 && diagonalTopLeftWin; i++) {
        diagonalTopLeftWin = getSlotAt(diagonalTopLeft[i]) == state;
    }

    for (int i = 0; i < 3 && diagonalTopRightWin; i++) {
        diagonalTopRightWin = getSlotAt(diagonalTopRight[i]) == state;
    }

    for (int i = 0; i < 9 && allChecked; i++) {
        allChecked = getSlotAt(i) != SlotState::Empty;
    }

    BoardResult result;
    result.isFinished = columnWin || rowWin || diagonalTopLeftWin || diagonalTopRightWin || allChecked;
    result.winningColumn = columnWin ? columnId : -1;
    result.winningRow = rowWin ? rowId : -1;
    result.isTopLeftDiagonalWinning = diagonalTopLeftWin;
    result.isTopRightDiagonalWinning = diagonalTopRightWin;
    return result;
}
