# Digital Voting Machine with Secure Memory System

This repository contains the complete firmware implementation for a secure, bounce-protected Digital Voting Machine (DVM) built as part of my embedded systems internship project.

## 📋 Project Guidelines Fulfilled

### 1. Finite State Machine (FSM) Logic
The system uses a strict state machine structure (`STATE_BALLOT_OPEN`, `STATE_CONFIRMATION`, `STATE_DISPLAY_RESULTS`) to handle vote counting logically and securely. This structure ensures that a voter cannot bypass the validation sequence.

### 2. Secure Storage (EEPROM Integration)
To prevent data tampering or loss during sudden power failure, vote counts are backed up using non-volatile memory operations (`EEPROM.put()`) instantly after every button press. The system safely retrieves historical data logs (`EEPROM.get()`) during every startup reboot.

### 3. Output Interface System
The architecture is configured to handle live status messages and real-time candidate scores smoothly. In a physical setup, this outputs directly to an alphanumeric LCD character grid display or a 7-segment display.

### 4. Software Debounce Logic
To neutralize contact bounce noise from standard mechanical tactile switches, an automated 50ms timing buffer is integrated into the button polling loops. The logic locks further inputs until the user completely releases the switch, ensuring one press equals exactly one vote.
