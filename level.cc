#include "level.h"
#include <limits>
using namespace std;

Level1::Level1(Colour c): Computer{c} {}
Level2::Level2(Colour c): Computer{c} {}
Level3::Level3(Colour c): Computer{c} {}
Level4::Level4(Colour c): Computer{c} {}

Posn Level1::genMove(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<Posn> moves;
    int randNum;

    moves = collectLegalMoves(grid);

    randNum = randNumGen(0, moves.size() - 1);

    if (moves.size() > 0) {
        grid[moves[randNum].fromR][moves[randNum].fromC]->addToHistory(moves[randNum]);
        return moves[randNum];
    }
}


Posn Level2::genMove(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<Posn> moves;
    vector<Posn> checksAndCaptures;

    int randNum;

    moves = collectLegalMoves(grid);
    
    // Collect checking and capturing moves
    for (int i = 0; i < moves.size(); ++i) {
        if (isCheckingMove(grid, moves[i]) || isCapturingMove(grid, moves[i])) {
            checksAndCaptures.emplace_back(moves[i]);
        }
    }

    // Return a move preferring checks and captures
    if (checksAndCaptures.size() > 0) {
        randNum = randNumGen(0, checksAndCaptures.size() - 1);
        grid[checksAndCaptures[randNum].fromR][checksAndCaptures[randNum].fromC]->addToHistory(checksAndCaptures[randNum]);
        return checksAndCaptures[randNum];
    } else {
        randNum = randNumGen(0, moves.size() - 1);
        grid[moves[randNum].fromR][moves[randNum].fromC]->addToHistory(moves[randNum]);        
        return moves[randNum];
    }
}


Posn Level3::genMove(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<Posn> moves;
    vector<Posn> enemyMoves;
    vector<Posn> defensiveMoves;
    vector<Posn> defensiveAndAttackingMoves;
    int randNum;

    moves = collectLegalMoves(grid);
    enemyMoves = genEnemyMoves(grid);
    
    for (int i = 0; i < moves.size(); ++i) {
        if ((isCheckingMove(grid, moves[i]) || isCapturingMove(grid, moves[i])) && 
            moveAvoidsCapture(grid, enemyMoves, moves[i])) {

            defensiveAndAttackingMoves.emplace_back(moves[i]);
        }
    }

    // Collect moves which avoid capture, capturing moves, and checks
    for (int i = 0; i < moves.size(); ++i) {
        if (!(isCheckingMove(grid, moves[i]) || isCapturingMove(grid, moves[i])) && 
            moveAvoidsCapture(grid, enemyMoves, moves[i])) {

            defensiveMoves.emplace_back(moves[i]);
        }
    }

    // Return a move which avoids capture, capturing moves, and checks
    if (defensiveAndAttackingMoves.size() > 0) {
        randNum = randNumGen(0, defensiveAndAttackingMoves.size() - 1);
        grid[defensiveAndAttackingMoves[randNum].fromR][defensiveAndAttackingMoves[randNum].fromC]->addToHistory(defensiveAndAttackingMoves[randNum]);    
        return defensiveAndAttackingMoves[randNum];
    } else if (defensiveMoves.size() > 0) {
        randNum = randNumGen(0, defensiveMoves.size() - 1);
        grid[defensiveMoves[randNum].fromR][defensiveMoves[randNum].fromC]->addToHistory(defensiveMoves[randNum]);    
        return defensiveMoves[randNum];
    } else {
        randNum = randNumGen(0, moves.size() - 1);
        grid[moves[randNum].fromR][moves[randNum].fromC]->addToHistory(moves[randNum]);
        return moves[randNum];
    }
}


Posn Level4::genMove(const vector<vector<unique_ptr<Piece>>> &grid) {
    vector<Posn> moves;
    vector<Posn> enemyMoves;
    vector<Posn> equalEvalMoves;
    int randNum;
    int currentEval;
    int bestMoveEval = numeric_limits<int>::min();

    moves = collectLegalMoves(grid);
    
    // Select move index with best evaluation
    for (int i = 0; i < moves.size(); ++i) {
        auto futureGrid = genFutureGrid(moves[i], grid);
        enemyMoves = genEnemyMoves(futureGrid);
        currentEval = moveEval(grid, enemyMoves, moves[i]);

        if (currentEval > bestMoveEval) {
            bestMoveEval = currentEval;
        }
    }
    
    // Collect all moves with eval equal to the best eval
    for (int i = 0; i < moves.size(); ++i) {
        if (moveEval(grid, enemyMoves, moves[i]) == bestMoveEval) {
            equalEvalMoves.emplace_back(moves[i]);
        }
    }

    // Pick a random move of those with equal eval
    randNum = randNumGen(0, equalEvalMoves.size() - 1);

    if (equalEvalMoves.size() == 0) {
       randNum = randNumGen(0, moves.size() - 1);
       return moves[randNum]; 
    }
    
    grid[equalEvalMoves[randNum].fromR][equalEvalMoves[randNum].fromC]->addToHistory(equalEvalMoves[randNum]);
    return equalEvalMoves[randNum];
}
