#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <set>
#include "SpookyHash.h"
#include <unordered_map>

typedef std::size_t size_t;

static const unsigned int MAX_STATES = 1024;
static const unsigned int BOARD_WIDTH = 8;
static const unsigned int BOARD_HEIGHT = 8;
static const unsigned int BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT;

static const unsigned int PLAYER_BIT = 0b10000000;
static const unsigned int ENPASSANT_BIT = 0b00100000;
static const unsigned int EDGE_BIT = 0b00010000;
static const unsigned int PIECE_BITS = 0b00000111;

enum Piece
{
	NONE, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING
};

struct Move
{
	uint8_t count = 0;
	int indices[4] = { -1, -1, -1, -1 };
	uint8_t new_values[4] = { 0x00, 0x00, 0x00, 0x00 };
};

struct Board
{
	uint8_t data[BOARD_SIZE];
};

struct GameState
{
	bool evaluated = false;
	int value = 0;
	uint8_t child_count = 0;
	GameState * children[256];
	Board board;
};

struct HashBoard
{
	size_t operator()(const Board& board) const
	{
		size_t hash = 0x32;
		SpookyHash::Hash32(board.data, sizeof(board.data[0]) * BOARD_SIZE, hash);
		return hash;
	}
};

struct EqualsBoard
{
	size_t operator()(const Board& lhs, const Board& rhs) const
	{
		for (int i = 0; i < BOARD_SIZE / 8; i++)
			if (((uint64_t*)lhs.data)[i] != ((uint64_t*)rhs.data)[i])
				return false;
		return true;
	}
};

void setupGameState(GameState* state)
{
	/*
		WRONG, REDO
	*/
	uint8_t b[BOARD_SIZE] =
	{
		0x11, 0x12, 0x13, 0x15, 0x14, 0x13, 0x12, 0x11,
		0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
		0x50, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x50,
		0x50, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x50,
		0x50, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x50,
		0x50, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x50,
		0x90, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x90,
		0x91, 0x92, 0x93, 0x94, 0x95, 0x93, 0x92, 0x91
	};
	for (int i = 0; i < BOARD_SIZE; i++)
		state->board.data[i] = b[i];
}

void printBoard(GameState* state)
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			unsigned int piece = state->board.data[j + i * BOARD_WIDTH] & PIECE_BITS;
			switch (piece)
			{
			case NONE:
				std::cout << "_";
				break;
			default:
				std::cout << piece;
			}
		}
		std::cout << std::endl;
	}
}

std::vector<Move> generateMoves(GameState * state)
{
	std::vector<Move> moves;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		unsigned int piece = state->board.data[i] & PIECE_BITS;
		if (piece == NONE)
			continue;
		Move move;
		switch (piece)
		{
		case PAWN:
			
			break;
		case ROOK:
			break;
		case KNIGHT:
			break;
		case BISHOP:
			break;
		case QUEEN:
			break;
		case KING:
			break;
		}
	}
}

Board applyMove(const Board * board, Move move)
{
	Board b = *board;
	for (int i = 0; i < move.count; i++)
		b.data[move.indices[i]] = move.new_values[i];
	return b;
}

void alpha_beta_minmax(	GameState * state, std::vector<GameState>& states,
						std::unordered_map<Board, GameState*>& visited,
						int depth, int a, int b, int f)
{
	visited[state->board] = state;
	std::vector<Move> moves = generateMoves(state);
	if (depth == 0 || state->child_count == 0)
		return;
	for (Move move : moves)
	{
		Board board = state->board;
		applyMove(&board, move);
		GameState * move = nullptr;
		auto it = visited.find(board);
		if (it != visited.end()) {
			move = it->second;
		}
		else{
			states.push_back(GameState());
			setupGameState(&states.back());
			move = &state[0];
			alpha_beta_minmax(move, states, visited, depth + 1, a, b, -f);
		}
		state->children[state->child_count++] = move;

		if (f > 0) {
			state->value = state->value > move->value ? state->value : move->value;
			a = state->value > a ? state->value : a;
			if(a >= b)
				break;
		}
		else {
			state->value = state->value < move->value ? state->value : move->value;
			b = state->value < b ? state->value : b;
			if (a < b)
				break;
		}
	}
}

int main()
{
	std::cout << "This is Kobold!\n";
	std::unordered_map<Board, GameState*, HashBoard, EqualsBoard> visited;
	std::vector<GameState> states;
	states.push_back(GameState());
	setupGameState(&states[0]);
	visited[states[0].board] = &states[0];
	printBoard(&states[0]);
	GameState * currentState = &states[0];

	int i;
	std::cin >> i;
	return 0;
}