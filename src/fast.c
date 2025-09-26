#include <stdio.h>

#define SECRET_CHECKSUM 0xA512

typedef struct {
    int score;
    int checksum;
} score_t;

int make_checksum(int num) {
    const int secret = SECRET_CHECKSUM;
    return (num ^ secret) + (num << 3);
}

void save_score(int score) {
    score_t data = {0};
    data.score = score;
    data.checksum = make_checksum(score);
    FILE *f = fopen("score.dt", "wb");
    if (f) {
        fwrite(&data, sizeof(score_t), 1, f);
        fclose(f);
    }
}

int load_score() {
    int res = 0;
    FILE *f = fopen("score.dt", "rb");
    if (f) {
        score_t data = {0};
        size_t n = fread(&data, sizeof(score_t), 1, f);
        fclose(f);
        if (data.checksum == make_checksum(data.score))
            res = data.score;
    }
    return res;
}

int main() {
    int score = 12345;
    save_score(score);
    
    int lscore = load_score();
    printf("score=%d, load_score=%d\n", score, lscore);
    return 0;
}