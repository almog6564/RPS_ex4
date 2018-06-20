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
			//printf("!=\t (%p,%p) (%d,%d) (%d,%d)\n", cellPtr, other.cellPtr, row, other.row, col, other.col);
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

#define IteratorType const std::tuple<int, int, GAME_PIECE, int>

	

	GameBoard() 
	{
		//table(ROWS, std::vector<int /*PieceInfo<GAME_PIECE*/>>(COLS, -1/*std::make_unique<PieceInfo<GAME_PIECE>>(-1, GAME_PIECE())*/));
		
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

	class conditionalIterator
	{
		const GameBoard* m_gameBoard;
		int m_playerNum;
		int m_ROWS;
		int m_COLS;
		condition_t m_condition;
		GAME_PIECE m_piece;
	public:
		conditionalIterator(const GameBoard* _gameBoard, int _playerNum, int _ROWS, int _COLS, condition_t _condition)
			: m_playerNum(_playerNum), m_gameBoard(_gameBoard), m_ROWS(_ROWS), m_COLS(_COLS), m_condition(_condition) {}

		conditionalIterator(const GameBoard* _gameBoard, GAME_PIECE _piece, int _ROWS, int _COLS, condition_t _condition)
			: m_piece(_piece), m_gameBoard(_gameBoard), m_ROWS(_ROWS), m_COLS(_COLS), m_condition(_condition) {}

		conditionalIterator(const GameBoard* _gameBoard, GAME_PIECE _piece, int _playerNum, int _ROWS, int _COLS, condition_t _condition)
			: m_piece(_piece), m_playerNum(_playerNum), m_gameBoard(_gameBoard), m_ROWS(_ROWS), m_COLS(_COLS), m_condition(_condition) {}

		class iterator1
		{
			int m_index;
			int m_innerSize;
			int m_innerPlayerNum;
			const GameBoard* m_innerGameBoard;
			int m_innerRows;
			int m_innerCols;
			condition_t m_innerCondition;
			GAME_PIECE m_innerPiece;
						
		public:
			iterator1(int _index, int _size, int _playerNum, const GameBoard* _gameBoard, int _ROWS, int _COLS, condition_t _condition)
				: m_index(_index), m_innerSize(_size), m_innerPlayerNum(_playerNum), m_innerGameBoard(_gameBoard), m_innerRows(_ROWS), m_innerCols(_COLS), m_innerCondition(_condition)  {}

			iterator1(int _index, int _size, GAME_PIECE _piece, const GameBoard* _gameBoard, int _ROWS, int _COLS, condition_t _condition)
				: m_index(_index), m_innerSize(_size), m_innerPiece(_piece), m_innerGameBoard(_gameBoard), m_innerRows(_ROWS), m_innerCols(_COLS), m_innerCondition(_condition) {}

			iterator1(int _index, int _size, GAME_PIECE _piece, int _playerNum, const GameBoard* _gameBoard, int _ROWS, int _COLS, condition_t _condition)
				: m_index(_index), m_innerSize(_size), m_innerPiece(_piece), m_innerPlayerNum(_playerNum), m_innerGameBoard(_gameBoard), m_innerRows(_ROWS), m_innerCols(_COLS), m_innerCondition(_condition) {}
			
			iterator1(int _maxSize) : m_index(_maxSize) {}

			iterator1 operator++()
			{
				m_index++;
				int i;
				bool found = false;
				for (i = m_index; i < m_innerSize ; i++)
				{
					switch (m_innerCondition)
					{
					case AllPiecesOfPlayer:
						if (m_innerGameBoard->table[i]->first == m_innerPlayerNum)
						{
							found = true;
						}
						break;
					case AllOccureneceOfPiece:
						if (m_innerGameBoard->table[i]->second == m_innerPiece)
						{
							found = true;
						}
						break;
					case AllOccureneceOfPieceForPlayer:
						if (m_innerGameBoard->table[i]->first == m_innerPlayerNum && m_innerGameBoard->table[i]->second == m_innerPiece)
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

			bool operator!=(const iterator1& other)
			{
				return (m_index != other.m_index);
			}

			const std::tuple<int, int, GAME_PIECE, int> operator*() const
			{
				return std::tuple<int, int, GAME_PIECE, int>(m_index / m_innerRows, m_index % m_innerRows, GAME_PIECE(m_innerGameBoard->table[m_index]->second), m_innerPlayerNum);
			}
		};

		iterator1 begin()
		{
			int size = m_gameBoard->table.size();

			for (int i = 0; i < size; i++)
			{
				switch (m_condition)
				{
				case AllPiecesOfPlayer:
					if (m_gameBoard->table[i]->first == m_playerNum)
					{
						cout << "begin found piece" << endl;
						return iterator1(i, size, m_playerNum, m_gameBoard, m_ROWS, m_COLS, m_condition);
					}
					break;
				case AllOccureneceOfPiece:
					if (m_gameBoard->table[i]->second == m_piece)
					{
						cout << "begin found piece" << endl;
						return iterator1(i, size, m_piece, m_gameBoard, m_ROWS, m_COLS, m_condition);
					}
					break;
				case AllOccureneceOfPieceForPlayer:
					if (m_gameBoard->table[i]->first == m_playerNum && m_gameBoard->table[i]->second == m_piece)
					{
						cout << "begin found piece" << endl;
						return iterator1(i, size, m_piece, m_playerNum, m_gameBoard, m_ROWS, m_COLS, m_condition);
					}
					break;
				}
				
				
			}
			return iterator1(m_gameBoard->table.size()); //did not find  player's piece
		}

		iterator1 end()
		{
			return iterator1(m_gameBoard->table.size());
		}

	};

	conditionalIterator allPiecesOfPlayer(int playerNum)
	{
		return conditionalIterator(this, playerNum, ROWS, COLS, AllPiecesOfPlayer);
	}
	
	conditionalIterator allOccureneceOfPiece(GAME_PIECE piece)
	{
		return conditionalIterator(this, piece, ROWS, COLS, AllOccureneceOfPiece);
	}

	conditionalIterator allOccureneceOfPieceForPlayer(GAME_PIECE piece, int playerNum)
	{
		return conditionalIterator(this, piece, playerNum, ROWS, COLS, AllOccureneceOfPieceForPlayer);
	}
};








#endif // ifndef __EX4_HEADER__

