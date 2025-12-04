#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h> // Standard HID Keyboard Library
#include <Encoder.h> // Library for reading the rotary encoder

// --- PIN DEFINITIONS (Matching Final Wiring Map) ---
#define NUM_ROWS 3
#define NUM_COLS 3
#define ENCODER_CLK_PIN GP8
#define ENCODER_DT_PIN GP9
#define ENCODER_SW_PIN GP10

#define YES_BTN_PIN GP12 // Action Button 'Yes'
#define NO_BTN_PIN GP11  // Action Button 'No'

// Matrix Pins (Must match your wiring)
const int rowPins[NUM_ROWS] = {GP2, GP3, GP4};
const int colPins[NUM_COLS] = {GP5, GP6, GP7};

// I2C Pins (Matching your plan for the OLED)
#define OLED_SDA GP0
#define OLED_SCL GP1

// --- DISPLAY SETUP (128x32 Screen) ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 // <-- FINAL SIZE CONFIRMED 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- ENCODER SETUP ---
Encoder knob(ENCODER_CLK_PIN, ENCODER_DT_PIN);
long oldKnobPosition = 0;

// --- APP AND MENU DATA ---
// App display names and the command string (executable name)
struct App {
  const char* name;
  const char* command;
  int keycode; // Dummy keycode for mapping ease
};

App apps[] = {
  {"Minecraft", "minecraft", '1'},
  {"Roblox", "RobloxPlayer", '2'},
  {"Discord", "discord", '3'},
  {"Twitch", "twitch", '4'},
  {"File Exp", "explorer", '5'},
  {"Slack", "slack", '6'},
  {"Spotify", "spotify", '7'},
  {"Game Bar", "explorer", '8'},
  {"Fortnite", "FortniteLauncher", '9'}
};

const int NUM_APPS = sizeof(apps) / sizeof(apps[0]);
int currentSelection = 0;
bool selectionMode = false; // False = App Selection (Layer 0), True = Utility (Layer 1)

// --- FUNCTION PROTOTYPES ---
void launchApp(const char* command);
void scanMatrix();
void handleEncoder();
void updateDisplay(const char* line1, const char* line2);
void handleMenu();
void handleActionButtons();

// ====================================================================

void setup() {
  // Initialize all matrix pins
  for (int row = 0; row < NUM_ROWS; row++) {
    pinMode(rowPins[row], OUTPUT);
    digitalWrite(rowPins[row], HIGH); // Set rows high by default (sinking current)
  }
  for (int col = 0; col < NUM_COLS; col++) {
    pinMode(colPins[col], INPUT_PULLUP); // Use internal pull-up resistors on columns
  }

  // Initialize Action Buttons & Encoder Button
  pinMode(YES_BTN_PIN, INPUT_PULLUP);
  pinMode(NO_BTN_PIN, INPUT_PULLUP);
  pinMode(ENCODER_SW_PIN, INPUT_PULLUP);

  // Initialize Display (Wire is required for I2C)
  Wire.begin(OLED_SDA, OLED_SCL); 
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    for (;;); // Don't proceed if display fails
  }

  Keyboard.begin();
  
  // Display Homescreen at startup
  updateDisplay("Tamedesk V1.0", "READY");
}

void loop() {
  handleMenu();
  handleEncoder();
  handleActionButtons();
}

// ====================================================================
// --- MAIN LOGIC FUNCTIONS ---
// ====================================================================

void handleMenu() {
  if (selectionMode == false) { // Layer 0: App Selection Mode (9 Keys used for direct selection)
    
    scanMatrix(); // Check which of the 9 keys is pressed
    
    // Update display with selected app name
    updateDisplay(apps[currentSelection].name, "Launch: YES/NO");
    
  } else { // Layer 1: Utility/Navigation Mode
    
    updateDisplay("UTILITY MODE", "Vol/Mute Active");
    
  }
}

void handleEncoder() {
  long newKnobPosition = knob.read() / 4; // Divide by 4 for detents

  if (selectionMode == false) { // Layer 0: Use encoder to scroll through apps

    if (newKnobPosition != oldKnobPosition) {
      if (newKnobPosition > oldKnobPosition) {
        currentSelection = (currentSelection + 1) % NUM_APPS;
      } else {
        currentSelection = (currentSelection - 1 + NUM_APPS) % NUM_APPS;
      }
      oldKnobPosition = newKnobPosition;
    }
  } else { // Layer 1: Use encoder for Volume Control
    if (newKnobPosition != oldKnobPosition) {
      if (newKnobPosition > oldKnobPosition) {
        Keyboard.press(KEY_MEDIA_VOLUME_UP);
        Keyboard.release(KEY_MEDIA_VOLUME_UP);
      } else {
        Keyboard.press(KEY_MEDIA_VOLUME_DOWN);
        Keyboard.release(KEY_MEDIA_VOLUME_DOWN);
      }
      oldKnobPosition = newKnobPosition;
    }
    
    // Handle Encoder Push Button (Mute)
    if (digitalRead(ENCODER_SW_PIN) == LOW) {
      Keyboard.press(KEY_MEDIA_MUTE);
      Keyboard.release(KEY_MEDIA_MUTE);
      delay(200); // Debounce
    }
  }
}

void handleActionButtons() {
  // --- YES Button (GP12) ---
  if (digitalRead(YES_BTN_PIN) == LOW) {
    if (selectionMode == false) {
      // Layer 0: Confirm launch of selected app
      updateDisplay("LAUNCHING...", apps[currentSelection].name);
      launchApp(apps[currentSelection].command);
      delay(500);
      
    } else {
      // Layer 1: Do nothing, toggle to Layer 0
    }
    selectionMode = false; // Always return to App Selection Mode (Layer 0)
  }

  // --- NO Button (GP11) ---
  if (digitalRead(NO_BTN_PIN) == LOW) {
    // Always returns to Homescreen/App Select (Layer 0)
    selectionMode = false;
    currentSelection = 0;
    updateDisplay("Tamedesk V1.0", "HOMESCREEN");
    delay(500); // Debounce
  }
}

void scanMatrix() {
  for (int row = 0; row < NUM_ROWS; row++) {
    digitalWrite(rowPins[row], LOW); // Activate row

    for (int col = 0; col < NUM_COLS; col++) {
      if (digitalRead(colPins[col]) == LOW) {
        
        // Button is pressed, update selection to this key's index
        currentSelection = (row * NUM_COLS) + col;
        
        delay(50); // Debounce
        goto endScan; 
      }
    }
    digitalWrite(rowPins[row], HIGH); // Deactivate row
  }
  
  endScan:;
}

// --- UTILITY FUNCTIONS ---

void updateDisplay(const char* line1, const char* line2) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Line 1: Primary Message (App Name / Status)
  display.setCursor(0, 0);
  display.print(line1);
  
  // Line 2: Secondary Message (Launch prompt / Mode)
  display.setCursor(0, 16);
  display.print(line2);
  
  display.display();
}

void launchApp(const char* command) {
  // Sequence: Win+R -> Type Command -> Enter
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(100);

  Keyboard.print(command);
  delay(100);

  Keyboard.press(KEY_RETURN);
  Keyboard.release(KEY_RETURN);
  delay(100); 
}
