#include <string>
#include <utility>
#include "ex4_header.h"
#include "unit_test_util.h"

using std::string;
using std::get;

static bool test1(){
	GameBoard<12, 7, char, 4> board;
	//int i = 0;
	//for (int row = 0; row < 12; row++){
	//	for(int col =0; col < 7; col++){
	//
	//		if ((row*7 + col) % 5 == 0)
	//			continue;
	//
	//		board.setPiece(row, col, 'J', i);
	//		i = (i+1)%4;
	//
	//
	//	}
	//}
	board.setPiece(1, 1, 'P', 0);
	board.setPiece(2, 3, 'R', 1);
	board.setPiece(3, 5, 'R', 1);
	board.setPiece(11, 6, 'R', 1);




	//int occurence[4]= {0,0,0,0};
	cout << "first for" << endl;
	for(auto pieceInfo : board){
		printf("row <%d> col <%d>\n", get<0>(pieceInfo), get<1>(pieceInfo));
		//occurence[get<3>(pieceInfo)]++;
		
	}
	cout << "second for" << endl;
	int playerNum = 0;
	for (auto pieceInfo : board.allPiecesOfPlayer(playerNum)) {
		printf("row <%d> col <%d>\n", get<0>(pieceInfo), get<1>(pieceInfo));
		for (auto pieceInfo : board) {
			printf("row <%d> col <%d>\n", get<0>(pieceInfo), get<1>(pieceInfo));
			//occurence[get<3>(pieceInfo)]++;
		}
		//occurence[get<3>(pieceInfo)]++;
	}
	cout << "second for" << endl;
	playerNum = 1;
	for (auto pieceInfo : board.allPiecesOfPlayer(playerNum)) {
		printf("row <%d> col <%d>\n", get<0>(pieceInfo), get<1>(pieceInfo));
		//occurence[get<3>(pieceInfo)]++;
	}

	cout << "third for" << endl;
	char piece = 'P';
	for (auto pieceInfo : board.allOccureneceOfPiece(piece)) {
		printf("row <%d> col <%d>\n", get<0>(pieceInfo), get<1>(pieceInfo));
		//occurence[get<3>(pieceInfo)]++;
	}
	cout << "third for" << endl;

	piece = 'R';
	for (auto pieceInfo : board.allOccureneceOfPiece(piece)) {
		printf("row <%d> col <%d>\n", get<0>(pieceInfo), get<1>(pieceInfo));
		//occurence[get<3>(pieceInfo)]++;
	}
	cout << "third for" << endl;

	piece = 'J';
	for (auto pieceInfo : board.allOccureneceOfPiece(piece)) {
		printf("row <%d> col <%d>\n", get<0>(pieceInfo), get<1>(pieceInfo));
		//occurence[get<3>(pieceInfo)]++;
	}

	piece = 'R';
	cout << "forth for" << endl;
	for (auto pieceInfo : board.allOccureneceOfPieceForPlayer(piece, 1)) {
		printf("row <%d> col <%d>\n", get<0>(pieceInfo), get<1>(pieceInfo));
		//occurence[get<3>(pieceInfo)]++;
	}

	
	cout << "last for" << endl;

	//for(int i = 0;i<4;i++){
	//	if (occurence[i] != 21){
	//		printf("occurence[%d] = %d\n", i, occurence[i]);
	//		//return false;
	//	}
	//}
	return true;
}

int main(){
	RUN_TEST(test1);
	return 0;
}
