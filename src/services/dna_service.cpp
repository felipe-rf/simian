#include "dna_service.hpp"

bool check_direction(const std::vector<std::string>& dna, int x, int y, int dx, int dy) {
    char c = dna[x][y];
    int count = 0;
    int n = dna.size();

    for (int i = 0; i < 4; i++) {
        int nx = x + dx * i;
        int ny = y + dy * i;

        if (nx >= n || ny >= n || nx < 0 || ny < 0) return false;
        if (dna[nx][ny] != c) return false;
    }

    return true;
}

bool isSimian(const std::vector<std::string>& dna) {
    if (dna.empty()) return false; // Empty DNA is not valid
    if(dna.size() != dna[0].size()) return false; // Ensure it's a square matrix
    int n = dna.size();
    int sequences = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            // Horizontal
            if (check_direction(dna, i, j, 0, 1)) sequences++;

            // Vertical
            if (check_direction(dna, i, j, 1, 0)) sequences++;

            // Diagonal ↘
            if (check_direction(dna, i, j, 1, 1)) sequences++;

            // Diagonal ↙
            if (check_direction(dna, i, j, 1, -1)) sequences++;

            if (sequences >= 1) return true;
        }
    }

    return false;
}