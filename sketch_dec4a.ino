import board
import time
from kmk.kmk_keyboard import KMKKeyboard
from kmk.scanners.keypad import MatrixScanner
from kmk.keys import KC
from kmk.modules.macros import Macros
from kmk.modules.encoder import EncoderHandler
from kmk.modules.layers import Layers
from kmk.extensions.display import Display
from kmk.extensions import Extension
import displayio
from adafruit_display_text import label
from terminalio import FONT

# --- 1. CONFIGURATION ---
BOARD_NAME = "Tamedesk"
LAYER_NAMES = ["APPS", "UTILITY"]

# --- APP LAUNCH MACROS (using Win+R) ---
# NOTE: These use the Windows/Linux Run command (Win+R) followed by the application's executable name.
# These commands assume the application's path is already in the system's PATH variable.
KC.APP_MINECRAFT = KC.MACRO( (KC.LGUI(KC.R), KC.DF('minecraft'), KC.ENTER) )
KC.APP_ROBLOX = KC.MACRO( (KC.LGUI(KC.R), KC.DF('RobloxPlayer'), KC.ENTER) )
KC.APP_DISCORD = KC.MACRO( (KC.LGUI(KC.R), KC.DF('discord'), KC.ENTER) )
KC.APP_TWITCH = KC.MACRO( (KC.LGUI(KC.R), KC.DF('twitch'), KC.ENTER) ) 
KC.APP_FILEEXPLR = KC.MACRO( (KC.LGUI(KC.R), KC.DF('explorer'), KC.ENTER) )
KC.APP_SLACK = KC.MACRO( (KC.LGUI(KC.R), KC.DF('slack'), KC.ENTER) )
KC.APP_SPOTIFY = KC.MACRO( (KC.LGUI(KC.R), KC.DF('spotify'), KC.ENTER) )
KC.APP_XBOXBAR = KC.LGUI(KC.G) 
KC.APP_FORTNITE = KC.MACRO( (KC.LGUI(KC.R), KC.DF('FortniteLauncher'), KC.ENTER) ) 
KC.APP_MUTE = KC.LCTL(KC.LALT(KC.M)) 

# Yes/No Button Remaps (using unused F-keys to avoid conflicts)
KC.YES_BTN = KC.F13 
KC.NO_BTN = KC.F14

# --- 2. KMK SETUP ---
keyboard = KMKKeyboard()
keyboard.modules.append(Macros())
keyboard.modules.append(Layers())

ROWS = (board.GP2, board.GP3, board.GP4,)
COLS = (board.GP5, board.GP6, board.GP7,)
keyboard.matrix = MatrixScanner.for_rows_cols(ROWS, COLS, diode_orientation='ROW2COL')

# Action Buttons ('No'=GP11, 'Yes'=GP12)
ACTION_BUTTONS = (board.GP11, board.GP12,)
keyboard.matrix.cols += ACTION_BUTTONS
keyboard.matrix.rows += (None, None,)

# Encoder Setup
keyboard.modules.append(EncoderHandler())
keyboard.encoders = [
    (board.GP8, board.GP9, board.GP10,) 
]


# --- 3. KEYMAPS ---
# App launching keys (KC.MACRO(n) uses the INDEX of the app in the APP_NAMES list below)
KC.APP_1 = KC.MACRO(0) 
KC.APP_2 = KC.MACRO(1)
KC.APP_3 = KC.MACRO(2)
KC.APP_4 = KC.MACRO(3)
KC.APP_5 = KC.MACRO(4)
KC.APP_6 = KC.MACRO(5)
KC.APP_7 = KC.MACRO(6)
KC.APP_8 = KC.MACRO(7)
KC.APP_9 = KC.MACRO(8)

STREAM_LAYER = [
    KC.APP_1, KC.APP_2, KC.APP_3,    
    KC.APP_4, KC.APP_5, KC.APP_6,
    KC.APP_7, KC.APP_8, KC.APP_9,          
    KC.NO_BTN, KC.YES_BTN, 
]

NAV_LAYER = [
    KC.MACRO('MUTE_MIC'), KC.MEDIA_PLAY_PAUSE, KC.CLOSE_WINDOW,
    KC.VOLUP, KC.VOLDN, KC.TRNS,
    KC.TRNS, KC.TRNS, KC.TRNS,
    KC.NO_BTN, KC.YES_BTN,
]

keyboard.keymap = [
    STREAM_LAYER,
    NAV_LAYER,
]

# Encoder Map
keyboard.encoder_map = [
    (KC.TRNS, KC.TRNS, KC.TRNS), 
    (KC.VOLUP, KC.VOLDN, KC.MUTE), 
]


# --- 4. OLED SCREEN LOGIC ---
APP_NAMES = [
    ("Minecraft", KC.APP_MINECRAFT),
    ("Roblox", KC.APP_ROBLOX),
    ("Discord", KC.APP_DISCORD),
    ("Twitch", KC.APP_TWITCH),
    ("File Exp", KC.APP_FILEEXPLR),
    ("Slack", KC.APP_SLACK),
    ("Spotify", KC.APP_SPOTIFY),
    ("Game Bar", KC.APP_XBOXBAR),
    ("Fortnite", KC.APP_FORTNITE),
]

class MenuSystem(Extension):
    def __init__(self, display_ext, app_names, encoder_pins):
        self.display_ext = display_ext
        self.app_names = app_names
        self.encoder_pins = encoder_pins
        self.current_selection = 0
        self.encoder_value = 0
        self.last_encoder_update = 0.0
        self.last_key_press = 0.0
        
    def on_keyboard_start(self, keyboard):
        display = self.display_ext.display
        self.group = displayio.Group()
        display.show(self.group)

        self.layer_label = label.Label(FONT, text=f"LAYER: {LAYER_NAMES[0]}", color=0x00FF00, x=2, y=8)
        self.title_label = label.Label(FONT, text=BOARD_NAME, color=0xFFFFFF, x=35, y=28)
        self.app_label = label.Label(FONT, text="", color=0xFFFFFF, x=2, y=48)
        self.prompt_label = label.Label(FONT, text="", color=0xFFFFFF, x=70, y=8) 
        
        self.group.append(self.layer_label)
        self.group.append(self.title_label)
        self.group.append(self.app_label)
        self.group.append(self.prompt_label)

    def on_layers_change(self, keyboard, active_layers):
        self.current_selection = 0
        self.layer_label.text = f"LAYER: {LAYER_NAMES[active_layers[0]]}"

    def on_post_process_key(self, keyboard, key, is_pressed, coordinates):
        active_layer = keyboard.active_layers[0]

        if active_layer == 0:
            if is_pressed:
                self.last_key_press = time.monotonic()
                key_index = None
                try:
                    key_index = STREAM_LAYER.index(key)
                except ValueError:
                    pass

                if key_index is not None and key_index < len(self.app_names):
                    self.current_selection = key_index
                    self.update_display(keyboard)
                
                # Check for YES/NO buttons
                if key == KC.YES_BTN:
                    self.execute_app_launch(keyboard)
                elif key == KC.NO_BTN:
                    self.clear_selection()
        
        self.update_display(keyboard)
        return key

    def update_display(self, keyboard):
        active_layer = keyboard.active_layers[0]
        
        if active_layer == 0:
            # Display App Selection Mode
            selected_app_name = self.app_names[self.current_selection][0]
            self.title_label.text = selected_app_name
            self.app_label.text = f"Launch: {selected_app_name}?"
            self.prompt_label.text = "YES/NO"
            
        elif active_layer == 1:
            # Display Utility Mode (No specific menu logic)
            self.title_label.text = "UTILITY MODE"
            self.app_label.text = ""
            self.prompt_label.text = ""

    def execute_app_launch(self, keyboard):
        app_kc = self.app_names[self.current_selection][1]
        keyboard.send_hid_report(app_kc.code, app_kc.hid_type, app_kc.usage_id)
        self.title_label.text = f"LAUNCHING..."
        self.app_label.text = f"{self.app_names[self.current_selection][0]}"
        time.sleep(0.5)

    def clear_selection(self):
        self.current_selection = 0
        self.title_label.text = BOARD_NAME
        self.app_label.text = "Applications"
        self.prompt_label.text = ""

    def during_exec(self, keyboard):
        # Encoder selection logic (Only works on Layer 0)
        active_layer = keyboard.active_layers[0]
        if active_layer == 0:
            current_encoder_value = self.display_ext.encoder_value
            
            if current_encoder_value != self.encoder_value:
                # Rotation detected
                if current_encoder_value > self.encoder_value:
                    self.current_selection = (self.current_selection + 1) % len(self.app_names)
                else:
                    self.current_selection = (self.current_selection - 1 + len(self.app_names)) % len(self.app_names)
                
                self.encoder_value = current_encoder_value
                self.update_display(keyboard)


# --- 5. OLED INITIALIZATION ---
display_ext = Display(
    to_init=[
        (
            board.GP1,
            board.GP0,
            128,
            64,
        ),
    ]
)
keyboard.extensions.append(display_ext)

# Initialize the custom screen display logic
keyboard.extensions.append(MenuSystem(display_ext, APP_NAMES, [board.GP8, board.GP9, board.GP10]))

if __name__ == '__main__':
    keyboard.go()