#pragma once
#include <vector>
#include "bitboards.hpp"
#include "types.hpp"
#include <string>

#include <iostream>

class Position { 
    private:
    std::vector<Move> moveHistory; 
    void update_castlingPerm(const Move move);
    public:
    U64 pieceBB[8];
    Color side;
    int enPassant;
    int castlingPerm;



    Position();
    std::optional<Move> uci_to_move(std::string uci);
    void start_position();
    U64 get_bitboard(const Color color, const Piece piece) const;
    U64 get_attacks(const Color color) const;
    bool can_castle(const Move move) const;
    void make_castle(const Move move);
    void make_en_passant(const Move move);
    bool pseudo_legal(const Move move) const;
    bool legal(const Move move) const;
    bool make_move(std::string);
    void undo_move();
    void print_board() const;
};
