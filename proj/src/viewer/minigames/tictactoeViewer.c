#include "tictactoeViewer.h"

static TicTacToe *_ttt;


void ticTacToeViewer_setTicTacToe(TicTacToe* ttt) {
    _ttt = ttt;
}

void ticTacToeViewer_draw() {

    vg_clear_screen();

    state_ttt_t currentTicTacToe = ticTacToe_get_state(_ttt);

    // Background elements
    wrapper_draw_background();

    // Foreground elements
    if (currentTicTacToe == TX_RX_DECISION_TTT) {
        draw_button(ticTacToe_get_quitButton(_ttt));
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

    draw_cursor(ticTacToe_get_cursor(_ttt));
    vg_page_flip();
}
