#include "player.h"

#include <QtConcurrent/QtConcurrent>

Player::Player(Team team, QObject *parent)
    : QObject{parent}, m_team(team)
{}

Player::~Player()
{
    m_worker.waitForFinished();
}

void Player::startTurn(const BoardState * boardState, int lastMove)
{
    m_worker = play(boardState, lastMove);
    m_worker.then([this](int result) {
        emit endTurn(result);
    }).onFailed([this] {
        emit endTurn(-1);
    });
}

HumanPlayer::~HumanPlayer()
{
    m_selection = -1;
    m_mutex.tryLock();
    m_mutex.unlock();
}

QFuture<int> HumanPlayer::play(const BoardState * boardState, int lastMove)
{
    m_mutex.lock();
    return QtConcurrent::run([this]() -> int {
        //Wait for the unlock by selectSlot
        m_mutex.lock();
        int ret = m_selection;
        m_mutex.unlock();
        return ret;
    });
}

void HumanPlayer::selectSlot(int ix)
{
    m_mutex.unlock();
    m_selection = ix;
}

ComputerPlayer::ComputerPlayer(Team team, QObject *parent) : Player(team,parent)
{
    m_random = QRandomGenerator::securelySeeded();
}

QFuture<int> ComputerPlayer::play(const BoardState * boardState, int lastMove)
{
    const BoardState::Field myField = m_team == Team::Crosses ? BoardState::Field::Cross : BoardState::Field::Nough;
    const BoardState::Field otherField = m_team == Team::Crosses ? BoardState::Field::Nough : BoardState::Field::Cross;

    QSet<int> availableFields;
    availableFields.reserve(9);
    std::array<QVector<int>,3> availableColumns;
    std::array<QVector<int>,3> availableRows;
    QVector<int> availableTopLeftDiagonal;
    QVector<int> availableTopRightDiagonal;
    std::array<int,3> myColumns = {0,0,0};
    std::array<int,3> otherColumns = {0,0,0};
    std::array<int,3> myRows = {0,0,0};
    std::array<int,3> otherRows = {0,0,0};
    int myTopLeftDiagonal = 0;
    int otherTopLeftDiagonal = 0;
    int myTopRightDiagonal = 0;
    int otherTopRightDiagonal = 0;

    int result = -1;

    for(int i = 0; i < 9; i++) {
        const int x = i % 3;
        const int y = i / 3;
        BoardState::Field field = boardState->getFieldAt(i);
        if (field == BoardState::Field::Empty) {
            availableFields.insert(i);
            availableColumns[x].append(i);
            availableRows[y].append(i);
        } else if (field == myField) {
            myColumns[x] += 1;
            myRows[y] += 1;
        } else {
            otherColumns[x] += 1;
            otherRows[y] += 1;
        }
    }

    for (int i : BoardState::diagonalTopLeft) {
        BoardState::Field field = boardState->getFieldAt(i);
        if (field == BoardState::Field::Empty) {
            availableTopLeftDiagonal.append(i);
        } else if (field == myField) {
            myTopLeftDiagonal += 1;
        } else {
            otherTopLeftDiagonal += 1;
        }
    }

    for (int i : BoardState::diagonalTopRight) {
        BoardState::Field field = boardState->getFieldAt(i);
        if (field == BoardState::Field::Empty) {
            availableTopRightDiagonal.append(i);
        } else if (field == myField) {
            myTopRightDiagonal += 1;
        } else {
            otherTopRightDiagonal += 1;
        }
    }

    if (availableFields.isEmpty())
        return sendMove(-1);

    QVector<int> possibleWinning;
    //First, look if there's a possibility of winning.
    for (int i = 0; i < 3; i++) {
        if (myColumns[i] == 2 && otherColumns[i] == 0) {
            possibleWinning.append(availableColumns[i][0]);
        }
        if (myRows[i] == 2 && otherRows[i] == 0) {
            possibleWinning.append(availableRows[i][0]);
        }
    }

    if (myTopLeftDiagonal == 2 && otherTopLeftDiagonal == 0) {
        possibleWinning.append(availableTopLeftDiagonal[0]);
    }

    if (myTopRightDiagonal == 2 && otherTopRightDiagonal == 0) {
        possibleWinning.append(availableTopRightDiagonal[0]);
    }

    //Try to use the winning move.
    for(int i : possibleWinning)
        availableFields.remove(i);

    bool useWinning = availableFields.isEmpty();
    useWinning = useWinning || (not possibleWinning.isEmpty() && m_random.generateDouble() < 0.95);

    if (useWinning) {
        return sendMove(possibleWinning[m_random.bounded(0, possibleWinning.size())]);
    }

    //Try to block winning move of the opponent.
    QVector<int> possibleLosing;
    for (int i = 0; i < 3; i++) {
        if (myColumns[i] == 0 && otherColumns[i] == 2) {
            possibleLosing.append(availableColumns[i][0]);
        }
        if (myRows[i] == 0 && otherRows[i] == 2) {
            possibleLosing.append(availableRows[i][0]);
        }
    }

    if (myTopLeftDiagonal == 0 && otherTopLeftDiagonal == 2) {
        possibleLosing.append(availableTopLeftDiagonal[0]);
    }

    if (myTopRightDiagonal == 0 && otherTopRightDiagonal == 2) {
        possibleLosing.append(availableTopRightDiagonal[0]);
    }

    //Attempt to choose the losing one. if not...

    for (int i : possibleLosing) {
        availableFields.remove(i);
    }

    bool useLosing = availableFields.isEmpty();
    useLosing = useLosing || (not possibleLosing.isEmpty() && m_random.generateDouble() < 0.95);

    if (useLosing) {
        return sendMove(possibleLosing[m_random.bounded(0, possibleLosing.size())]);
    }

    //Calculate the weights for choices.
    double totalWeight = 0;
    QVector<QPair<int,double>> weighted;
    for(int i : availableFields) {
        const int x = i % 3;
        const int y = i / 3;

        double weight = 5.0;
        weight += (otherColumns[x] == 0) * (10 + 25 * myColumns[x]);
        weight += (otherRows[y] == 0) * (10 + 25 * myRows[y]);
        weight += ((i == 0 || i == 4 || i == 8) && otherTopLeftDiagonal == 0) * (10 + 25 * myTopLeftDiagonal);
        weight += ((i == 2 || i == 4 || i == 6) && otherTopRightDiagonal == 0) * (10 + 25 * myTopRightDiagonal);
        weighted.append({i,weight});
        totalWeight += weight;
    }

    for (auto & [i, weight] : weighted) {
        weight /= totalWeight;
    }

    double chosenWeight = m_random.generateDouble();
    totalWeight = 0;
    for (const auto & [i, weight] : weighted) {
        totalWeight += weight;
        if (chosenWeight < totalWeight) {
            return sendMove(i);
        }
    }

    //Failsafe
    return sendMove(weighted[m_random.bounded(0, weighted.size())].first);
}

QFuture<int> ComputerPlayer::sendMove(int move)
{
    return QtConcurrent::run([move]() -> int {
        QThread::msleep(QRandomGenerator::global()->bounded(150,600));
        return move;
    });
}
