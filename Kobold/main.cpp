#include <iostream>
#include <vector>
#include <set>
#include "SpookyHash.h"
#include <unordered_set>

static const unsigned int MAX_STATES = 1024;
static const unsigned int BOARD_WIDTH = 8;
static const unsigned int BOARD_HEIGHT = 8;
static const unsigned int BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT;

static const unsigned int PLAYER_BIT = 0b10000000;
static const unsigned int EMPTY_BIT = 0b01000000;
static const unsigned int ENPASSANT_BIT = 0b00100000;
static const unsigned int EDGE_BIT = 0b00010000;
static const unsigned int PIECE_BITS = 0b00000111;

enum Piece
{
	PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING
};

struct BoardBuffer
{
	uint8_t data[BOARD_SIZE * MAX_STATES];
	uint8_t * index = data;
};

struct GameState
{
	int value;
	uint8_t child_count;
	GameState * children[256];
	uint8_t * board;
};

/**
	GameState logic functions
**/

void calculateValue(GameState * state)
{
	state->value = 0;
	for (int i = 0; i < state->child_count; i++)
		state->value += state->children[i]->value;
}

void searchChildren(GameState * state, const std::unordered_set<GameState*>& visisted_states)
{

}

/**
	GameState function-objects for hashing
**/
struct HashState
{
	size_t operator()(const GameState* state) const
	{
		size_t hash = 0x32;
		SpookyHash::Hash32(state->board, sizeof(state->board[0]) * BOARD_SIZE, hash);
		return hash;
	}
};

struct EqualsState
{
	size_t operator()(const GameState* lhs, const GameState* rhs) const
	{
		if (lhs == rhs)
			return true;
		for (int i = 0; i < BOARD_SIZE / 8; i++)
			if (((uint64_t*)lhs->board)[i] != ((uint64_t*)rhs->board)[i])
				return false;
		return true;
	}
};

void setupGameState(GameState* state, BoardBuffer * buffer)
{
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
	state->board = buffer->index++;
	for (int i = 0; i < BOARD_SIZE; i++)
		state->board[i] = b[i];
}

void printBoard(GameState* state)
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			uint8_t square = state->board[j + i * BOARD_WIDTH];
			if (square & EMPTY_BIT)
				std::cout << '_';
			else
				std::cout << (square & PIECE_BITS);
		}
		std::cout << std::endl;
	}
}

int main()
{
	std::cout << "This is Kobold!\n";
	std::unordered_set<GameState*, HashState, EqualsState> visited_states;
	BoardBuffer boardBuffer;
	std::vector<GameState> states;
	states.push_back(GameState());
	setupGameState(&states[0], &boardBuffer);
	visited_states.insert(&states[0]);
	printBoard(&states[0]);

	GameState * currentState = &states[0];


	int i;
	std::cin >> i;
	return 0;
}