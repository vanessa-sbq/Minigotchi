#include "tictactoeViewer.h"

static TicTacToe *_ttt;
static bool toBeReset = false;

void ticTacToeViewer_setTicTacToe(TicTacToe* ttt) {
    _ttt = ttt;
}

void ticTacToeMayBeResetNow() {
    toBeReset = false;
}

TicTacToe* ticTacToeViewer_get_saved_ttt() {
    return _ttt;
}

void ticTacToeViewer_draw() {

    vg_clear_screen();

    if (!toBeReset) {

        state_ttt_t currentTicTacToe = ticTacToe_get_state(_ttt);

        // Background elements
        wrapper_draw_background(0);

        draw_button(ticTacToe_get_quitButton(_ttt));

        // Foreground elements
        if (currentTicTacToe == TX_RX_DECISION_TTT) {
            draw_button(ticTacToe_get_hostButton(_ttt));
            draw_button(ticTacToe_get_guestButton(_ttt));
        }

        if (currentTicTacToe == CURRENT_PLAYER_TURN_TTT) {
            if (ticTacToe_get_otherPlayersMoves(_ttt)[0]) drawSprite(guiDrawer_get_ttt_o(), 246,110);
            if (ticTacToe_get_otherPlayersMoves(_ttt)[1]) drawSprite(guiDrawer_get_ttt_o(), 464,110);
            if (ticTacToe_get_otherPlayersMoves(_ttt)[2]) drawSprite(guiDrawer_get_ttt_o(), 682,110);
            if (ticTacToe_get_otherPlayersMoves(_ttt)[3]) drawSprite(guiDrawer_get_ttt_o(), 246,328);
            if (ticTacToe_get_otherPlayersMoves(_ttt)[4]) drawSprite(guiDrawer_get_ttt_o(), 464,328);
            if (ticTacToe_get_otherPlayersMoves(_ttt)[5]) drawSprite(guiDrawer_get_ttt_o(), 682,328);
            if (ticTacToe_get_otherPlayersMoves(_ttt)[6]) drawSprite(guiDrawer_get_ttt_o(), 246,546);
            if (ticTacToe_get_otherPlayersMoves(_ttt)[7]) drawSprite(guiDrawer_get_ttt_o(), 464,546);
            if (ticTacToe_get_otherPlayersMoves(_ttt)[8]) drawSprite(guiDrawer_get_ttt_o(), 682,546);

            if (ticTacToe_get_currentPlayersMoves(_ttt)[0]) drawSprite(guiDrawer_get_ttt_x(), 246,110);
            if (ticTacToe_get_currentPlayersMoves(_ttt)[1]) drawSprite(guiDrawer_get_ttt_x(), 464,110);
            if (ticTacToe_get_currentPlayersMoves(_ttt)[2]) drawSprite(guiDrawer_get_ttt_x(), 682,110);
            if (ticTacToe_get_currentPlayersMoves(_ttt)[3]) drawSprite(guiDrawer_get_ttt_x(), 246,328);
            if (ticTacToe_get_currentPlayersMoves(_ttt)[4]) drawSprite(guiDrawer_get_ttt_x(), 464,328);
            if (ticTacToe_get_currentPlayersMoves(_ttt)[5]) drawSprite(guiDrawer_get_ttt_x(), 682,328);
            if (ticTacToe_get_currentPlayersMoves(_ttt)[6]) drawSprite(guiDrawer_get_ttt_x(), 246,546);
            if (ticTacToe_get_currentPlayersMoves(_ttt)[7]) drawSprite(guiDrawer_get_ttt_x(), 464,546);
            if (ticTacToe_get_currentPlayersMoves(_ttt)[8]) drawSprite(guiDrawer_get_ttt_x(), 682,546);
        }

        if (ticTacToe_get_state(_ttt) == CPLAYER_LOST){
            drawSprite(guiDrawer_get_rpsLose_sprite(), 100, 100);
        }

        if (ticTacToe_get_state(_ttt) == CPLAYER_WON){
            drawSprite(guiDrawer_get_rpsWin_sprite(), 410, 200);
            
            Sprite* reward_sprite = item_get_sprite(ticTacToe_get_reward(_ttt));
            drawSprite(reward_sprite, (1152 - sprite_get_width(reward_sprite)) / 2, 400);
        }

        if (ticTacToe_get_state(_ttt) == TIE_TTT){
            drawSprite(guiDrawer_get_rpsTie_sprite(), 500, 200);
        }


        draw_cursor(ticTacToe_get_cursor(_ttt));
    } else {
        ticTacToe_set_state(_ttt, TX_RX_DECISION_TTT);
        toBeReset = false;
    }

    vg_page_flip();
}
