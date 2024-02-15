#include "cf_match.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "card.h"

struct _s_match {
    card carta;
    cf_match next;
};

cf_match match_new(void) {
    return NULL;
}

cf_match match_add(cf_match match, card c) {
    assert(c != NULL);
    cf_match new_match = calloc(1, sizeof(struct _s_match));
    new_match->carta = c;
    new_match->next = NULL;

    if (match != NULL) {
        cf_match aux = match;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = new_match;
        aux = NULL;
    } else {
        match = new_match;
    }
    new_match = NULL;

    return match;
}

unsigned int match_size(cf_match match) {
    unsigned int r = 0;
    cf_match aux = match;
    while (aux != NULL) {

        aux = aux->next;
        r++;
    }

    return r;
}

bool is_match_correct(cf_match match) {
    bool r = false;
    cf_match aux = match;
    unsigned int num_player = 0;
    // Comienza el jugador 1 y la cant de cartas jugadas es par
    if (match != NULL && (card_player(match->carta) == 1) && (match_size(match) % 2 == 0)) {
        r = true;
        while (aux != NULL && r) {
            num_player = card_player(aux->carta);
            aux = aux->next;
            if (aux == NULL) {
                break;
            }
            r = num_player != card_player(aux->carta);
        }
    }
    aux = NULL;
    return r;
}

static int player_score(cf_match match, unsigned int player) {
    assert(player == 1 || player == 2);
    cf_match aux_current = match;
    cf_match aux_prev = NULL;
    bool isFlush = false;
    bool upwardsFlush = false;
    int score1 = 0;
    int score2 = 0;

    while (aux_current->next != NULL) {
        aux_prev = aux_current;
        aux_current = aux_current->next;
        score2 += card_drop_points(aux_prev->carta, aux_current->carta,
                                   isFlush, upwardsFlush);
        aux_prev = aux_current;
        aux_current = aux_current->next;
        if (aux_current == NULL) {
            break;
        }
        score1 += card_drop_points(aux_prev->carta, aux_current->carta,
                                   isFlush, upwardsFlush);
    }
    aux_current = NULL;
    aux_prev = NULL;
    return (player == 1) ? score1 : score2;
}

unsigned int match_winner(cf_match match) {
    return (player_score(match, 1) > player_score(match, 2)) ? 1 : 2;
}

unsigned int winner_total_points(cf_match match) {
    if (!is_match_correct(match)) {
        return -1;
    }

    int playerOneScore = player_score(match, 1);
    int playerTwoScore = player_score(match, 2);

    if (playerOneScore == playerTwoScore) {
        return 0;
    }
    return playerOneScore > playerTwoScore ? playerOneScore : playerTwoScore;
}

card* match_to_array(cf_match match) {
    cf_match aux = match;
    unsigned int size = match_size(match);
    card* r = calloc(size, sizeof(card));
    unsigned int num = 0;
    char palo;
    unsigned int player = 0;
    for (unsigned int i = 0; i < size; i++) {
        num = card_number(aux->carta);
        palo = card_suit(aux->carta);
        player = card_player(aux->carta);
        r[i] = card_new(num, palo, player);
        aux = aux->next;
    }
    aux = NULL;
    return r;
}

void match_dump(cf_match match) {
    card* array = match_to_array(match);
    for (unsigned int i = 0u; i < match_size(match); i++) {
        card_dump(array[i]);
    }
    free(array);
}

cf_match match_destroy(cf_match match) {
    cf_match aux = match;
    while (match->next != NULL) {
        match = match->next;
        card_destroy(aux->carta);
        aux->next = NULL;
        aux->carta = NULL;
        free(aux);
        aux = match;
    }
    card_destroy(aux->carta);
    aux->next = NULL;
    aux->carta = NULL;
    free(aux);
    return match;
}
