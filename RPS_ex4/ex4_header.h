#ifndef __EX4_HEADER__
#define __EX4_HEADER__

#include <memory>
#include <utility>
#include <array>
#include <tuple>
#include <cstdio>
#include <iostream>

using std::get;
using std::cout;
using std::endl;

template<typename GAME_PIECE>
using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;

typedef enum { AllPiecesOfPlayer , AllOccureneceOfPiece, AllOccureneceOfPieceForPlayer} condition_t;
 
template <int ROWS, 
		  int COLS, 
		  class GAME_PIECE, 
		  int NumberOfPlayers = 2>

class GameBoard
{
	std::array<PieceInfo<GAME_PIECE>, ROWS*COLS> table;

	class Iterator
	{
		PieceInfo<GAME_PIECE>* cellPtr;
		int row;
		int col;

		void advanceOneCell()
		{
			if (!(row == ROWS - 1 && col == COLS - 1) && cellPtr)
			{
				cellPtr++;

				if (col == COLS - 1)
				{
					row++;
					col = 0;
				}
				else
				{
					col++;
				}
			}
			else
			{
				cellPtr = nullptr;
				row = -1;
				col = -1;
			}
		}



	public:

		Iterator() : cellPtr(nullptr), row(-1), col(-1) {}

		Iterator(PieceInfo<GAME_PIECE>* ptr, int row, int col) : cellPtr(ptr), row(row), col(col) {}

		Iterator operator++()
		{
			advanceToNextExistingCell();

			return *this;
		}

		bool operator!=(const Iterator & other)
		{
			return ((cellPtr != other.cellPtr) && (row != other.row) && (col != other.col));
		}

		const std::tuple<int, int, GAME_PIECE, int> operator*() const
		{
			return std::tuple<int, int, GAME_PIECE, int>(row, col, GAME_PIECE((*cellPtr)->second), (*cellPtr)->first);
		}

		bool exists()
		{
			if (row == -1 || col == -1)
				return false;

			if (!cellPtr)
				return false;

			if((*cellPtr)->first == -1)
				return false;

			return true;

		}

		void advanceToNextExistingCell()
		{
			bool done = false;
			do
			{
				if (!done)
					done = true;
				else
					printf("row %d col %d does not exist\n", row, col);
				advanceOneCell();

				if (row == -1 || col == -1)
					break;

			} while ((*cellPtr)->first == -1);
		}

	};

public:

	GameBoard() 
	{
		for (auto& cell : table)
			cell = std::move(std::make_unique<const std::pair<int, GAME_PIECE>>(-1, GAME_PIECE()));
		
	};

	PieceInfo<GAME_PIECE> getPiece(int row, int col)
	{
		if(col < 0 || row < 0 || row >= ROWS || col >= COLS)
			return nullptr;

		if (table[row*COLS + col]->first == -1)
		{
			return nullptr;
		}
		else
		{
			auto& cell = table[row*ROWS + col];
			return std::make_unique<const std::pair<int, GAME_PIECE>>(cell->first, GAME_PIECE(cell->second));
		}
	}
	
	PieceInfo<GAME_PIECE> setPiece(int row, int col, GAME_PIECE piece, int player)
	{
		if (col < 0 || row < 0 || row >= ROWS || col >= COLS || player >= NumberOfPlayers)
			return nullptr;

		auto ret = std::move(getPiece(row, col));

		table[row*COLS + col] = std::move(std::make_unique<const std::pair<int, GAME_PIECE>>(player, GAME_PIECE(piece)));

		return std::move(ret);
	}
	
	Iterator begin()
	{
		Iterator it(&table[0], 0, 0);
		if (!it.exists())
		{
			printf("row 0 col 0 does not exist\n");

			it.advanceToNextExistingCell();
		}
		return it;
	}

	Iterator end()
	{
		return Iterator();
	}

	struct conditionalIterator
	{
		const GameBoard& m_gameBoard;
		int m_playerNum;
		condition_t m_condition;
		GAME_PIECE m_piece;

		conditionalIterator(const GameBoard& _gameBoard, int _playerNum, condition_t _condition)
			: m_playerNum(_playerNum), m_gameBoard(_gameBoard), m_condition(_condition) {}

		conditionalIterator(const GameBoard& _gameBoard, GAME_PIECE _piece, condition_t _condition)
			: m_piece(_piece), m_gameBoard(_gameBoard), m_condition(_condition) {}

		conditionalIterator(const GameBoard& _gameBoard, GAME_PIECE _piece, int _playerNum, condition_t _condition)
			: m_piece(_piece), m_playerNum(_playerNum), m_gameBoard(_gameBoard), m_condition(_condition) {}

		class iterator2
		{
			class conditionalIterator& m_condIter;
			int m_index;

		public:
			iterator2(int _index, conditionalIterator& _condIter)
				: m_index(_index), m_condIter(_condIter) {}
			
			iterator2 operator++()
			{
				m_index++;
				int i;
				bool found = false;
				for (i = m_index; i < ROWS*COLS ; i++)
				{
					switch (m_condIter.m_condition)
					{
					case AllPiecesOfPlayer:
						if (m_condIter.m_gameBoard.table[i]->first == m_condIter.m_playerNum)
						{
							found = true;
						}
						break;
					case AllOccureneceOfPiece:
						if (m_condIter.m_gameBoard.table[i]->second == m_condIter.m_piece)
						{
							found = true;
						}
						break;
					case AllOccureneceOfPieceForPlayer:
						if (m_condIter.m_gameBoard.table[i]->first == m_condIter.m_playerNum && 
							m_condIter.m_gameBoard.table[i]->second == m_condIter.m_piece)
						{
							found = true;
						}
						break;
					}
					if (found) break;
				}
				m_index = i;
				return *this;
			}

			bool operator!=(const iterator2& other)
			{
				return (m_index != other.m_index);
			}

			const std::tuple<int, int, GAME_PIECE, int> operator*() const
			{
				return std::tuple<int, int, GAME_PIECE, int>(m_index / COLS, m_index % COLS, GAME_PIECE(m_condIter.m_gameBoard.table[m_index]->second), m_condIter.m_gameBoard.table[m_index]->first);
			}
		};

		iterator2 begin()
		{
			int size = ROWS*COLS;
			bool found = false;

			for (int i = 0; i < size; i++)
			{
				switch (m_condition)
				{

				case AllPiecesOfPlayer:
					if (m_gameBoard.table[i]->first == m_playerNum)
					{
						return iterator2(i, *this);
					}
					break;

				case AllOccureneceOfPiece:
					if (m_gameBoard.table[i]->second == m_piece)
					{
						return iterator2(i, *this);
					}
					break;

				case AllOccureneceOfPieceForPlayer:
					if (m_gameBoard.table[i]->first == m_playerNum && m_gameBoard.table[i]->second == m_piece)
					{
						return iterator2(i, *this);
					}
					break;
				}
			}
			return iterator2(ROWS*COLS, *this); //did not find  player's piece
		}

		iterator2 end()
		{
			return iterator2(ROWS*COLS, *this);
		}
	};

	conditionalIterator allPiecesOfPlayer(int playerNum)
	{
		return conditionalIterator(*this, playerNum, AllPiecesOfPlayer);
	}
	
	conditionalIterator allOccureneceOfPiece(GAME_PIECE piece)
	{
		return conditionalIterator(*this, piece, AllOccureneceOfPiece);
	}

	conditionalIterator allOccureneceOfPieceForPlayer(GAME_PIECE piece, int playerNum)
	{
		return conditionalIterator(*this, piece, playerNum, AllOccureneceOfPieceForPlayer);
	}
};

#endif // ifndef __EX4_HEADER__