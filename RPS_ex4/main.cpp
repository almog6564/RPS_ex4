#include <string>
#include <utility>

#include "ex4_header.h"
#include "unit_test_util.h"

using std::string;
using std::get;

static bool test1(){
	GameBoard<12, 7, string, 4> board;
	int i = 0;
	for (int row = 0; row < 12; row++){
		for(int col =0; col < 7; col++){
			board.setPiece(row, col, "Piece", i);
			i = (i+1)%4;
		}
	}
	int occurence[4]= {0,0,0,0};
	for(auto pieceInfo : board){
		occurence[get<3>(pieceInfo)]++;
	}
	for(int i = 0;i<4;i++){
		if (occurence[i] != 21){
			return false;
		}
	}
	return true;
}

int main(){
	RUN_TEST(test1);
	return 0;
}
