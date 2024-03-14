#pragma once
#include <string>
#include <optional>
#include <iostream>
#include <vector>
#include <limits>

namespace Smyslov {
typedef unsigned long long U64;

enum class Square {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    EMPTY_SQUARE
};

enum File {A, B, C, D, E, F, G, H};
enum Rank {First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth};





namespace file {
inline constexpr U64 A = 0x101010101010101ULL;
inline constexpr U64 B = 0x202020202020202ULL;
inline constexpr U64 C = 0x404040404040404ULL;
inline constexpr U64 D = 0x808080808080808ULL;
inline constexpr U64 E = 0x1010101010101010ULL;
inline constexpr U64 F = 0x2020202020202020ULL;
inline constexpr U64 G = 0x4040404040404040ULL;
inline constexpr U64 H = 0x8080808080808080ULL;
}

namespace rank {
inline constexpr U64 first   = 0xff;
inline constexpr U64 second  = 0xff00;
inline constexpr U64 third   = 0xff0000;
inline constexpr U64 fourth  = 0xff000000;
inline constexpr U64 fifth   = 0xff00000000;
inline constexpr U64 sixth   = 0xff0000000000;
inline constexpr U64 seventh = 0xff000000000000;
inline constexpr U64 eighth  = 0xff00000000000000;
}


enum class Castling {None = 0, WhiteKingside = 1, WhiteQueenside = 2, BlackKingside = 4, BlackQueenside = 8};

enum Result {WHITE_WIN, BLACK_WIN, DRAW, EMPTY};


struct Score
{
    void set_score(Result res)
    {
        switch(res)
        {
            case Result::WHITE_WIN:
                _white = std::numeric_limits<int>::max();
                _black = std::numeric_limits<int>::lowest();
                break;
            case Result::BLACK_WIN:
                _white = std::numeric_limits<int>::lowest();
                _black = std::numeric_limits<int>::max();
                break;
            case Result::DRAW:
                _white = 0;
                _black = 0;
                break;
            case Result::EMPTY:
                _white = Result::EMPTY;
                _black = Result::EMPTY;
                break;
            default:
                break;
        }
        
    }

    int white_score() const {return _white;}
    int black_score() const {return _black;}



    private:
    int _white = Result::EMPTY;
    int _black  = Result::EMPTY;
};
/*
White kingside castling: 0001
0010
0100
1000
*/
enum class GameResult {WHITE_WINS, BLACK_WINS, DRAW};

enum class Color {WHITE, BLACK, NONE}; 
enum class Piece {PAWN = 2, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY}; // starts at 2 to access position bitboard array

enum class Flag {
    QUIET,
    DOUBLE_PAWN,
    KING_CASTLE,
    QUEEN_CASTLE,
    EN_PASSANT,
    CAPTURE,
    PROMOTE_QUEEN,
    PROMOTE_KNIGHT,
    PROMOTE_ROOK,
    PROMOTE_BISHOP,
    PROMOTE_KNIGHT_CAPTURE,
    PROMOTE_QUEEN_CAPTURE,
    PROMOTE_ROOK_CAPTURE,
    PROMOTE_BISHOP_CAPTURE,
};

inline U64 set_bit(Square sq) {
    return 0x1ULL << static_cast<int>(sq);
}

inline bool is_empty(U64 bb)  {
    return bb == 0x0ULL;
}

inline bool has(U64 bb, Square square) {
    U64 check_sq = bb & set_bit(square);
    return !(is_empty(check_sq));
}


struct GameState
{
    int fiftyMove;
    short castlingPerm;
    Piece captured;
    Square enPassant;

    GameState(int move50, short castling, Piece capture, Square enPas)
    : enPassant(enPas), captured(capture), castlingPerm(castling), fiftyMove(move50)
    {}

    void set_enPassant(Square sq){ enPassant = sq;}

    void incrementFiftyMove() {fiftyMove++;}
    void decrementFiftyMove() {fiftyMove--;}

};

inline File get_file(Square square){
    int file = static_cast<int>(square) % 8;
    return static_cast<File>(file);
}
inline Rank get_rank(Square square){
    int rank = static_cast<int>(square) / 8;
    return static_cast<Rank>(rank);
}


template <typename T>
inline constexpr U64 get_file_mask(T square){
    int file = square % 8;
    switch (file) {
        case 0:
        return file::A;
        case 1:
        return file::B;
        case 2:
        return file::C;
        case 3:
        return file::D;
        case 4:
        return file::E;
        case 5:
        return file::F;
        case 6:
        return file::G;
        case 7:
        return file::H;
        default:
        return 0x0ULL;
    }
}

template <typename T>
inline constexpr U64 get_rank_mask(const T square){
    const int rank = square / 8;
    switch (rank) {
        case 0:
        return rank::first;
        case 1:
        return rank::second;
        case 2:
        return rank::third;
        case 3:
        return rank::fourth;
        case 4:
        return rank::fifth;
        case 5:
        return rank::sixth;
        case 6:
        return rank::seventh;
        case 7:
        return rank::eighth;
        default:
        return 0x0ULL;
    }
}

template <typename T>
inline constexpr U64 mask_west(const T sq)
{
    U64 mask = 0x0ULL;

    const U64 stop = get_file_mask(sq);

    for (U64 file = file::A; file < stop; file <<= 1)
    {

        mask |= file;
    }

    return mask;

}

template <typename T>
inline constexpr U64 mask_east(const T sq)
{
    U64 mask = 0x0ULL;
    const U64 stop = get_file_mask(sq);

    for (U64 file = file::H; file > stop; file >>=1)
    {
        mask |= file;
    }

    return mask;
}

inline std::optional<Square> try_offset(Square sq, int file_offset, int rank_offset){
    int file = get_file(sq) + file_offset;
    int rank = get_rank(sq) + rank_offset;
    int new_square = rank * 8 + file;
    if (file >= 8 || file < 0 || rank >= 8 || rank < 0) 
    {
        return {};
    }

    return static_cast<Square>(new_square);
}

inline std::vector<std::string> split_fen(std::string str, char split)
{
    std::string word = "";
    std::vector<std::string> result;
    for (auto i : str)
    {
        if (i == split && word != "")
        {
            result.push_back(word);
            word = "";
            continue;
        }
        word += i;
    }

    if (!word.empty())
    {
        result.push_back(word);
    }
    return result;

}

}


