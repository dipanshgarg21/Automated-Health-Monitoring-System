# Automated Health Monitoring System

A comprehensive IoT-based health monitoring solution designed for automated health checkups in schools and educational institutes. This system combines Arduino sensors, Raspberry Pi interface, and cloud storage to streamline health data collection and guardian notification.

## 🎯 Overview

The Automated Health Monitoring System is designed to revolutionize how schools conduct routine health checkups. Instead of manual record-keeping and time-consuming processes, this system automates the collection of vital health metrics (body temperature, heart rate, and blood oxygen levels) and instantly notifies guardians via email.

**Key Components:**
- **Arduino**: Handles sensor data collection and cloud upload
- **Raspberry Pi**: Manages user interface, student database, and fingerprint authentication
- **Firebase**: Real-time cloud storage for sensor readings
- **MySQL**: Local database for student records and historical data
- **IFTTT**: Email notification service for guardians

## ✨ Features

### Core Functionality
- 📊 **Real-time Health Monitoring**: Continuous tracking of temperature, heart rate, and SpO2 levels
- 🔐 **Fingerprint Authentication**: Secure student identification using biometric sensors
- 📧 **Automated Guardian Notifications**: Instant email reports sent to guardians after checkup
- 💾 **Historical Data**: Access previous health records for any student
- 👤 **Student Management**: Easy registration and management of student profiles
- 🌐 **Cloud Integration**: Seamless data sync with Firebase Realtime Database

### User Interface
- Clean, intuitive Tkinter-based GUI
- Large, readable fonts suitable for quick operations
- Real-time display of sensor readings
- Easy navigation between student records

## 🏗️ System Architecture

```
┌─────────────────┐
│  MAX30100       │  Heart Rate & SpO2
│  Sensor         │───┐
└─────────────────┘   │
                      │
┌─────────────────┐   │    ┌──────────────┐
│  DS18B20        │   ├───►│   Arduino    │
│  Temperature    │───┘    │   + WiFi     │
└─────────────────┘        └──────┬───────┘
                                  │
                                  ▼
                           ┌─────────────┐
                           │  Firebase   │
                           │  Realtime   │
                           │  Database   │
                           └──────┬──────┘
                                  │
                                  ▼
┌─────────────────┐        ┌─────────────┐
│  Fingerprint    │◄───────┤ Raspberry   │
│  Sensor         │        │    Pi       │
└─────────────────┘        └──────┬──────┘
                                  │
                    ┌─────────────┼─────────────┐
                    ▼             ▼             ▼
              ┌──────────┐  ┌─────────┐  ┌──────────┐
              │  MySQL   │  │  IFTTT  │  │ Tkinter  │
              │ Database │  │  Email  │  │   GUI    │
              └──────────┘  └─────────┘  └──────────┘
```

## 🔧 Hardware Requirements

### Arduino Side
- Arduino board with WiFi capability (WiFiNINA compatible)
- MAX30100 Pulse Oximeter sensor
- DS18B20 Temperature sensor
- 4.7kΩ resistor (for DS18B20)
- Connecting wires
- Breadboard

### Raspberry Pi Side
- Raspberry Pi (3/4 recommended)
- Adafruit fingerprint sensor
- Monitor, keyboard, mouse
- Power supply

### Wiring Connections

**DS18B20 Temperature Sensor:**
- VCC → 3.3V
- GND → GND
- DATA → Arduino Pin 2 (with 4.7kΩ pull-up resistor to 3.3V)

**MAX30100 Pulse Oximeter:**
- VCC → 3.3V
- GND → GND
- SDA → Arduino SDA
- SCL → Arduino SCL

**Fingerprint Sensor (on Raspberry Pi):**
- Connected via UART on /dev/ttyS0 at 57600 baud

## 💻 Software Requirements

### Arduino
- Arduino IDE (1.8.x or later)
- Libraries:
  - `OneWire`
  - `DallasTemperature`
  - `Firebase_Arduino_WiFiNINA`
  - `Wire`
  - `MAX30100_PulseOximeter`

### Raspberry Pi
- Raspbian OS (or Raspberry Pi OS)
- Python 3.7+
- Python packages:
  ```
  tkinter
  PyQt5
  pymysql
  pyrebase
  adafruit-circuitpython-fingerprint
  ifttt-webhook
  PIL (Pillow)
  ```

### Cloud Services
- Firebase account with Realtime Database
- IFTTT account with Webhook and Gmail integration
- MySQL server (can be local on Raspberry Pi)

## 🚀 Installation & Setup

### Step 1: Firebase Configuration

1. Create a Firebase project at [Firebase Console](https://console.firebase.google.com)
2. Enable Realtime Database
3. Get your database URL and auth token
4. Note down the configuration details

### Step 2: Arduino Setup

1. **Install Required Libraries:**
   - Open Arduino IDE
   - Go to Sketch → Include Library → Manage Libraries
   - Install all libraries listed in requirements

2. **Configure WiFi and Firebase:**
   ```cpp
   #define FIREBASE_HOST "your-project.firebaseio.com"
   #define FIREBASE_AUTH "your-auth-token"
   #define WIFI_SSID "your-wifi-name"
   #define WIFI_PASSWORD "your-wifi-password"
   ```

3. **Upload Code:**
   - Connect Arduino via USB
   - Select correct board and port
   - Upload `ArduinoSide.ino`

### Step 3: MySQL Database Setup

1. **Install MySQL on Raspberry Pi:**
   ```bash
   sudo apt-get update
   sudo apt-get install mysql-server
   ```

2. **Create Database and Tables:**
   ```sql
   CREATE DATABASE sensordb;
   USE sensordb;
   
   CREATE TABLE STUDENT (
       STU_ID VARCHAR(10) PRIMARY KEY,
       STU_NAME VARCHAR(100),
       STU_MAIL VARCHAR(100),
       STU_FINGER INT
   );
   
   CREATE TABLE SENSORDATA (
       STU_ID VARCHAR(10),
       SPO2 INT,
       TEMP FLOAT,
       PULSE INT,
       RECORD_DATE DATETIME,
       FOREIGN KEY (STU_ID) REFERENCES STUDENT(STU_ID)
   );
   ```

3. **Create MySQL User:**
   ```sql
   CREATE USER 'localuser1'@'localhost' IDENTIFIED BY 'rootuser';
   GRANT ALL PRIVILEGES ON sensordb.* TO 'localuser1'@'localhost';
   FLUSH PRIVILEGES;
   ```

### Step 4: IFTTT Configuration

1. Create IFTTT account at [ifttt.com](https://ifttt.com)
2. Get Webhook key from [Webhook Settings](https://ifttt.com/maker_webhooks)
3. Create two applets:
   - **Applet 1**: Webhook "senddata" → Gmail
   - **Applet 2**: Direct Gmail sending (if using ifttt.gmail method)

### Step 5: Raspberry Pi Setup

1. **Install Python Dependencies:**
   ```bash
   pip3 install pymysql pyrebase4 adafruit-circuitpython-fingerprint
   pip3 install ifttt-webhook pillow PyQt5
   ```

2. **Configure the Script:**
   Edit `RaspberrypiSide.py` and update:
   ```python
   # Firebase config
   config = {
       "apiKey": "your-api-key",
       "authDomain": "your-project.firebaseapp.com",
       "databaseURL": "https://your-project.firebaseio.com",
       # ... other config
   }
   
   # IFTTT key
   IFTTT_KEY = 'your-ifttt-key'
   ```

3. **Run the Application:**
   ```bash
   python3 RaspberrypiSide.py
   ```

## 📖 Usage Guide

### Starting a Health Checkup

1. **Launch the Application:**
   - Run the Python script on Raspberry Pi
   - Main interface will appear

2. **Authentication Options:**
   
   **Option A: Fingerprint Authentication**
   - Click "Start" button next to "Start with Finger print"
   - Student places finger on sensor
   - System automatically identifies and loads student profile
   
   **Option B: Manual ID Entry**
   - Enter student's 10-digit ID in the text field
   - Click "OK" to load profile

3. **Conduct Checkup:**
   - Student profile appears with name, ID, and guardian email
   - Click "Start" button to begin sensor readings
   - Arduino collects data (takes ~50 seconds total):
     - Temperature: 30 seconds
     - Pulse: 20 seconds
     - SpO2: 20 seconds
   - Results display automatically

4. **Review Results:**
   - Current readings shown on screen
   - Email automatically sent to guardian
   - Data saved to database with timestamp

### Adding a New Student

1. Click "YES" next to "Add New Student"
2. Enter student details:
   - Student Name
   - 10-digit Student ID
   - Guardian Email Address
3. Click "SUBMIT"
4. Optional: Add fingerprint enrollment
   - Select "Yes" when prompted
   - Follow fingerprint enrollment process
   - Student places finger twice for verification

### Viewing Previous Records

1. Access existing student profile (fingerprint or ID)
2. Click "Previous record" button
3. View historical data:
   - Previous SpO2, Temperature, and Pulse readings
   - Timestamp of last checkup

## 🗄️ Database Schema

### STUDENT Table
| Column | Type | Description |
|--------|------|-------------|
| STU_ID | VARCHAR(10) | Primary key, 10-digit student ID |
| STU_NAME | VARCHAR(100) | Student's full name |
| STU_MAIL | VARCHAR(100) | Guardian's email address |
| STU_FINGER | INT | Fingerprint template ID |

### SENSORDATA Table
| Column | Type | Description |
|--------|------|-------------|
| STU_ID | VARCHAR(10) | Foreign key to STUDENT table |
| SPO2 | INT | Blood oxygen saturation (%) |
| TEMP | FLOAT | Body temperature (°F) |
| PULSE | INT | Heart rate (bpm) |
| RECORD_DATE | DATETIME | Timestamp of checkup |

### Firebase Structure
```json
{
  "Temperature": {
    "FinalTemp": {
      "T": <float_value>
    },
    "RealTime": {
      "push_id": { "T": <float_value> }
    }
  },
  "Pulse": {
    "FinalPulse": {
      "P": <int_value>
    },
    "RealTime2": {
      "push_id": { "P": <int_value> }
    }
  },
  "SpO2": {
    "FinalPulse": {
      "S": <int_value>
    },
    "RealTime3": {
      "push_id": { "S": <int_value> }
    }
  }
}
```

## ⚙️ Configuration

### Arduino Configuration
Located in `ArduinoSide.ino`:
- `FIREBASE_HOST`: Your Firebase database URL
- `FIREBASE_AUTH`: Firebase authentication token
- `WIFI_SSID`: WiFi network name
- `WIFI_PASSWORD`: WiFi password
- `ONE_WIRE_BUS`: Pin for temperature sensor (default: 2)
- `REPORTING_PERIOD_MS`: Sensor reading interval (default: 1000ms)

### Raspberry Pi Configuration
Located in `RaspberrypiSide.py`:
- Firebase config dictionary
- MySQL connection parameters
- IFTTT webhook key
- Fingerprint sensor UART port (default: /dev/ttyS0)
- GUI dimensions and positioning

## 🔮 Future Enhancements

- [ ] **Real-time Dashboard**: Web-based monitoring dashboard for administrators
- [ ] **Mobile App**: Cross-platform mobile application for guardians
- [ ] **Advanced Analytics**: Trend analysis and health pattern recognition
- [ ] **Alert System**: Automatic alerts for abnormal readings
- [ ] **Multi-language Support**: Interface localization
- [ ] **Batch Processing**: Concurrent checkups for multiple students
- [ ] **Cloud Sync**: Backup to multiple cloud providers
- [ ] **Report Generation**: Automated PDF health reports
- [ ] **Integration**: Connect with school management systems
- [ ] **Voice Guidance**: Audio instructions for students during checkup

## 🙏 Acknowledgments

- Adafruit for excellent sensor libraries
- Firebase for cloud infrastructure
- IFTTT for notification services
- The open-source community for various libraries used

---

**Note**: This system is designed for educational purposes and routine health monitoring. It should not replace professional medical equipment or advice. Always consult healthcare professionals for medical decisions.
