#include <Seeed_HM330X.h>

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL_OUTPUT SerialUSB
#else
    #define SERIAL_OUTPUT Serial
#endif

#define BAUDRATE 115200

#define SENS_BUF_SIZE 30
#define SAMPLING_RATE 1 // Hz

//num of PMx sized particles in ug/m^3
#define PM1_0_STD         5-1
#define PM2_5_STD         7-1
#define PM10__STD         9-1

//num of particles below given size in um in 1 litre of air 
#define NUM_PARTICLES_0_3 17-1
#define NUM_PARTICLES_0_5 19-1
#define NUM_PARTICLES_1_0 21-1
#define NUM_PARTICLES_2_5 23-1
#define NUM_PARTICLES_5_0 25-1
#define NUM_PARTICLES_10_ 27-1

HM330X sensor;
uint8_t buf[SENS_BUF_SIZE];  // sensor data buffer

uint16_t pm1_0 = 0;
uint16_t pm2_5 = 0;
uint16_t pm10_ = 0;

uint16_t num_particles_0_3 = 0;
uint16_t num_particles_0_5 = 0;
uint16_t num_particles_1_0 = 0;
uint16_t num_particles_2_5 = 0;
uint16_t num_particles_5_0 = 0;
uint16_t num_particles_10_ = 0;

void parse_sensor_data(uint8_t* data) {
    if (data != NULL) {
        pm2_5 = (uint16_t)(data[PM2_5_STD] << sizeof(data)) + data[PM2_5_STD+1];
        pm1_0 = (uint16_t)(data[PM1_0_STD] << sizeof(data)) + data[PM1_0_STD+1];
        pm10_ = (uint16_t)(data[PM10__STD] << sizeof(data)) + data[PM10__STD+1];

        num_particles_0_3 = (uint16_t)(data[NUM_PARTICLES_0_3] << sizeof(data)) + data[NUM_PARTICLES_0_3+1];
        num_particles_0_5 = (uint16_t)(data[NUM_PARTICLES_0_5] << sizeof(data)) + data[NUM_PARTICLES_0_5+1];
        num_particles_1_0 = (uint16_t)(data[NUM_PARTICLES_1_0] << sizeof(data)) + data[NUM_PARTICLES_1_0+1];
        num_particles_2_5 = (uint16_t)(data[NUM_PARTICLES_2_5] << sizeof(data)) + data[NUM_PARTICLES_2_5+1];
        num_particles_5_0 = (uint16_t)(data[NUM_PARTICLES_5_0] << sizeof(data)) + data[NUM_PARTICLES_5_0+1];
        num_particles_10_ = (uint16_t)(data[NUM_PARTICLES_10_] << sizeof(data)) + data[NUM_PARTICLES_10_+1];
    }
}

void setup() {
    SERIAL_OUTPUT.begin(BAUDRATE);
    delay(100);
    SERIAL_OUTPUT.println("Serial initialized");

    SERIAL_OUTPUT.println("Initializing HM3301 Sensor...");
    if (sensor.init()) {
        SERIAL_OUTPUT.println("HM3301 init failed");
        while (true);
    }

    SERIAL_OUTPUT.println("HM3301 Sensor Initialized.");
}

void loop() {
    if (sensor.read_sensor_value(buf, 29)) {
        SERIAL_OUTPUT.println("HM3301 read failed!");
        return;
    }

    parse_sensor_data(buf);
    // for (size_t i = 0; i < 30; i++) SERIAL_OUTPUT.print(String(buf[i]) + " ");
    // SERIAL_OUTPUT.println();


    SERIAL_OUTPUT.print("PM1.0_STD:" + String(pm1_0) + ", PM2.5_STD:" + String(pm2_5) + ", PM10_STD:" + String(pm10_));
    SERIAL_OUTPUT.print(", NUM_PTC_0_3:" + String(num_particles_0_3) + ", NUM_PTC_0_5:" + String(num_particles_0_5) +
                        ", NUM_PTC_1_0:" + String(num_particles_1_0) + ", NUM_PTC_2_5:" + String(num_particles_2_5) +
                        ", NUM_PTC_5_0:" + String(num_particles_5_0) + ", NUM_PTC_10 :" + String(num_particles_10_) + "\n");

    delay(1000 / SAMPLING_RATE);
}
