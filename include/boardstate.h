#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <array>

struct BoardResult {
    Q_GADGET
    QML_NAMED_ELEMENT(boardResult)
    Q_PROPERTY(bool isFinished MEMBER isFinished)
    Q_PROPERTY(int winningColumn MEMBER winningColumn)
    Q_PROPERTY(int winningRow MEMBER winningRow)
    Q_PROPERTY(bool isTopLeftDiagonalWinning MEMBER isTopLeftDiagonalWinning)
    Q_PROPERTY(bool isTopRightDiagonalWinning MEMBER isTopRightDiagonalWinning)
public:
    bool isFinished = false;
    int winningColumn = -1;
    int winningRow = -1;
    bool isTopLeftDiagonalWinning = false;
    bool isTopRightDiagonalWinning = false;
};

class BoardState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum class Field {
        Empty,
        Nough,
        Cross,
    }; Q_ENUM(Field)

    explicit BoardState(QObject *parent = nullptr);

    const std::array<Field,9> & getFields() const;
    void setFieldAt(int ix, Field field);
    Q_INVOKABLE Field getFieldAt(int ix) const;

    BoardResult checkForEndCondition(int ix);

    static constexpr const int diagonalTopLeft[3] = {0,4,8};
    static constexpr const int diagonalTopRight[3] = {2,4,6};

protected:
    std::array<Field,9> m_fields;
};

#endif // BOARDSTATE_H
