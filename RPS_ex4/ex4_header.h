#ifndef __EX4_HEADER__
#define __EX4_HEADER__

#include <memory>
#include <utility>
#include <array>
#include <tuple>

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

	public:

		Iterator() : cellPtr(nullptr), row(-1), col(-1) {}

		Iterator(PieceInfo<GAME_PIECE>* ptr, int row, int col) : cellPtr(ptr), row(row), col(col) {}

		Iterator operator++()
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
		if(col < 0 || row < 0 || row > ROWS || col > COLS)
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
		if (col < 0 || row < 0 || row > ROWS || col > COLS || player >= NumberOfPlayers)
			return nullptr;

		auto ret = std::move(getPiece(row, col));

		table[row*COLS + col] = std::move(std::make_unique<const std::pair<int, GAME_PIECE>>(player, GAME_PIECE(piece)));

		return std::move(ret);
	}
	
	Iterator begin()
	{
		return Iterator(&table[0], 0, 0);
	}

	Iterator end()
	{
		return Iterator();
	}
	
};








#endif // ifndef __EX4_HEADER__

