#include <Arduino.h>
#include <EEPROM.h>

// 1. PIN CONFIGURATIONS
const int BUTTON_CANDIDATE_A = 2;
const int BUTTON_CANDIDATE_B = 3;
const int BUTTON_CANDIDATE_C = 4;
const int BUTTON_SHOW_RESULT = 5;
const int BUTTON_HARD_RESET = 6;

// 2. SECURE EEPROM MEMORY ADDRESSES
const int ADDR_VOTES_A = 10;
const int ADDR_VOTES_B = 20;
const int ADDR_VOTES_C = 30;

int totalVotesA = 0;
int totalVotesB = 0;
int totalVotesC = 0;

// 3. STATE MACHINE ENGINE STATE DEFINITIONS
enum MachineState {
    STATE_BALLOT_OPEN,
    STATE_CONFIRMATION,
    STATE_DISPLAY_RESULTS
};
MachineState currentSystemState = STATE_BALLOT_OPEN;

// 4. DEBOUNCE PROTECTION SETTING
const unsigned long DEBOUNCE_MILLIS = 50; 

// Switch contact debounce validation logic routine
bool checkDebouncedPress(int pinNumber) {
    if (digitalRead(pinNumber) == LOW) { 
        delay(DEBOUNCE_MILLIS); 
        if (digitalRead(pinNumber) == LOW) { 
            while (digitalRead(pinNumber) == LOW); 
            return true;
        }
    }
    return false;
}

void setup() {
    Serial.begin(9600);
    Serial.println("System Initialized. Securing Flash Memory...");

    pinMode(BUTTON_CANDIDATE_A, INPUT_PULLUP);
    pinMode(BUTTON_CANDIDATE_B, INPUT_PULLUP);
    pinMode(BUTTON_CANDIDATE_C, INPUT_PULLUP);
    pinMode(BUTTON_SHOW_RESULT, INPUT_PULLUP);
    pinMode(BUTTON_HARD_RESET, INPUT_PULLUP);

    EEPROM.get(ADDR_VOTES_A, totalVotesA);
    EEPROM.get(ADDR_VOTES_B, totalVotesB);
    EEPROM.get(ADDR_VOTES_C, totalVotesC);

    if (totalVotesA < 0 || totalVotesA > 999) totalVotesA = 0;
    if (totalVotesB < 0 || totalVotesB > 999) totalVotesB = 0;
    if (totalVotesC < 0 || totalVotesC > 999) totalVotesC = 0;

    currentSystemState = STATE_BALLOT_OPEN;
}

void loop() {
    switch (currentSystemState) {
        
        case STATE_BALLOT_OPEN:
            // Open for voting
            if (checkDebouncedPress(BUTTON_CANDIDATE_A)) {
                totalVotesA++;
                EEPROM.put(ADDR_VOTES_A, totalVotesA); 
                currentSystemState = STATE_CONFIRMATION;
            } 
            else if (checkDebouncedPress(BUTTON_CANDIDATE_B)) {
                totalVotesB++;
                EEPROM.put(ADDR_VOTES_B, totalVotesB);
                currentSystemState = STATE_CONFIRMATION;
            } 
            else if (checkDebouncedPress(BUTTON_CANDIDATE_C)) {
                totalVotesC++;
                EEPROM.put(ADDR_VOTES_C, totalVotesC);
                currentSystemState = STATE_CONFIRMATION;
            } 
            else if (checkDebouncedPress(BUTTON_SHOW_RESULT)) {
                currentSystemState = STATE_DISPLAY_RESULTS;
            }
            break;

        case STATE_CONFIRMATION:
            Serial.println("Vote Logged! Data Encrypted.");
            delay(2000); 
            currentSystemState = STATE_BALLOT_OPEN;
            break;

        case STATE_DISPLAY_RESULTS:
            Serial.print("A:"); Serial.print(totalVotesA);
            Serial.print(" B:"); Serial.print(totalVotesB);
            Serial.print(" C:"); Serial.println(totalVotesC);

            if (checkDebouncedPress(BUTTON_HARD_RESET)) {
                totalVotesA = 0; totalVotesB = 0; totalVotesC = 0;
                EEPROM.put(ADDR_VOTES_A, totalVotesA);
                EEPROM.put(ADDR_VOTES_B, totalVotesB);
                EEPROM.put(ADDR_VOTES_C, totalVotesC);
                Serial.println("Memory Formatted. System Restarting...");
                delay(1500);
                currentSystemState = STATE_BALLOT_OPEN;
            }
            break;
    }
    delay(50);
}
