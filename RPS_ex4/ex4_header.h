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

	class AllPiecesOfPlayer
	{
		const GameBoard* m_gameBoard;
		int m_playerNum;
		int m_ROWS;
		int m_COLS;
	public:
		AllPiecesOfPlayer(const GameBoard* _gameBoard, int _playerNum, int _ROWS, int _COLS)
			: m_playerNum(_playerNum), m_gameBoard(_gameBoard), m_ROWS(_ROWS), m_COLS(_COLS) {}
		class iterator1
		{
			int m_index;
			int m_innerSize;
			int m_innerPlayerNum;
			const GameBoard* m_innerGameBoard;
			int m_innerRows;
			int m_innerCols;
						
		public:
			iterator1(int _index, int _size, int _playerNum, const GameBoard* _gameBoard, int _ROWS, int _COLS)
				: m_index(_index), m_innerSize(_size), m_innerPlayerNum(_playerNum), m_innerGameBoard(_gameBoard), m_innerRows(_ROWS), m_innerCols(_COLS)  {}

			iterator1(int _maxSize) : m_index(_maxSize) {}

			iterator1 operator++()
			{
				m_index++;
				for (int i = m_index; i < m_innerSize ; i++)
				{
					if (m_innerGameBoard->table[i]->first == m_innerPlayerNum)
					{
						m_index = i;
						return *this;					
					}
				}
				m_index = m_innerSize;
				return *this;
			}

			bool operator!=(const iterator1& other)
			{
				return (m_index != other.m_index);
			}

			const std::tuple<int, int, GAME_PIECE, int> operator*() const
			{
				return std::tuple<int, int, GAME_PIECE, int>(m_index / m_innerRows, m_index % m_innerRows, GAME_PIECE(m_innerGameBoard->table[m_index]->second), m_innerGameBoard->table[m_index]->first);
			}
		};

		iterator1 begin()
		{
			int size = m_gameBoard->table.size();

			for (int i = 0; i < size; i++)
			{
				if (m_gameBoard->table[i]->first == m_playerNum)
				{
					cout << "begin found piece" << endl;
					return iterator1(i, size , m_playerNum, m_gameBoard, m_ROWS, m_COLS);
				}
			}
			return iterator1(m_gameBoard->table.size()); //did not find  player's piece
		}

		iterator1 end()
		{
			return iterator1(m_gameBoard->table.size());
		}

	};

	AllPiecesOfPlayer allPiecesOfPlayer(int playerNum)
	{
		return AllPiecesOfPlayer(this, playerNum, ROWS, COLS);
	}
	
};








#endif // ifndef __EX4_HEADER__

