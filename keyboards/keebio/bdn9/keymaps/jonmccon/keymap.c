/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Here's the build command
// make keebio/bdn9:jonmccon CTPC=yes


// looks like you're going to get knob layers without the fancies
// work on your general layers


#include QMK_KEYBOARD_H



// Layers
enum bdn9_layers {
  _AUDITION_Edit,
  _AUDITION_Mark,
  _GENERAL
};

// Variables
enum bdn9_keycodes {
  AUDITION_Edit,
  AUDITION_Mark,
  GENERAL,
  COPA,
  COPY,
  PASTE,
  UNDO,
  REDO
};

// Layer change noises  
// #ifdef AUDIO_ENABLE

//     float layerSound[][2] = SONG(COIN_SOUND);
//     float layerSoundB[][2] = SONG(ONE_UP_SOUND);

// #endif

// keymaps
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    
    // AUDITION - Editing 
    /*
        | Layer         |           | 8 , 9 (Audition zoom) |
        | Press: Mute   | SPACE     | Press: Play/Pause  |
        | V             | '         | Volume Up          |
        | R             | ;         | Volume Down        |
     */
        
    [_AUDITION_Edit] = LAYOUT(
        KC_MUTE, KC_SPACE,  KC_MPLY,
        KC_V,    KC_QUOTE,  KC__VOLUP,
        KC_R,    KC_SCOLON, KC__VOLDOWN
    ),

    // AUDITION - Cues and Marking
    /*
        | Layer         |           | 8 , 9 (Audition zoom) |
        | Press: Mute   | SPACE     | Press: Play/Pause  |
        | V             | '         | Volume Up          |
        | R             | ;         | Volume Down        |
     */
        
    [_AUDITION_Mark] = LAYOUT(
        KC_MUTE, KC_SPACE,  KC_MPLY,
        KC_T,    KC_QUOTE,  KC__VOLUP,
        KC_C,    KC_SCOLON, KC__VOLDOWN
    ),
    
    // LOWER to be swapped
    /*
        | RESET          | N/A  | Media Stop |
        | Held: Layer 2  | Home | RGB Mode   |
        | Media Previous | End  | Media Next |
     */
    [_GENERAL] = LAYOUT(
        RESET,   MI_C_1,   MI_C_2,
        PASTE,   REDO,     KC__VOLUP,
        COPY,    UNDO,     KC__VOLDOWN        
    ),
};


/* Copa
 * Sends Ctrl+C on press and
 * Sends Ctrl+V on release
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case COPA:
    	 if (record->event.pressed) {
                SEND_STRING( SS_LCMD("c"));
            } else {
                SEND_STRING( SS_LCMD("v"));
            }
            return false;
            break;
        case COPY:
          if (record->event.pressed) {
            // when keycode ZMIN is pressed
            SEND_STRING(SS_LCMD("c"));
          } else {
            // when keycode is released
          }
          return false;
          break;
        case PASTE:
          if (record->event.pressed) {
            // when keycode ZMIN is pressed
            SEND_STRING(SS_LCMD("v"));
          } else {
            // when keycode is released
          }
          return false;
          break;
        case UNDO:
          if (record->event.pressed) {
            // when keycode ZMIN is pressed
            SEND_STRING(SS_LCMD("z"));
          } else {
            // when keycode is released
          }
          return false;
          break;
        case REDO:
          if (record->event.pressed) {
            // when keycode ZADD is pressed
            SEND_STRING(SS_LCMD(SS_LSFT("z")));
          } else {
            // when keycode is released
          }
          return false;
          break;
    }
    return true;
};





// Audition zoom knob
// void encoder_update_user(uint8_t index, bool clockwise) {
//     if (index == 1) {
//         if (clockwise) {
//             tap_code(KC_8);
//         } else {
//             tap_code(KC_9);
//         }
//     }
// }


// Layer handling via knob 0
// Audition zoom via knob 1
// Possibly pass a variable in here in to replace the 8 & 9 keys
uint8_t selected_layer = 0;
void encoder_update_user(uint8_t index, bool clockwise) {
  
    if (index == 1) {
      if (clockwise) {
            tap_code(KC_8);
        } else {
            tap_code(KC_9);
        }
    } else {
      if (index == 0) {
        if (!clockwise && selected_layer  < 2) {
          selected_layer ++;
        } else if (clockwise && selected_layer  > 0){
          selected_layer --;
        }
        layer_clear();
        layer_on(selected_layer);
    }
}}






// turn on oled
// void i2c_init(void);
void keyboard_post_init_user(void) {
  wait_ms(2000);
  oled_init(OLED_ROTATION_180);
}
// bool oled_on(void);

// talk to oled
#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("L:\n"), false);

    switch (get_highest_layer(layer_state)) {
        case _AUDITION_Edit:
            oled_write_P(PSTR("Audition\nEditing\n"), false);
            break;
        case _AUDITION_Mark:
            oled_write_P(PSTR("Audition\nCue Markers\n"), false);
            break;
        case _GENERAL:
            oled_write_P(PSTR("GENERAL\n \n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined\n"), false);
    }
}
#endif
