#include "boardstate.h"

BoardState::BoardState(QObject *parent)
    : QObject{parent}
{
}

const std::array<BoardState::Field,9> & BoardState::getFields() const
{
    return m_fields;
}

void BoardState::setFieldAt(int ix, Field field)
{
    Q_ASSERT(ix >= 0 && ix < 9);
    m_fields[ix] = field;
}

BoardState::Field BoardState::getFieldAt(int ix) const
{
    Q_ASSERT(ix >= 0 && ix < 9);
    return m_fields[ix];
}

BoardResult BoardState::checkForEndCondition(int ix)
{
    static constexpr int diagonalTopLeft[3] = {0,4,8};
    static constexpr int diagonalTopRight[3] = {2,4,6};

    const Field state = getFieldAt(ix);

    int columnId = ix % 3;
    int rowId = ix / 3;

    bool columnWin = true;
    bool rowWin = true;
    bool diagonalTopLeftWin = ix == diagonalTopLeft[0] || ix == diagonalTopLeft[1] || ix == diagonalTopLeft[2];
    bool diagonalTopRightWin = ix == diagonalTopRight[0] || ix == diagonalTopRight[1] || ix == diagonalTopRight[2];
    bool allChecked = true;

    for (int i = 0; i < 3 && columnWin; i++) {
        columnWin = getFieldAt(i*3 + columnId) == state;
    }

    for (int i = 0; i < 3 && rowWin; i++) {
        rowWin = getFieldAt(rowId*3 + i) == state;
    }

    for (int i = 0; i < 3 && diagonalTopLeftWin; i++) {
        diagonalTopLeftWin = getFieldAt(diagonalTopLeft[i]) == state;
    }

    for (int i = 0; i < 3 && diagonalTopRightWin; i++) {
        diagonalTopRightWin = getFieldAt(diagonalTopRight[i]) == state;
    }

    for (int i = 0; i < 9 && allChecked; i++) {
        allChecked = getFieldAt(i) != Field::Empty;
    }

    BoardResult result;
    result.isFinished = columnWin || rowWin || diagonalTopLeftWin || diagonalTopRightWin || allChecked;
    result.winningColumn = columnWin ? columnId : -1;
    result.winningRow = rowWin ? rowId : -1;
    result.isTopLeftDiagonalWinning = diagonalTopLeftWin;
    result.isTopRightDiagonalWinning = diagonalTopRightWin;
    return result;
}
